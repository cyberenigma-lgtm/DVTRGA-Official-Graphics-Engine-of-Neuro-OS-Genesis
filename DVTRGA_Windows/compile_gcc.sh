#!/bin/bash
# Compilar DVTRGA usando GCC (MinGW para Windows)

echo "========================================"
echo "DVTRGA - Compilador con GCC"
echo "========================================"
echo ""

# Compilar DLL
echo "[1/3] Compilando DLL (dvtrga_api.dll)..."
gcc -shared -o dvtrga_api.dll dvtrga_api.c -lgdi32 -luser32 -O2

if [ $? -eq 0 ]; then
    echo "[OK] DLL compilada"
else
    echo "[ERROR] Fallo compilando DLL"
    exit 1
fi

echo ""

# Compilar aplicación
echo "[2/3] Compilando aplicación (dvtrga_test.exe)..."
gcc -o dvtrga_test.exe dvtrga_test.c -L. -ldvtrga_api -lgdi32 -luser32 -O2

if [ $? -eq 0 ]; then
    echo "[OK] Aplicación compilada"
else
    echo "[ERROR] Fallo compilando aplicación"
    exit 1
fi

echo ""

# Verificar archivos
echo "[3/3] Verificando archivos generados..."
if [ -f "dvtrga_api.dll" ]; then
    echo "[OK] dvtrga_api.dll"
    ls -lh dvtrga_api.dll
fi

if [ -f "dvtrga_test.exe" ]; then
    echo "[OK] dvtrga_test.exe"
    ls -lh dvtrga_test.exe
fi

echo ""
echo "========================================"
echo "Compilación completada"
echo "========================================"
echo ""
echo "Para probar:"
echo "  ./dvtrga_test.exe"
echo ""
