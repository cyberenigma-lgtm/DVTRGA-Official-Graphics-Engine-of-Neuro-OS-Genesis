/*
 * ============================================================
 * NEURO-OS LOGO - Demo con DVTRGA
 * ============================================================
 * Dibuja el logo de Neuro-OS usando DVTRGA
 * Estilo: Cyberpunk con efectos neón
 *
 * Copyright (C) 2025 José Manuel Moreno Cano
 * ============================================================
 */

#include "dvtrga_api.h"
#include <stdio.h>

#define WIDTH 1024
#define HEIGHT 768

// Colores Neuro-OS (Cyberpunk)
#define COLOR_BG 0x0A0E27      // Azul profundo
#define COLOR_CYAN 0x00FFFF    // Cyan neón
#define COLOR_MAGENTA 0xFF00FF // Magenta neón
#define COLOR_GREEN 0x00FF00   // Verde terminal
#define COLOR_PURPLE 0x8B00FF  // Púrpura

// Dibujar círculo simple
void draw_circle(int cx, int cy, int radius, uint32_t color) {
  for (int y = -radius; y <= radius; y++) {
    for (int x = -radius; x <= radius; x++) {
      if (x * x + y * y <= radius * radius) {
        int px = cx + x;
        int py = cy + y;
        if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
          dvtrga_put_pixel(px, py, color);
        }
      }
    }
  }
}

