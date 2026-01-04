"""
DVTRGA Test Application
Aplicación de prueba para DVTRGA en Windows
"""

import sys
import os

# Añadir path actual
sys.path.insert(0, os.path.dirname(__file__))

from dvtrga_api import *

def main():
    print("=" * 60)
    print("  DVTRGA Test Application v1.0")
    print("  Direct Visual Transport & Render Graphics Architecture")
    print("=" * 60)
    print()
    
    # Inicializar
    print("[1/5] Inicializando DVTRGA...")
    if not DvtrgaInit():
        print("❌ Error: No se pudo inicializar DVTRGA")
        return 1
    
    print("✅ DVTRGA inicializado correctamente")
    print()
    
    # Limpiar pantalla
    print("[2/5] Limpiando pantalla...")
    DvtrgaClear(0x001020)  # Azul oscuro
    print("✅ Pantalla limpiada")
    print()
    
    # Dibujar rectángulos de prueba
    print("[3/5] Dibujando rectángulos de prueba...")
    DvtrgaFillRect(50, 50, 200, 100, 0xFF0000)    # Rojo
    DvtrgaFillRect(50, 160, 200, 100, 0x00FF00)   # Verde
    DvtrgaFillRect(50, 270, 200, 100, 0x0000FF)   # Azul
    print("✅ Rectángulos dibujados")
    print()
    
    # Dibujar texto
    print("[4/5] Dibujando texto...")
    DvtrgaDrawText(300, 50, "DVTRGA Graphics Driver", 0xFFFFFF)
    DvtrgaDrawText(300, 70, "Version 1.0", 0xCCCCCC)
    DvtrgaDrawText(300, 100, "Desarrollado por:", 0xAAAA AA)
    DvtrgaDrawText(300, 120, "Jose Manuel Moreno Cano", 0xFFFFFF)
    DvtrgaDrawText(300, 150, "Estado: FUNCIONANDO", 0x00FF00)
    print("✅ Texto dibujado")
    print()
    
    # Dibujar píxeles individuales
    print("[5/5] Dibujando píxeles...")
    for i in range(100):
        DvtrgaPutPixel(300 + i, 200, 0xFFFFFF)
        DvtrgaPutPixel(300 + i, 201, 0xFFFFFF)
    print("✅ Píxeles dibujados")
    print()
    
    # Finalizar
    print("=" * 60)
    print("  ✅ Prueba completada exitosamente")
    print("=" * 60)
    print()
    print("NOTA: Esta es una simulación del driver DVTRGA.")
    print("En un sistema real, esto dibujaría en la pantalla.")
    print()
    
    DvtrgaClose()
    return 0

if __name__ == "__main__":
    sys.exit(main())
