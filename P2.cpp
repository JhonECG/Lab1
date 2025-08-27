#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

// Entrada de metadatos: describe un registro en el archivo de datos
struct MetadataEntry {
    long offset;  // posición (byte offset) dentro del archivo de datos
    long size;    // tamaño en bytes del registro
    bool active;  // indica si el registro está activo o eliminado
};

// Clase que representa un registro de matrícula
class Matricula {
    string codigo;
    int ciclo;
    float mensualidad;
    string observaciones;

public:
    Matricula() = default;
    Matricula(const string& c, int ci, float m, const string& o)
        : codigo(c), ciclo(ci), mensualidad(m), observaciones(o) {}

    // Getters
    const string& getCodigo() const { return codigo; }
    int getCiclo() const { return ciclo; }
    float getMensualidad() const { return mensualidad; }
    const string& getObservaciones() const { return observaciones; }

    // Empaquetar el objeto en un string binario (serialización)
    string pack() const {
        string data;
        size_t len_codigo = codigo.length();
        size_t len_obs = observaciones.length();

        // Reservar espacio para todo el registro (longitudes + datos)
        data.resize(sizeof(size_t) + len_codigo +
                    sizeof(int) + sizeof(float) +
                    sizeof(size_t) + len_obs);

        char* ptr = &data[0];

        // Guardar longitud y contenido de 'codigo'
        memcpy(ptr, &len_codigo, sizeof(size_t));
        ptr += sizeof(size_t);
        memcpy(ptr, codigo.c_str(), len_codigo);
        ptr += len_codigo;

        // Guardar ciclo
        memcpy(ptr, &ciclo, sizeof(int));
        ptr += sizeof(int);

        // Guardar mensualidad
        memcpy(ptr, &mensualidad, sizeof(float));
        ptr += sizeof(float);

        // Guardar longitud y contenido de 'observaciones'
        memcpy(ptr, &len_obs, sizeof(size_t));
        ptr += sizeof(size_t);
        memcpy(ptr, observaciones.c_str(), len_obs);

        return data;
    }

    // Deserializar (bytes → objeto Matricula)
    static Matricula unpack(const string& data) {
        Matricula m;
        const char* ptr = data.data();
        size_t len_codigo = 0;
        size_t len_obs = 0;

        memcpy(&len_codigo, ptr, sizeof(size_t));
        ptr += sizeof(size_t);
        m.codigo = string(ptr, len_codigo);
        ptr += len_codigo;

        memcpy(&m.ciclo, ptr, sizeof(int));
        ptr += sizeof(int);

        memcpy(&m.mensualidad, ptr, sizeof(float));
        ptr += sizeof(float);

        memcpy(&len_obs, ptr, sizeof(size_t));
        ptr += sizeof(size_t);
        m.observaciones = string(ptr, len_obs);

        return m;
    }
};

// Clase que administra el archivo de registros de longitud variable
class VariableRecordFile {
private:
    string dataFilename;            // archivo de datos
    string metaFilename;            // archivo de metadatos
    vector<MetadataEntry> metadata; // vector en memoria con las entradas

    // Cargar metadatos desde archivo
    void loadMetadata() {
        metadata.clear();
        ifstream metaFile(metaFilename, ios::binary);
        if (!metaFile.is_open()) {
            return;
        }

        MetadataEntry entry;
        while (metaFile.read(reinterpret_cast<char*>(&entry.offset), sizeof(long)) &&
               metaFile.read(reinterpret_cast<char*>(&entry.size), sizeof(long)) &&
               metaFile.read(reinterpret_cast<char*>(&entry.active), sizeof(bool))) {
            metadata.push_back(entry);
        }
    }

    // Guardar metadatos en archivo
    void saveMetadata() {
        ofstream metaFile(metaFilename, ios::binary | ios::trunc);
        if (!metaFile.is_open()) {
            return;
        }

        for (const MetadataEntry& entry : metadata) {
            metaFile.write(reinterpret_cast<const char*>(&entry.offset), sizeof(long));
            metaFile.write(reinterpret_cast<const char*>(&entry.size), sizeof(long));
            metaFile.write(reinterpret_cast<const char*>(&entry.active), sizeof(bool));
        }
    }

public:
    // Constructor: crea archivos vacíos de datos y metadatos
    VariableRecordFile(const string& dataFile, const string& metaFile)
        : dataFilename(dataFile), metaFilename(metaFile) {
        ofstream dataCreate(dataFilename, ios::binary | ios::trunc);
        dataCreate.close();
        ofstream metaCreate(metaFilename, ios::binary | ios::trunc);
        metaCreate.close();
        metadata.clear();
    }

    // Cargar todos los registros activos en memoria
    vector<Matricula> load() {
        vector<Matricula> res;
        for (size_t i = 0; i < metadata.size(); i++) {
            if (metadata[i].active) {
                Matricula record = readRecord(i);
                res.push_back(record);
            }
        }
        return res;
    }

    // Agregar un nuevo registro
    void add(const Matricula& record) {
        string packedData = record.pack();

        // Abrir archivo de datos en append
        ofstream dataFile(dataFilename, ios::binary | ios::app);
        if (!dataFile.is_open()) {
            return;
        }

        // Obtener posición (offset) actual
        long offset = dataFile.tellp();

        // Escribir los bytes del registro
        if (!dataFile.write(packedData.data(), packedData.size())) {
            return;
        }
        dataFile.close();

        // Crear entrada en metadatos
        MetadataEntry entry;
        entry.offset = offset;
        entry.size = packedData.size();
        entry.active = true;
        metadata.push_back(entry);

        // Guardar metadatos en archivo
        saveMetadata();
    }

    // Leer un registro por posición en el vector de metadatos
    Matricula readRecord(int pos) {
        if (pos < 0 || pos >= static_cast<int>(metadata.size())) {
            return Matricula(); // registro vacío
        }

        const MetadataEntry& entry = metadata[pos];
        if (!entry.active) {
            return Matricula(); // si está marcado como eliminado
        }

        // Abrir archivo de datos
        ifstream dataFile(dataFilename, ios::binary);
        if (!dataFile.is_open()) {
            return Matricula();
        }

        // Buscar posición
        dataFile.seekg(entry.offset);
        if (dataFile.fail()) {
            return Matricula();
        }

        // Leer bytes del registro
        vector<char> buffer(entry.size);
        if (!dataFile.read(buffer.data(), entry.size)) {
            return Matricula();
        }

        // Convertir a objeto Matricula
        string recordData(buffer.data(), buffer.size());
        return Matricula::unpack(recordData);
    }

    // Marcar un registro como eliminado
    void remove(int pos) {
        if (pos < 0 || pos >= static_cast<int>(metadata.size())) {
            return;
        }
        metadata[pos].active = false;
        saveMetadata();
    }
};
