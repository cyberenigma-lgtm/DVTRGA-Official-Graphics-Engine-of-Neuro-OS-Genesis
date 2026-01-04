/*
 * DVTRGA API - Windows User-Mode Library
 * Direct Visual Transport & Render Graphics Architecture
 *
 * Biblioteca REAL en C para Windows
 * Compilar: cl /LD dvtrga_api.c /Fe:dvtrga_api.dll user32.lib gdi32.lib
 */

#include "dvtrga_security_bridge.h" // Private security module (gitignored)
#include <stdint.h>
#include <windows.h>

// Estructura del contexto DVTRGA
typedef struct {
  HWND hwnd;
  HDC hdc;
  HBITMAP hBitmap;
  void *pBits;
  uint32_t width;
  uint32_t height;
  uint32_t pitch;
  uint8_t bpp;
  uint8_t initialized;
} DVTRGA_CONTEXT;

// Contexto global
static DVTRGA_CONTEXT g_ctx = {0};

// Inicializar DVTRGA con Licencia Profesional
__declspec(dllexport) int DvtrgaInit(uint32_t width, uint32_t height,
                                     const char *license_key) {
  if (g_ctx.initialized)
    return 0;

  // --- VALIDACIÓN DE SEGURIDAD (ENTERPRISE) ---
  if (!Dvtrga_Security_Validate(license_key, NULL)) {
    OutputDebugStringA(
        "DVTRGA: FAILED TO INITIALIZE - INVALID OR EXPIRED LICENSE.");
    return -1; // Bloqueo de inicialización
  }
  // --------------------------------------------

  // Crear ventana invisible para obtener DC
  g_ctx.hwnd = GetDesktopWindow();
  g_ctx.hdc = GetDC(g_ctx.hwnd);

  if (!g_ctx.hdc)
    return -1;

  // Crear bitmap compatible
  BITMAPINFO bmi = {0};
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = -(int32_t)height; // Top-down
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;

  g_ctx.hBitmap =
      CreateDIBSection(g_ctx.hdc, &bmi, DIB_RGB_COLORS, &g_ctx.pBits, NULL, 0);

  if (!g_ctx.hBitmap || !g_ctx.pBits) {
    ReleaseDC(g_ctx.hwnd, g_ctx.hdc);
    return -1;
  }

  g_ctx.width = width;
  g_ctx.height = height;
  g_ctx.pitch = width * 4;
  g_ctx.bpp = 32;
  g_ctx.initialized = 1;

  return 0;
}

// Limpiar pantalla
__declspec(dllexport) void DvtrgaClear(uint32_t color) {
  if (!g_ctx.initialized || !g_ctx.pBits)
    return;

  uint32_t *pixels = (uint32_t *)g_ctx.pBits;
  uint32_t totalPixels = g_ctx.width * g_ctx.height;

  for (uint32_t i = 0; i < totalPixels; i++) {
    pixels[i] = color;
  }
}

// Dibujar píxel
__declspec(dllexport) void DvtrgaPutPixel(uint32_t x, uint32_t y,
                                          uint32_t color) {
  if (!g_ctx.initialized || !g_ctx.pBits)
    return;
  if (x >= g_ctx.width || y >= g_ctx.height)
    return;

  uint32_t *pixels = (uint32_t *)g_ctx.pBits;
  pixels[y * g_ctx.width + x] = color;
}

// Leer píxel
__declspec(dllexport) uint32_t DvtrgaGetPixel(uint32_t x, uint32_t y) {
  if (!g_ctx.initialized || !g_ctx.pBits)
    return 0;
  if (x >= g_ctx.width || y >= g_ctx.height)
    return 0;

  uint32_t *pixels = (uint32_t *)g_ctx.pBits;
  return pixels[y * g_ctx.width + x];
}

// Dibujar rectángulo lleno
__declspec(dllexport) void DvtrgaFillRect(uint32_t x, uint32_t y, uint32_t w,
                                          uint32_t h, uint32_t color) {
  if (!g_ctx.initialized)
    return;

  for (uint32_t dy = 0; dy < h; dy++) {
    for (uint32_t dx = 0; dx < w; dx++) {
      DvtrgaPutPixel(x + dx, y + dy, color);
    }
  }
}

// Dibujar línea (algoritmo de Bresenham)
__declspec(dllexport) void DvtrgaDrawLine(uint32_t x1, uint32_t y1, uint32_t x2,
                                          uint32_t y2, uint32_t color) {
  if (!g_ctx.initialized)
    return;

  int dx = abs((int)x2 - (int)x1);
  int dy = abs((int)y2 - (int)y1);
  int sx = x1 < x2 ? 1 : -1;
  int sy = y1 < y2 ? 1 : -1;
  int err = dx - dy;

  while (1) {
    DvtrgaPutPixel(x1, y1, color);

    if (x1 == x2 && y1 == y2)
      break;

    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }
}

// Presentar en pantalla
__declspec(dllexport) void DvtrgaPresent(void) {
  if (!g_ctx.initialized || !g_ctx.hBitmap)
    return;

  HDC hdcMem = CreateCompatibleDC(g_ctx.hdc);
  HBITMAP hOldBitmap = SelectObject(hdcMem, g_ctx.hBitmap);

  BitBlt(g_ctx.hdc, 0, 0, g_ctx.width, g_ctx.height, hdcMem, 0, 0, SRCCOPY);

  SelectObject(hdcMem, hOldBitmap);
  DeleteDC(hdcMem);
}

// Guardar a archivo BMP
__declspec(dllexport) int DvtrgaSaveBMP(const char *filename) {
  if (!g_ctx.initialized || !g_ctx.hBitmap)
    return -1;

  BITMAPFILEHEADER bfh = {0};
  BITMAPINFOHEADER bih = {0};

  bfh.bfType = 0x4D42; // "BM"
  bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
  bfh.bfSize = bfh.bfOffBits + (g_ctx.width * g_ctx.height * 4);

  bih.biSize = sizeof(BITMAPINFOHEADER);
  bih.biWidth = g_ctx.width;
  bih.biHeight = -(int32_t)g_ctx.height;
  bih.biPlanes = 1;
  bih.biBitCount = 32;
  bih.biCompression = BI_RGB;

  HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE)
    return -1;

  DWORD written;
  WriteFile(hFile, &bfh, sizeof(bfh), &written, NULL);
  WriteFile(hFile, &bih, sizeof(bih), &written, NULL);
  WriteFile(hFile, g_ctx.pBits, g_ctx.width * g_ctx.height * 4, &written, NULL);

  CloseHandle(hFile);
  return 0;
}

// Cerrar DVTRGA
__declspec(dllexport) void DvtrgaClose(void) {
  if (!g_ctx.initialized)
    return;

  if (g_ctx.hBitmap)
    DeleteObject(g_ctx.hBitmap);
  if (g_ctx.hdc)
    ReleaseDC(g_ctx.hwnd, g_ctx.hdc);

  memset(&g_ctx, 0, sizeof(g_ctx));
}

// Obtener información
__declspec(dllexport) void DvtrgaGetInfo(uint32_t *width, uint32_t *height,
                                         uint32_t *bpp) {
  if (width)
    *width = g_ctx.width;
  if (height)
    *height = g_ctx.height;
  if (bpp)
    *bpp = g_ctx.bpp;
}
