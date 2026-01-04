#!/bin/bash
# Compilar DVTRGA para Linux, Windows y Mac

echo "========================================"
echo "DVTRGA - Compilador Multiplataforma"
echo "Copyright (C) 2025 José Manuel Moreno Cano"
echo "========================================"
echo ""

# Detectar sistema
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    PLATFORM="Linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="Mac"
elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
    PLATFORM="Windows"
else
    PLATFORM="Unknown"
fi

echo "Plataforma detectada: $PLATFORM"
echo ""

# Compilar según plataforma
case $PLATFORM in
    "Linux")
        echo "[1/2] Compilando biblioteca..."
        gcc -o libdvtrga.so -shared -fPIC dvtrga_linux.c -lX11
        
        echo "[2/2] Compilando aplicación..."
        gcc -o dvtrga_test dvtrga_test_linux.c -L. -ldvtrga -lX11
        
        echo ""
        echo "Archivos generados:"
        ls -lh libdvtrga.so dvtrga_test
        echo ""
        echo "Para ejecutar: ./dvtrga_test"
        ;;
        
    "Mac")
        echo "[1/2] Compilando biblioteca..."
        gcc -o libdvtrga.dylib -shared -fPIC dvtrga_mac.c -framework Cocoa
        
        echo "[2/2] Compilando aplicación..."
        gcc -o dvtrga_test dvtrga_test_mac.c -L. -ldvtrga -framework Cocoa
        
        echo ""
        echo "Archivos generados:"
        ls -lh libdvtrga.dylib dvtrga_test
        echo ""
        echo "Para ejecutar: ./dvtrga_test"
        ;;
        
    "Windows")
        echo "[1/2] Compilando DLL..."
        gcc -o dvtrga.dll -shared dvtrga_windows.c -lgdi32 -luser32
        
        echo "[2/2] Compilando aplicación..."
        gcc -o dvtrga_test.exe dvtrga_test_windows.c -L. -ldvtrga -lgdi32 -luser32
        
        echo ""
        echo "Archivos generados:"
        ls -lh dvtrga.dll dvtrga_test.exe
        echo ""
        echo "Para ejecutar: dvtrga_test.exe"
        ;;
        
    *)
        echo "ERROR: Plataforma no soportada"
        exit 1
        ;;
esac

echo ""
echo "========================================"
echo "Compilación completada"
echo "========================================"
