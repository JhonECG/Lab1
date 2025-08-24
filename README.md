# Lab1

## P2 (10 pts)

Dada la siguiente estructura del registro de longitud variable:

**Registro Matricula:**
- `codigo` (Cadena*)        → Tamaño variable
- `ciclo` (Entero)          → Entero para el ciclo
- `mensualidad` (Decimal)   → Decimal para la mensualidad
- `observaciones` (Cadena*) → Tamaño variable

### Requisitos:
Se le pide implementar un programa para leer y escribir registros de longitud variable en un archivo binario usando el tamaño del dato como separador:

1. Manejar un archivo adicional (metadata) para indicar la posición inicial de cada registro.  
   Evaluar si es necesario guardar también el tamaño del registro.

2. Implementar las siguientes funciones:
    - `load()` : devuelve todos los registros del archivo.
    - `add(record)` : agrega un nuevo registro al archivo **O(1)**.
    - `readRecord(pos)` : obtiene el registro de la posición `"pos"` **O(1)**.
    - `remove(pos)` : elimina el registro de la posición `"pos"` (proponer una estrategia eficiente).

3. Realizar las pruebas funcionales de cada método (**P2.cpp**).