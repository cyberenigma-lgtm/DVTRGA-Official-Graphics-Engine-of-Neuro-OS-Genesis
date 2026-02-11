"""
DVTRGA API Simulada para Windows
Biblioteca user-mode que simula el driver
"""

import ctypes
from ctypes import wintypes
import sys

class DVTRGA_CONTEXT(ctypes.Structure):
    _fields_ = [
        ("DeviceHandle", wintypes.HANDLE),
        ("Width", wintypes.DWORD),
        ("Height", wintypes.DWORD),
        ("Pitch", wintypes.DWORD),
        ("Bpp", wintypes.DWORD),
    ]

class DVTRGASimulator:
    """Simulador de DVTRGA para pruebas en Windows"""
    
    def __init__(self):
        self.context = DVTRGA_CONTEXT()
        self.context.Width = 1024
        self.context.Height = 768
        self.context.Pitch = 1024 * 4
        self.context.Bpp = 32
        self.initialized = False
    
    def init(self):
        """Inicializar DVTRGA (simulado)"""
        print("[DVTRGA] Inicializando...")
        print(f"[DVTRGA] Resolución: {self.context.Width}x{self.context.Height}")
        print(f"[DVTRGA] Profundidad: {self.context.Bpp} bits")
        self.initialized = True
        return True
    
    def clear(self, color):
        """Limpiar pantalla (simulado)"""
        if not self.initialized:
            return False
        print(f"[DVTRGA] Limpiando pantalla con color 0x{color:06X}")
        return True
    
    def put_pixel(self, x, y, color):
        """Dibujar píxel (simulado)"""
        if not self.initialized:
            return False
        if x >= self.context.Width or y >= self.context.Height:
            return False
        # Simulado - no hace nada real
        return True
    
    def fill_rect(self, x, y, w, h, color):
        """Dibujar rectángulo (simulado)"""
        if not self.initialized:
            return False
        print(f"[DVTRGA] Rectángulo en ({x},{y}) tamaño {w}x{h} color 0x{color:06X}")
        return True
    
    def draw_text(self, x, y, text, color):
        """Dibujar texto (simulado)"""
        if not self.initialized:
            return False
        print(f"[DVTRGA] Texto en ({x},{y}): '{text}' color 0x{color:06X}")
        return True
    
    def close(self):
        """Cerrar DVTRGA"""
        print("[DVTRGA] Cerrando...")
        self.initialized = False

# API compatible con C
dvtrga_instance = None

def DvtrgaInit():
    """Inicializar DVTRGA"""
    global dvtrga_instance
    dvtrga_instance = DVTRGASimulator()
    return dvtrga_instance.init()

def DvtrgaClear(color):
    """Limpiar pantalla"""
    if dvtrga_instance:
        return dvtrga_instance.clear(color)
    return False

def DvtrgaPutPixel(x, y, color):
    """Dibujar píxel"""
    if dvtrga_instance:
        return dvtrga_instance.put_pixel(x, y, color)
    return False

def DvtrgaFillRect(x, y, w, h, color):
    """Dibujar rectángulo"""
    if dvtrga_instance:
        return dvtrga_instance.fill_rect(x, y, w, h, color)
    return False

def DvtrgaDrawText(x, y, text, color):
    """Dibujar texto"""
    if dvtrga_instance:
        return dvtrga_instance.draw_text(x, y, text, color)
    return False

def DvtrgaClose():
    """Cerrar DVTRGA"""
    global dvtrga_instance
    if dvtrga_instance:
        dvtrga_instance.close()
        dvtrga_instance = None

if __name__ == "__main__":
    print("DVTRGA API Simulator - Test")
    print("=" * 50)
    
    if DvtrgaInit():
        print("\n✅ Inicialización exitosa\n")
        
        DvtrgaClear(0x000000)
        DvtrgaFillRect(10, 10, 100, 50, 0xFF0000)
        DvtrgaFillRect(10, 70, 100, 50, 0x00FF00)
        DvtrgaDrawText(10, 130, "DVTRGA Funcionando", 0xFFFFFF)
        
        print("\n✅ Prueba completada")
        DvtrgaClose()
    else:
        print("\n❌ Error de inicialización")
