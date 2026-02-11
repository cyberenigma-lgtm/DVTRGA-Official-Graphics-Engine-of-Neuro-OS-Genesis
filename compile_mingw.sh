#!/bin/bash
# Compilar DVTRGA usando MinGW cross-compiler

echo "========================================"
echo "DVTRGA - Compilador MinGW"
echo "========================================"
echo ""

# Compilar DLL
echo "[1/3] Compilando DLL (dvtrga_api.dll)..."
x86_64-w64-mingw32-gcc -shared -o dvtrga_api.dll dvtrga_api.c \
    -lgdi32 -luser32 -O2 -Wall

if [ $? -eq 0 ]; then
    echo "[OK] DLL compilada"
else
    echo "[ERROR] Fallo compilando DLL"
    exit 1
fi

echo ""

# Compilar aplicación
echo "[2/3] Compilando aplicación (dvtrga_test.exe)..."
x86_64-w64-mingw32-gcc -o dvtrga_test.exe dvtrga_test.c \
    -L. -ldvtrga_api -lgdi32 -luser32 -O2 -Wall

if [ $? -eq 0 ]; then
    echo "[OK] Aplicación compilada"
else
    echo "[ERROR] Fallo compilando aplicación"
    exit 1
fi

echo ""

# Verificar archivos
echo "[3/3] Archivos generados:"
ls -lh dvtrga_api.dll dvtrga_test.exe 2>/dev/null

echo ""
echo "========================================"
echo "Compilación completada exitosamente"
echo "========================================"
echo ""
echo "Archivos:"
echo "  - dvtrga_api.dll (biblioteca)"
echo "  - dvtrga_test.exe (aplicación)"
echo ""
echo "Para probar en Windows:"
echo "  dvtrga_test.exe"
echo ""
