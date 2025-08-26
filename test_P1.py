import os
from P1 import FixedRecord_FreeList, FixedRecord_MoveTheLast, Record

def limpiar_archivo(filename):
    if os.path.exists(filename):
        os.remove(filename)
        print(f"\nArchivo {filename} eliminado correctamente.")

def test_freelist():
    print("\n=== TEST FREELIST ===")
    # Crear una instancia de FixedRecord_FreeList
    archivo = FixedRecord_FreeList("datos_freelist.bin")
    
    # 1. Crear y agregar registros iniciales
    print("1. Insertando registros iniciales...")
    registros = [
        Record("CS101", "Juan", "Perez Garcia", "Computacion", 5, 1500.50),
        Record("IS202", "Maria", "Lopez Torres", "Sistemas", 3, 1200.75),
        Record("CS303", "Carlos", "Rodriguez Ruiz", "Computacion", 7, 1350.25),
        Record("IS404", "Ana", "Martinez Silva", "Sistemas", 4, 1400.00)
    ]
    
    for registro in registros:
        archivo.add(registro)
        print(f"Agregado: {registro.codigo} - {registro.nombre} {registro.apellidos}")

    # 2. Mostrar todos los registros
    print("\n2. Registros actuales en el archivo:")
    registros_cargados = archivo.load()
    for i, reg in enumerate(registros_cargados):
        print(f"Posición {i}: {reg.codigo} - {reg.nombre} {reg.apellidos} - {reg.carrera}")

    # 3. Eliminar registros
    print("\n3. Eliminando registros en posiciones 1 y 2...")
    archivo.remover(1)
    archivo.remover(2)
    
    # 4. Mostrar registros después de eliminar
    print("\n4. Registros después de eliminar:")
    registros_cargados = archivo.load()
    for i, reg in enumerate(registros_cargados):
        print(f"Posición {i}: {reg.codigo} - {reg.nombre} {reg.apellidos} - {reg.carrera}")

    # 5. Agregar nuevos registros (deberían usar las posiciones libres)
    print("\n5. Agregando nuevos registros...")
    nuevos_registros = [
        Record("AA505", "Pedro", "Sanchez Mora", "Economia", 6, 1600.00),
        Record("BB606", "Laura", "Diaz Paz", "Marketing", 5, 1450.75)
    ]
    
    for registro in nuevos_registros:
        archivo.add(registro)
        print(f"Agregado: {registro.codigo} - {registro.nombre} {registro.apellidos}")

    # 6. Mostrar estado final del archivo
    print("\n6. Estado final del archivo:")
    registros_finales = archivo.load()
    for i, reg in enumerate(registros_finales):
        print(f"Posición {i}: {reg.codigo} - {reg.nombre} {reg.apellidos} - {reg.carrera}")

def test_movethelast():
    print("\n=== TEST MOVE THE LAST ===")
    # Crear instancia y archivo de prueba
    archivo = FixedRecord_MoveTheLast("datos_movethelast.bin")
    
    # 1. Prueba de inserción inicial
    print("1. Insertando registros iniciales...")
    registros = [
        Record("CS101", "Juan", "Perez Garcia", "Computacion", 5, 1500.50),    # pos 0
        Record("IS202", "Maria", "Lopez Torres", "Sistemas", 3, 1200.75),      # pos 1
        Record("CS303", "Carlos", "Rodriguez Ruiz", "Computacion", 7, 1350.25),# pos 2
        Record("IS404", "Ana", "Martinez Silva", "Sistemas", 4, 1400.00)       # pos 3
    ]
    
    for registro in registros:
        archivo.add(registro)
        print(f"Agregado: {registro.codigo} - {registro.nombre} {registro.apellidos}")

    # 2. Verificar registros iniciales
    print("\n2. Estado inicial del archivo:")
    for i, reg in enumerate(archivo.load()):
        print(f"Posición {i}: {reg.codigo} - {reg.nombre} {reg.apellidos}")

    # 3. Probar eliminación de un registro del medio
    print("\n3. Eliminando registro en posición 1...")
    archivo.remover(1)  # Debería mover el último registro (Ana) a la posición 1
    
    print("\nEstado después de eliminar posición 1:")
    for i, reg in enumerate(archivo.load()):
        print(f"Posición {i}: {reg.codigo} - {reg.nombre} {reg.apellidos}")
        
    # 4. Probar eliminación del último registro
    print("\n4. Eliminando último registro...")
    ultimo_indice = len(archivo.load()) - 1
    archivo.remover(ultimo_indice)
    
    print("\nEstado después de eliminar último registro:")
    for i, reg in enumerate(archivo.load()):
        print(f"Posición {i}: {reg.codigo} - {reg.nombre} {reg.apellidos}")
    
    # 5. Agregar nuevo registro después de eliminaciones
    print("\n5. Agregando nuevo registro...")
    nuevo_registro = Record("EC505", "Pedro", "Sanchez Mora", "Economia", 6, 1600.00)
    archivo.add(nuevo_registro)
    
    print("\nEstado final del archivo:")
    for i, reg in enumerate(archivo.load()):
        print(f"Posición {i}: {reg.codigo} - {reg.nombre} {reg.apellidos}")

if __name__ == "__main__":
    # Limpiar archivos anteriores si existen
    limpiar_archivo("datos_freelist.bin")
    limpiar_archivo("datos_movethelast.bin")
    
    # Ejecutar pruebas
    test_freelist()
    test_movethelast()
    
    # Limpiar archivos al finalizar
    limpiar_archivo("datos_freelist.bin")
    limpiar_archivo("datos_movethelast.bin")
