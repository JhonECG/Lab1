#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class Matricula {
    string codigo;
    int ciclo;
    float mensualidad;
    string observaciones;

public:
    void setCodigo(const string& c) {
        codigo = c;
    }
    void setCiclo(int c) {
        ciclo = c;
    }
    void setMensualidad(float m) {
        mensualidad = m;
    }
    void setObservaciones(const string& o) {
        observaciones = o;
    }
    string getCodigo() const {
        return codigo;
    }
    int getCiclo() const {
        return ciclo;
    }
    float getMensualidad() const {
        return mensualidad;
    }
    string getObservaciones() const {
        return observaciones;
    }

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

struct MetadataEntry {
    long offset;
    long size;
    bool active;
};

class VariableRecordFile {
private:
    string dataFilename;
    string metaFilename;
    vector<MetadataEntry> metadata;

    void loadMetadata() {
        // TODO: leer metadata desde archivo
    }

    void saveMetadata() {
        // TODO: guardar metadata en archivo
    }

public:
    VariableRecordFile(const string& dataFile, const string& metaFile) {
        dataFilename = dataFile;
        metaFilename = metaFile;
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
    }
};

int main() {
    VariableRecordFile file("datos.bin", "metadata.bin");

    // TODO: pruebas funcionales

    return 0;
}
