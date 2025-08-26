#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

struct MetadataEntry {
    long offset;
    long size;
    bool active;
};

class Matricula {
    string codigo;
    int ciclo;
    float mensualidad;
    string observaciones;

public:
    Matricula() = default;
    Matricula(const string& c, int ci, float m, const string& o)
        : codigo(c), ciclo(ci), mensualidad(m), observaciones(o) {}

    const string& getCodigo() const {return codigo;}
    int getCiclo() const {return ciclo;}
    float getMensualidad() const {return mensualidad;}
    const string& getObservaciones() const {return observaciones;}

    string pack() const {
        string data;
        size_t len_codigo = codigo.length();
        size_t len_obs = observaciones.length();

        data.resize(sizeof(size_t) + len_codigo + sizeof(int) + sizeof(float) + sizeof(size_t) + len_obs);
        char* ptr = &data[0];
        memcpy(ptr, &len_codigo, sizeof(size_t));
        ptr += sizeof(size_t);
        memcpy(ptr, codigo.c_str(), len_codigo);
        ptr += len_codigo;
        memcpy(ptr, &ciclo, sizeof(int));
        ptr += sizeof(int);
        memcpy(ptr, &mensualidad, sizeof(float));
        ptr += sizeof(float);
        memcpy(ptr, &len_obs, sizeof(size_t));
        ptr += sizeof(size_t);
        memcpy(ptr, observaciones.c_str(), len_obs);
        return data;
    }

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

class VariableRecordFile {
private:
    string dataFilename;
    string metaFilename;
    vector<MetadataEntry> metadata;

    void loadMetadata() {
        metadata.clear();
        ifstream metaFile(metaFilename, ios::binary);
        if (!metaFile.is_open()) {
            return;
        }
        
        MetadataEntry entry;
        while (metaFile.read(reinterpret_cast<char *>(&entry.offset), sizeof(long)) &&
               metaFile.read(reinterpret_cast<char *>(&entry.size), sizeof(long)) &&
               metaFile.read(reinterpret_cast<char *>(&entry.active), sizeof(bool))) {
            metadata.push_back(entry);
        }
    }

    void saveMetadata() {
        ofstream metaFile(metaFilename, ios::binary | ios::trunc);
        if (!metaFile.is_open()) {
            return;
        }

        for (const MetadataEntry &entry : metadata) {
            metaFile.write(reinterpret_cast<const char *>(&entry.offset), sizeof(long));
            metaFile.write(reinterpret_cast<const char *>(&entry.size), sizeof(long));
            metaFile.write(reinterpret_cast<const char *>(&entry.active), sizeof(bool));
        }
    }

public:
    VariableRecordFile(const string &dataFile, const string &metaFile)
        : dataFilename(dataFile), metaFilename(metaFile) {
        ofstream dataCreate(dataFilename, ios::binary | ios::trunc);
        dataCreate.close();        
        ofstream metaCreate(metaFilename, ios::binary | ios::trunc);
        metaCreate.close();
        metadata.clear();
    }

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

    void add(const Matricula &record) {
        string packedData = record.pack();
        
        // Abrir el archivo
        ofstream dataFile(dataFilename, ios::binary | ios::app);
        if (!dataFile.is_open()) {
            return;
        }
        
        // Obtiene la pos final
        long offset = dataFile.tellp();
        
        // Escribir los datos
        if (!dataFile.write(packedData.data(), packedData.size())) {
            return;
        }
        dataFile.close();

        // Entrada de metadatos
        MetadataEntry entry;
        entry.offset = offset;
        entry.size = packedData.size();
        entry.active = true;
        metadata.push_back(entry);
        
        // Guardar metadatos
        saveMetadata();
    }

    Matricula readRecord(int pos)
    {
        if (pos < 0 || pos >= static_cast<int>(metadata.size())) {
            return Matricula();
        }

        // Obtener la entrada de metadatos
        const MetadataEntry &entry = metadata[pos];
        
        if (!entry.active) {
            return Matricula();
        }

        // Abrir el archivo
        ifstream dataFile(dataFilename, ios::binary);
        if (!dataFile.is_open()) {
            return Matricula();
        }

        // Buscar la pos del registro
        dataFile.seekg(entry.offset);
        if (dataFile.fail()) {
            return Matricula();
        }

        // Leer los datos del registro
        vector<char> buffer(entry.size);
        if (!dataFile.read(buffer.data(), entry.size))
        {
            return Matricula();
        }

        // Convertir a string
        string recordData(buffer.data(), buffer.size());
        return Matricula::unpack(recordData);
    }

    void remove(int pos)
    {
        // TODO: eliminar registro en posición pos
        if (pos < 0 || pos >= static_cast<int>(metadata.size()))
        {
            return;
        }
        metadata[pos].active = false;
        saveMetadata();
    }
};
