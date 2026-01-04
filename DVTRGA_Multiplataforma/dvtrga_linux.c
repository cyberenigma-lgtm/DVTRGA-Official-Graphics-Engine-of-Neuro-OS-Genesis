/*
 * DVTRGA API - Linux Version (X11)
 * Direct Visual Transport & Render Graphics Architecture
 *
 * Copyright (C) 2025 José Manuel Moreno Cano
 * Todos los derechos reservados. All rights reserved.
 *
 * Autor: José Manuel Moreno Cano
 * Email: admin@neuro-os.es
 * Web: https://neuro-os.es
 *
 * Compilar: gcc -o libdvtrga.so -shared -fPIC dvtrga_linux.c -lX11
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Contexto DVTRGA
typedef struct {
  Display *display;
  Window window;
  GC gc;
  XImage *image;
  uint32_t *framebuffer;
  uint32_t width;
  uint32_t height;
  uint8_t initialized;
} DVTRGA_CONTEXT;

// Contexto global
static DVTRGA_CONTEXT g_ctx = {0};

// Inicializar DVTRGA
int DvtrgaInit(uint32_t width, uint32_t height) {
  if (g_ctx.initialized)
    return 0;

  // Abrir display
  g_ctx.display = XOpenDisplay(NULL);
  if (!g_ctx.display)
    return -1;

  int screen = DefaultScreen(g_ctx.display);

  // Crear ventana
  g_ctx.window = XCreateSimpleWindow(
      g_ctx.display, RootWindow(g_ctx.display, screen), 0, 0, width, height, 1,
      BlackPixel(g_ctx.display, screen), WhitePixel(g_ctx.display, screen));

  XStoreName(g_ctx.display, g_ctx.window, "DVTRGA - Linux");
  XSelectInput(g_ctx.display, g_ctx.window, ExposureMask | KeyPressMask);
  XMapWindow(g_ctx.display, g_ctx.window);

  // Crear GC
  g_ctx.gc = XCreateGC(g_ctx.display, g_ctx.window, 0, NULL);

  // Crear framebuffer
  g_ctx.framebuffer = (uint32_t *)malloc(width * height * sizeof(uint32_t));
  if (!g_ctx.framebuffer) {
    XCloseDisplay(g_ctx.display);
    return -1;
  }

  // Crear XImage
  g_ctx.image =
      XCreateImage(g_ctx.display, DefaultVisual(g_ctx.display, screen),
                   DefaultDepth(g_ctx.display, screen), ZPixmap, 0,
                   (char *)g_ctx.framebuffer, width, height, 32, 0);

  g_ctx.width = width;
  g_ctx.height = height;
  g_ctx.initialized = 1;

  return 0;
}

// Limpiar pantalla
void DvtrgaClear(uint32_t color) {
  if (!g_ctx.initialized)
    return;

  for (uint32_t i = 0; i < g_ctx.width * g_ctx.height; i++) {
    g_ctx.framebuffer[i] = color;
  }
}

// Dibujar píxel
void DvtrgaPutPixel(uint32_t x, uint32_t y, uint32_t color) {
  if (!g_ctx.initialized)
    return;
  if (x >= g_ctx.width || y >= g_ctx.height)
    return;

  g_ctx.framebuffer[y * g_ctx.width + x] = color;
}

// Leer píxel
uint32_t DvtrgaGetPixel(uint32_t x, uint32_t y) {
  if (!g_ctx.initialized)
    return 0;
  if (x >= g_ctx.width || y >= g_ctx.height)
    return 0;

  return g_ctx.framebuffer[y * g_ctx.width + x];
}

// Dibujar rectángulo lleno
void DvtrgaFillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                    uint32_t color) {
  if (!g_ctx.initialized)
    return;

  for (uint32_t dy = 0; dy < h; dy++) {
    for (uint32_t dx = 0; dx < w; dx++) {
      DvtrgaPutPixel(x + dx, y + dy, color);
    }
  }
}

// Dibujar línea
void DvtrgaDrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2,
                    uint32_t color) {
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
void DvtrgaPresent(void) {
  if (!g_ctx.initialized)
    return;

  XPutImage(g_ctx.display, g_ctx.window, g_ctx.gc, g_ctx.image, 0, 0, 0, 0,
            g_ctx.width, g_ctx.height);
  XFlush(g_ctx.display);
}

// Cerrar DVTRGA
void DvtrgaClose(void) {
  if (!g_ctx.initialized)
    return;

  if (g_ctx.image)
    XDestroyImage(g_ctx.image);
  if (g_ctx.gc)
    XFreeGC(g_ctx.display, g_ctx.gc);
  if (g_ctx.window)
    XDestroyWindow(g_ctx.display, g_ctx.window);
  if (g_ctx.display)
    XCloseDisplay(g_ctx.display);

  memset(&g_ctx, 0, sizeof(g_ctx));
}

// Obtener información
void DvtrgaGetInfo(uint32_t *width, uint32_t *height, uint32_t *bpp) {
  if (width)
    *width = g_ctx.width;
  if (height)
    *height = g_ctx.height;
  if (bpp)
    *bpp = 32;
}
