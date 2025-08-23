#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct Record {
    string codigo;
    int ciclo;
    float mensualidad;
    string observaciones;

    string pack() const {
        // TODO: serializar campos a string binario
        return "";
    }

    static Record unpack(const string& data) {
        // TODO: deserializar desde string binario
        return Record();
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

    vector<Record> load() {
        // TODO: devolver todos los registros activos
        return {};
    }

    void add(const Record& record) {
        // TODO: agregar un registro O(1)
    }

    Record readRecord(int pos) {
        // TODO: leer registro en posición pos
        return Record();
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
