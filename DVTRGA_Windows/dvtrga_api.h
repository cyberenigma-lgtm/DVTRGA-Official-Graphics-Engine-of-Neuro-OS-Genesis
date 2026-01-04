/*
 * DVTRGA API Header
 * Direct Visual Transport & Render Graphics Architecture
 *
 * Copyright (C) 2025 José Manuel Moreno Cano
 * Todos los derechos reservados. All rights reserved.
 *
 * Autor: José Manuel Moreno Cano
 * Email: admin@neuro-os.es
 * Web: https://neuro-os.es
 */

#ifndef DVTRGA_API_H
#define DVTRGA_API_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Funciones exportadas
__declspec(dllimport) int DvtrgaInit(uint32_t width, uint32_t height,
                                     const char *license_key);
__declspec(dllimport) void DvtrgaClear(uint32_t color);
__declspec(dllimport) void DvtrgaPutPixel(uint32_t x, uint32_t y,
                                          uint32_t color);
__declspec(dllimport) uint32_t DvtrgaGetPixel(uint32_t x, uint32_t y);
__declspec(dllimport) void DvtrgaFillRect(uint32_t x, uint32_t y, uint32_t w,
                                          uint32_t h, uint32_t color);
__declspec(dllimport) void DvtrgaDrawLine(uint32_t x1, uint32_t y1, uint32_t x2,
                                          uint32_t y2, uint32_t color);
__declspec(dllimport) void DvtrgaPresent(void);
__declspec(dllimport) int DvtrgaSaveBMP(const char *filename);
__declspec(dllimport) void DvtrgaClose(void);
__declspec(dllimport) void DvtrgaGetInfo(uint32_t *width, uint32_t *height,
                                         uint32_t *bpp);

// Colores predefinidos
#define DVTRGA_COLOR_BLACK 0x00000000
#define DVTRGA_COLOR_BLUE 0x000000FF
#define DVTRGA_COLOR_GREEN 0x0000FF00
#define DVTRGA_COLOR_CYAN 0x0000FFFF
#define DVTRGA_COLOR_RED 0x00FF0000
#define DVTRGA_COLOR_MAGENTA 0x00FF00FF
#define DVTRGA_COLOR_YELLOW 0x00FFFF00
#define DVTRGA_COLOR_WHITE 0x00FFFFFF
#define DVTRGA_COLOR_GRAY 0x00808080

#ifdef __cplusplus
}
#endif

#endif // DVTRGA_API_H
