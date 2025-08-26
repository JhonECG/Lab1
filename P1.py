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
        self.free_positions = []

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
    
    def addRecord(self, record: Record):
        with open(self.filename, 'ab') as f:
            data = record.pack()
            f.write(data)

    def add(self, record):
        if self.free_positions:
            pos = self.free_positions.pop(0)
            with open(self.filename, 'r+b') as f:
                f.seek(pos * Record.FORMAT_SIZE)
                data = record.pack()
                f.write(data)
        else:
            self.addRecord(record)

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
        self.free_positions.append(pos)
    
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
        with open(self.filename, 'ab') as f:
            data = record.pack()
            f.write(data)
    
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