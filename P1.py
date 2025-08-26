import struct

class Record:
    # Definimos el formato binario: 5s, 11s, 20s, 15s, int, float
    FORMAT = '5s11s20s15sif'
    FORMAT_SIZE = struct.calcsize(FORMAT)  # Tamaño en bytes de un registro

    def __init__(self, codigo, nombre, apellidos, carrera, ciclo, mensualidad):
        # Atributos de cada registro
        self.codigo = codigo
        self.nombre = nombre
        self.apellidos = apellidos
        self.carrera = carrera
        self.ciclo = ciclo
        self.mensualidad = mensualidad

    def pack(self):
        # Convierte el objeto en bytes usando el formato definido
        data = struct.pack(
            self.FORMAT,
            self.codigo.encode(),
            self.nombre.encode(),
            self.apellidos.encode(),
            self.carrera.encode(),
            self.ciclo,
            self.mensualidad
        )
        return data

    @staticmethod
    def unpack(data):
        # Convierte bytes en un objeto Record
        codigo, nombre, apellidos, carrera, ciclo, mensualidad = struct.unpack(Record.FORMAT, data)
        return Record(
            codigo.decode().strip(),
            nombre.decode().strip(),
            apellidos.decode().strip(),
            carrera.decode().strip(),
            ciclo,
            mensualidad
        )

# --- Estrategia 1: Free List ---
class FixedRecord_FreeList:
    def __init__(self, filename):
        self.filename = filename
        self.free_positions = []  # Guarda posiciones borradas para reutilizarlas

    def load(self):
        # Cargar todos los registros del archivo
        res = []
        with open(self.filename, 'rb') as f:
            while True:
                data = f.read(Record.FORMAT_SIZE)
                if not data:
                    break
                record = Record.unpack(data)
                res.append(record)
        return res

    def addRecord(self, record: Record):
        # Agregar al final del archivo
        with open(self.filename, 'ab') as f:
            data = record.pack()
            f.write(data)

    def add(self, record):
        # Si hay posiciones libres, reutilizar
        if self.free_positions:
            pos = self.free_positions.pop(0)
            with open(self.filename, 'r+b') as f:
                f.seek(pos * Record.FORMAT_SIZE)
                data = record.pack()
                f.write(data)
        else:
            # Si no, agregar al final
            self.addRecord(record)

    def readRecord(self, pos):
        # Leer un registro en la posición indicada
        with open(self.filename, 'rb') as f:
            f.seek(pos * Record.FORMAT_SIZE)
            data = f.read(Record.FORMAT_SIZE)
            if not data:
                return None
            return Record.unpack(data)

    def remover(self, pos):
        # Eliminar sobrescribiendo con ceros y guardando posición libre
        with open(self.filename, 'r+b') as f:
            f.seek(pos * Record.FORMAT_SIZE)
            f.write(b'\x00' * Record.FORMAT_SIZE)
        self.free_positions.append(pos)

# --- Estrategia 2: Move The Last ---
class FixedRecord_MoveTheLast:
    def __init__(self, filename):
        self.filename = filename

    def load(self):
        # Cargar todos los registros del archivo
        res = []
        with open(self.filename, 'rb') as f:
            while True:
                data = f.read(Record.FORMAT_SIZE)
                if not data:
                    break
                record = Record.unpack(data)
                res.append(record)
        return res

    def add(self, record):
        # Agregar siempre al final
        with open(self.filename, 'ab') as f:
            data = record.pack()
            f.write(data)

    def readRecord(self, pos):
        # Leer registro en posición específica
        with open(self.filename, 'rb') as f:
            f.seek(pos * Record.FORMAT_SIZE)
            data = f.read(Record.FORMAT_SIZE)
            if not data:
                return None
            return Record.unpack(data)

    def remover(self, pos):
        # Estrategia: mover último registro a la posición eliminada
        with open(self.filename, 'r+b') as f:
            f.seek(0, 2)  # Ir al final
            end_pos = f.tell() // Record.FORMAT_SIZE - 1  # Último índice
            if pos != end_pos:
                # Leer último registro
                f.seek(end_pos * Record.FORMAT_SIZE)
                last_record_data = f.read(Record.FORMAT_SIZE)
                # Sobrescribir en la posición eliminada
                f.seek(pos * Record.FORMAT_SIZE)
                f.write(last_record_data)
            # Truncar archivo para quitar el último duplicado
            f.truncate(end_pos * Record.FORMAT_SIZE)