int main(void) {
  printf("============================================================\n");
  printf("  NEURO-OS LOGO - Demo con DVTRGA\n");
  printf("  Direct Visual Transport & Render Graphics Architecture\n");
  printf("============================================================\n\n");

  // Inicializar DVTRGA
  printf("Inicializando DVTRGA (%dx%d)...\n", WIDTH, HEIGHT);
  if (!dvtrga_init(WIDTH, HEIGHT)) {
    printf("ERROR: No se pudo inicializar DVTRGA\n");
    return 1;
  }
  printf("OK: DVTRGA inicializado\n\n");

  // Limpiar pantalla (fondo azul profundo)
  printf("Limpiando pantalla (azul profundo cyberpunk)...\n");
  dvtrga_clear(COLOR_BG);
  printf("OK\n\n");

  int center_x = WIDTH / 2;
  int center_y = HEIGHT / 2;

  printf("[1/5] Dibujando círculo exterior (cyan)...\n");
  // Círculo exterior (cyan neón)
  draw_circle(center_x, center_y, 100, COLOR_CYAN);

  printf("[2/5] Dibujando red neuronal...\n");
  // 8 nodos alrededor
  int angles[] = {0, 45, 90, 135, 180, 225, 270, 315};
  for (int i = 0; i < 8; i++) {
    // Calcular posición (aproximada sin math.h)
    int x1 = center_x;
    int y1 = center_y;

    if (angles[i] == 0) {
      x1 += 80;
      y1 += 0;
    }
    if (angles[i] == 45) {
      x1 += 56;
      y1 += 56;
    }
    if (angles[i] == 90) {
      x1 += 0;
      y1 += 80;
    }
    if (angles[i] == 135) {
      x1 -= 56;
      y1 += 56;
    }
    if (angles[i] == 180) {
      x1 -= 80;
      y1 += 0;
    }
    if (angles[i] == 225) {
      x1 -= 56;
      y1 -= 56;
    }
    if (angles[i] == 270) {
      x1 += 0;
      y1 -= 80;
    }
    if (angles[i] == 315) {
      x1 += 56;
      y1 -= 56;
    }

    // Nodo
    draw_circle(x1, y1, 8, COLOR_MAGENTA);

    // Línea al centro
    dvtrga_draw_line(center_x, center_y, x1, y1, COLOR_PURPLE);
  }

  printf("[3/5] Dibujando núcleo central...\n");
  // Núcleo central (verde neón)
  draw_circle(center_x, center_y, 30, COLOR_GREEN);

  printf("[4/5] Dibujando texto 'NEURO-OS'...\n");
  // Rectángulos para simular texto "NEURO-OS"
  int text_y = center_y + 150;

  // N
  dvtrga_fill_rect(center_x - 150, text_y, 10, 40, COLOR_CYAN);
  dvtrga_fill_rect(center_x - 150, text_y, 30, 10, COLOR_CYAN);
  dvtrga_fill_rect(center_x - 130, text_y, 10, 40, COLOR_CYAN);

  // E
  dvtrga_fill_rect(center_x - 110, text_y, 10, 40, COLOR_CYAN);
  dvtrga_fill_rect(center_x - 110, text_y, 30, 10, COLOR_CYAN);
  dvtrga_fill_rect(center_x - 110, text_y + 15, 25, 10, COLOR_CYAN);
  dvtrga_fill_rect(center_x - 110, text_y + 30, 30, 10, COLOR_CYAN);

  // U
  dvtrga_fill_rect(center_x - 70, text_y, 10, 40, COLOR_CYAN);
  dvtrga_fill_rect(center_x - 50, text_y, 10, 40, COLOR_CYAN);
  dvtrga_fill_rect(center_x - 70, text_y + 30, 30, 10, COLOR_CYAN);

  // R
  dvtrga_fill_rect(center_x - 30, text_y, 10, 40, COLOR_CYAN);
  dvtrga_fill_rect(center_x - 30, text_y, 30, 10, COLOR_CYAN);
  dvtrga_fill_rect(center_x - 30, text_y + 15, 30, 10, COLOR_CYAN);
  dvtrga_fill_rect(center_x - 10, text_y, 10, 25, COLOR_CYAN);

  // O
  dvtrga_fill_rect(center_x + 10, text_y, 10, 40, COLOR_CYAN);
  dvtrga_fill_rect(center_x + 30, text_y, 10, 40, COLOR_CYAN);
  dvtrga_fill_rect(center_x + 10, text_y, 30, 10, COLOR_CYAN);
  dvtrga_fill_rect(center_x + 10, text_y + 30, 30, 10, COLOR_CYAN);

  // -
  dvtrga_fill_rect(center_x + 50, text_y + 15, 20, 10, COLOR_CYAN);

  // O
  dvtrga_fill_rect(center_x + 80, text_y, 10, 40, COLOR_CYAN);
  dvtrga_fill_rect(center_x + 100, text_y, 10, 40, COLOR_CYAN);
  dvtrga_fill_rect(center_x + 80, text_y, 30, 10, COLOR_CYAN);
  dvtrga_fill_rect(center_x + 80, text_y + 30, 30, 10, COLOR_CYAN);

  // S
  dvtrga_fill_rect(center_x + 120, text_y, 30, 10, COLOR_CYAN);
  dvtrga_fill_rect(center_x + 120, text_y, 10, 20, COLOR_CYAN);
  dvtrga_fill_rect(center_x + 120, text_y + 15, 30, 10, COLOR_CYAN);
  dvtrga_fill_rect(center_x + 140, text_y + 20, 10, 20, COLOR_CYAN);
  dvtrga_fill_rect(center_x + 120, text_y + 30, 30, 10, COLOR_CYAN);

  printf("[5/5] Añadiendo efectos...\n");
  // Partículas decorativas
  for (int i = 0; i < 200; i++) {
    int px = (i * 137) % WIDTH;
    int py = (i * 211) % HEIGHT;
    uint32_t color = (i % 2) ? COLOR_CYAN : COLOR_MAGENTA;
    dvtrga_put_pixel(px, py, color);
  }

  printf("OK: Logo dibujado\n\n");

  // Guardar imagen
  printf("Guardando imagen...\n");
  if (dvtrga_save_bmp("neuro_os_logo.bmp")) {
    printf("OK: Imagen guardada en neuro_os_logo.bmp\n");
  } else {
    printf("ERROR: No se pudo guardar la imagen\n");
  }

  // Presentar en pantalla
  printf("\nPresentando en pantalla...\n");
  dvtrga_present();
  printf("OK\n\n");

  printf("============================================================\n");
  printf("  Logo de Neuro-OS dibujado exitosamente\n");
  printf("============================================================\n\n");
  printf("Archivo generado: neuro_os_logo.bmp\n");
  printf("Presiona Enter para cerrar...");
  getchar();

  // Cerrar DVTRGA
  dvtrga_shutdown();

  return 0;
}
