/*
 * DVTRGA Test - Linux
 *
 * Copyright (C) 2025 José Manuel Moreno Cano
 *
 * Compilar: gcc -o dvtrga_test dvtrga_test_linux.c -L. -ldvtrga -lX11
 */

#include <stdio.h>
#include <unistd.h>

// Declaraciones
int DvtrgaInit(unsigned int width, unsigned int height);
void DvtrgaClear(unsigned int color);
void DvtrgaPutPixel(unsigned int x, unsigned int y, unsigned int color);
void DvtrgaFillRect(unsigned int x, unsigned int y, unsigned int w,
                    unsigned int h, unsigned int color);
void DvtrgaDrawLine(unsigned int x1, unsigned int y1, unsigned int x2,
                    unsigned int y2, unsigned int color);
void DvtrgaPresent(void);
void DvtrgaClose(void);
void DvtrgaGetInfo(unsigned int *width, unsigned int *height,
                   unsigned int *bpp);

// Colores
#define DVTRGA_COLOR_BLACK 0x00000000
#define DVTRGA_COLOR_BLUE 0x000000FF
#define DVTRGA_COLOR_GREEN 0x0000FF00
#define DVTRGA_COLOR_CYAN 0x0000FFFF
#define DVTRGA_COLOR_RED 0x00FF0000
#define DVTRGA_COLOR_MAGENTA 0x00FF00FF
#define DVTRGA_COLOR_YELLOW 0x00FFFF00
#define DVTRGA_COLOR_WHITE 0x00FFFFFF

int main(void) {
  printf("============================================================\n");
  printf("  DVTRGA Test Application - Linux\n");
  printf("  Copyright (C) 2025 José Manuel Moreno Cano\n");
  printf("============================================================\n\n");

  // Inicializar
  printf("[1/5] Inicializando DVTRGA (1024x768)...\n");
  if (DvtrgaInit(1024, 768) != 0) {
    printf("ERROR: No se pudo inicializar DVTRGA\n");
    return 1;
  }
  printf("OK\n\n");

  // Limpiar
  printf("[2/5] Limpiando pantalla...\n");
  DvtrgaClear(0x00001020);
  DvtrgaPresent();
  printf("OK\n\n");

  // Rectángulos
  printf("[3/5] Dibujando rectángulos...\n");
  DvtrgaFillRect(50, 50, 200, 100, DVTRGA_COLOR_RED);
  DvtrgaFillRect(50, 160, 200, 100, DVTRGA_COLOR_GREEN);
  DvtrgaFillRect(50, 270, 200, 100, DVTRGA_COLOR_BLUE);
  DvtrgaPresent();
  printf("OK\n\n");

  // Líneas
  printf("[4/5] Dibujando líneas...\n");
  DvtrgaDrawLine(300, 50, 500, 150, DVTRGA_COLOR_WHITE);
  DvtrgaDrawLine(300, 150, 500, 50, DVTRGA_COLOR_WHITE);
  DvtrgaPresent();
  printf("OK\n\n");

  // Píxeles
  printf("[5/5] Dibujando píxeles...\n");
  for (int i = 0; i < 200; i++) {
    DvtrgaPutPixel(300 + i, 350, DVTRGA_COLOR_MAGENTA);
  }
  DvtrgaPresent();
  printf("OK\n\n");

  printf("============================================================\n");
  printf("  Prueba completada - Presiona Ctrl+C para salir\n");
  printf("============================================================\n\n");

  // Mantener ventana abierta
  sleep(10);

  DvtrgaClose();
  return 0;
}
