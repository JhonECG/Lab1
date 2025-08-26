#include <iostream>
#include <cassert>
#include <filesystem>
#include "P2.cpp"

void testAdd() {
    // Crear archivo temporal para pruebas
    VariableRecordFile file("test_data.bin", "test_meta.bin");
    
    // Crear registros de prueba
    Matricula m1("A0001", 1, 1500.5f, "Estudiante regular");
    Matricula m2("B0002", 2, 2000.0f, "Beca parcial");
    
    // Probar añadir registros
    file.add(m1);
    file.add(m2);
    
    // Verificar que los registros se pueden leer correctamente
    Matricula r1 = file.readRecord(0);
    assert(r1.getCodigo() == "A0001");
    assert(r1.getCiclo() == 1);
    assert(r1.getMensualidad() == 1500.5f);
    assert(r1.getObservaciones() == "Estudiante regular");
    
    Matricula r2 = file.readRecord(1);
    assert(r2.getCodigo() == "B0002");
    assert(r2.getCiclo() == 2);
    assert(r2.getMensualidad() == 2000.0f);
    assert(r2.getObservaciones() == "Beca parcial");
    
    std::cout << "Test Add: PASSED\n";
}

void testRemove() {
    VariableRecordFile file("test_data.bin", "test_meta.bin");
    
    // Añadir registros de prueba
    Matricula m1("C0003", 3, 1800.0f, "Regular");
    Matricula m2("D0004", 4, 2200.0f, "Beca completa");
    file.add(m1);
    file.add(m2);
    
    // Eliminar el primer registro
    file.remove(0);
    
    // Cargar todos los registros activos
    vector<Matricula> records = file.load();
    
    // Verificar que solo queda un registro activo
    assert(records.size() == 1);
    assert(records[0].getCodigo() == "D0004");
    
    std::cout << "Test Remove: PASSED\n";
}

void testLoad() {
    VariableRecordFile file("test_data.bin", "test_meta.bin");
    
    // Añadir múltiples registros
    file.add(Matricula("E0005", 5, 1700.0f, "Observación 1"));
    file.add(Matricula("F0006", 6, 1900.0f, "Observación 2"));
    file.add(Matricula("G0007", 7, 2100.0f, "Observación 3"));
    
    // Eliminar uno de los registros
    file.remove(1);
    
    // Cargar todos los registros activos
    vector<Matricula> records = file.load();
    
    // Verificar que solo los registros activos son cargados
    assert(records.size() == 2);
    assert(records[0].getCodigo() == "E0005");
    assert(records[1].getCodigo() == "G0007");
    
    std::cout << "Test Load: PASSED\n";
}

void testReadRecord() {
    VariableRecordFile file("test_data.bin", "test_meta.bin");
    
    // Añadir un registro
    Matricula original("H0008", 8, 2300.0f, "Test de lectura");
    file.add(original);
    
    // Leer el registro añadido
    Matricula leido = file.readRecord(0);
    
    // Verificar que todos los campos coinciden
    assert(leido.getCodigo() == original.getCodigo());
    assert(leido.getCiclo() == original.getCiclo());
    assert(leido.getMensualidad() == original.getMensualidad());
    assert(leido.getObservaciones() == original.getObservaciones());
    
    // Probar lectura de posición inválida
    Matricula invalido = file.readRecord(999);
    assert(invalido.getCodigo().empty());
    
    std::cout << "Test ReadRecord: PASSED\n";
}

void cleanupTestFiles() {
    // Limpiar archivos de prueba
    std::filesystem::remove("test_data.bin");
    std::filesystem::remove("test_meta.bin");
}

int main() {
    std::cout << "Iniciando pruebas...\n";
    
    testAdd();
    testRemove();
    testLoad();
    testReadRecord();
    
    cleanupTestFiles();
    
    std::cout << "Todas las pruebas completadas exitosamente!\n";
    return 0;
}
