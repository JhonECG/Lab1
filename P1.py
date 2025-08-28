import struct

class Record:
    FORMAT = '5s11s20s15sif'
    FORMAT_SIZE = struct.calcsize(FORMAT)
    
    def __init__(self, codigo, nombre, apellidos, carrera, ciclo, mensualidad):
        self.codigo = codigo
        self.nombre = nombre
        self.apellidos = apellidos
        self.carrera = carrera
        self.ciclo = ciclo
        self.mensualidad = mensualidad

    def pack(self):
        data = struct.pack(self.FORMAT, self.codigo.encode(), self.nombre.encode(),
                           self.apellidos.encode(), self.carrera.encode(),
                           self.ciclo, self.mensualidad)
        return data
    
    @staticmethod
    def unpack(data):
        codigo, nombre, apellidos, carrera, ciclo, mensualidad = struct.unpack(Record.FORMAT, data)
        return Record(codigo.decode().strip(), nombre.decode().strip(), apellidos.decode().strip(),
                       carrera.decode().strip(), ciclo, mensualidad)

class FixedRecord_FreeList:
    def __init__(self, filename):
        self.filename = filename
        open(self.filename, 'ab').close()

    def get_free_positions(self):
        free_positions = []
        with open(self.filename, 'rb') as f:
            current_pos = 0
            while True:
                data = f.read(Record.FORMAT_SIZE)
                if not data or len(data) < Record.FORMAT_SIZE:
                    break
                if all(x == 0 for x in data):
                    free_positions.append(current_pos)
                current_pos += 1
        return free_positions

    def load(self):
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
        free_positions = self.get_free_positions()
        data = record.pack()
        
        with open(self.filename, 'r+b') as f:
            if free_positions:
                pos = free_positions[0]
                f.seek(pos * Record.FORMAT_SIZE)
            else:
                f.seek(0, 2)
                pos = f.tell() // Record.FORMAT_SIZE
            f.write(data)
            return pos

    def readRecord(self, pos):
        with open(self.filename, 'rb') as f:
            f.seek(pos * Record.FORMAT_SIZE)
            data = f.read(Record.FORMAT_SIZE)
            if not data:
                return None
            return Record.unpack(data)

    def remover(self, pos):
        with open(self.filename, 'r+b') as f:
            f.seek(pos * Record.FORMAT_SIZE)
            f.write(b'\x00' * Record.FORMAT_SIZE)

class FixedRecord_MoveTheLast:
    def __init__(self, filename):
        self.filename = filename

    def load(self):
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
        data = record.pack()    
        with open(self.filename, 'r+b') as f:
            f.seek(0, 2)
            pos = f.tell() // Record.FORMAT_SIZE
            f.write(data)
            return pos
    
    def readRecord(self, pos):
        with open(self.filename, 'rb') as f:
            f.seek(pos * Record.FORMAT_SIZE)
            data = f.read(Record.FORMAT_SIZE)
            if not data:
                return None
            return Record.unpack(data)
    
    def remover(self, pos):
        with open(self.filename, 'r+b') as f:
            f.seek(0, 2)
            end_pos = f.tell() // Record.FORMAT_SIZE - 1
            if pos != end_pos:
                f.seek(end_pos * Record.FORMAT_SIZE)
                last_record_data = f.read(Record.FORMAT_SIZE)
                f.seek(pos * Record.FORMAT_SIZE)
                f.write(last_record_data)

            f.truncate(end_pos * Record.FORMAT_SIZE)