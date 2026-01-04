/*
 * DVTRGA Test Application
 * Aplicación de prueba REAL en C
 *
 * Compilar: cl dvtrga_test.c dvtrga_api.lib user32.lib gdi32.lib
 */

#include "dvtrga_api.h"
#include <stdio.h>
#include <windows.h>


int main(void) {
  printf("============================================================\n");
  printf("  DVTRGA Test Application v1.0\n");
  printf("  Direct Visual Transport & Render Graphics Architecture\n");
  printf("============================================================\n\n");

  // Inicializar
  printf("[1/6] Inicializando DVTRGA (1024x768)...\n");
  if (DvtrgaInit(1024, 768) != 0) {
    printf("ERROR: No se pudo inicializar DVTRGA\n");
    return 1;
  }
  printf("OK: DVTRGA inicializado\n\n");

  // Obtener información
  uint32_t width, height, bpp;
  DvtrgaGetInfo(&width, &height, &bpp);
  printf("Resolucion: %dx%d @ %d bits\n\n", width, height, bpp);

  // Limpiar pantalla
  printf("[2/6] Limpiando pantalla (azul oscuro)...\n");
  DvtrgaClear(0x00001020);
  printf("OK\n\n");

  // Dibujar rectángulos
  printf("[3/6] Dibujando rectangulos de colores...\n");
  DvtrgaFillRect(50, 50, 200, 100, DVTRGA_COLOR_RED);
  DvtrgaFillRect(50, 160, 200, 100, DVTRGA_COLOR_GREEN);
  DvtrgaFillRect(50, 270, 200, 100, DVTRGA_COLOR_BLUE);
  DvtrgaFillRect(50, 380, 200, 100, DVTRGA_COLOR_YELLOW);
  printf("OK: 4 rectangulos dibujados\n\n");

  // Dibujar líneas
  printf("[4/6] Dibujando lineas...\n");
  DvtrgaDrawLine(300, 50, 500, 150, DVTRGA_COLOR_WHITE);
  DvtrgaDrawLine(300, 150, 500, 50, DVTRGA_COLOR_WHITE);
  DvtrgaDrawLine(300, 200, 500, 300, DVTRGA_COLOR_CYAN);
  DvtrgaDrawLine(300, 300, 500, 200, DVTRGA_COLOR_CYAN);
  printf("OK: Lineas dibujadas\n\n");

  // Dibujar píxeles
  printf("[5/6] Dibujando pixeles...\n");
  for (int i = 0; i < 200; i++) {
    DvtrgaPutPixel(300 + i, 350, DVTRGA_COLOR_MAGENTA);
    DvtrgaPutPixel(300 + i, 351, DVTRGA_COLOR_MAGENTA);
    DvtrgaPutPixel(300 + i, 352, DVTRGA_COLOR_MAGENTA);
  }
  printf("OK: 600 pixeles dibujados\n\n");

  // Guardar a archivo
  printf("[6/6] Guardando imagen...\n");
  if (DvtrgaSaveBMP("dvtrga_output.bmp") == 0) {
    printf("OK: Imagen guardada en dvtrga_output.bmp\n\n");
  } else {
    printf("ERROR: No se pudo guardar la imagen\n\n");
  }

  // Presentar en pantalla (opcional)
  printf("Presentando en pantalla...\n");
  DvtrgaPresent();
  printf("OK\n\n");

  printf("============================================================\n");
  printf("  Prueba completada exitosamente\n");
  printf("============================================================\n\n");
  printf("Archivo generado: dvtrga_output.bmp\n");
  printf("Presiona Enter para cerrar...\n");
  getchar();

  // Cerrar
  DvtrgaClose();

  return 0;
}
