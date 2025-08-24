#include <iostream>
#include <fstream>
#include <vector>
#include <string>
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
    // lo puse mejor con const string& por mejor practica de programación (perdom me dio amsiedad)

    string pack() const {
        string data;

        int len_codigo = codigo.size();
        int len_obs = observaciones.size();

        data.append(reinterpret_cast<const char*>(&len_codigo), sizeof(int));
        data.append(codigo);
        data.append(reinterpret_cast<const char*>(&ciclo), sizeof(int));
        data.append(reinterpret_cast<const char*>(&mensualidad), sizeof(float));
        data.append(reinterpret_cast<const char*>(&len_obs), sizeof(int));
        data.append(observaciones);

        return data;
    }

    static Matricula unpack(const string& data) {
        Matricula m;
        size_t pos = 0;
        int len_codigo = 0;
        memcpy(&len_codigo, data.data() + pos, sizeof(int));
        pos += sizeof(int);
        m.codigo = data.substr(pos, len_codigo);
        pos += len_codigo;
        memcpy(&m.ciclo, data.data() + pos, sizeof(int));
        pos += sizeof(int);
        memcpy(&m.mensualidad, data.data() + pos, sizeof(float));
        pos += sizeof(float);
        int len_obs = 0;
        memcpy(&len_obs, data.data() + pos, sizeof(int));
        pos += sizeof(int);
        m.observaciones = data.substr(pos, len_obs);
        return m;
    }
};

/*jaja ayuda, estoy quemando*/

class VariableRecordFile {
private:
    string dataFilename;
    string metaFilename;
    vector<MetadataEntry> metadata;

    void loadMetadata() {
        // TODO: leer metadata desde archivo
        metadata.clear();
        ifstream metaFile(metaFilename, ios::binary);
        MetadataEntry entry;
        while (metaFile.read(reinterpret_cast<char*>(&entry), sizeof(MetadataEntry))) {
            metadata.push_back(entry);
        }
        // aca si es copilot, exepto por la limpieza, eso si le añadi por si las moscas de la mem xd
    }

    void saveMetadata() {
        // TODO: guardar metadata en archivo
        ofstream metaFile(metaFilename, ios::binary | ios::trunc);
        for (const auto& entry : metadata) {
            metaFile.write(reinterpret_cast<const char*>(&entry), sizeof(MetadataEntry));
            // nose esto capaz esta mal me lo dio por defecto el copilot xd
        }
    }

/* este si lo hice yo
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
*/

public:
    VariableRecordFile(const string& dataFile, const string& metaFile) {
        dataFilename = dataFile;
        metaFilename = metaFile;
        loadMetadata();
    }

    vector<Matricula> load() {
        // TODO: devolver todos los registros activos
        return {};
    }

    void add(const Matricula& record) {
        // TODO: agregar un registro O(1)

    }

    Matricula readRecord(int pos) {
        // TODO: leer registro en posición pos
        return Matricula();
    }

    void remove(int pos) {
        // TODO: eliminar registro en posición pos
        if (pos < 0 || pos >= static_cast<int>(metadata.size())) {
            return;
        }
        metadata[pos].active = false;
        saveMetadata();
    }
};

int main() {
    VariableRecordFile file("datos.bin", "metadata.bin");

    // TODO: pruebas funcionales

    return 0;
}
