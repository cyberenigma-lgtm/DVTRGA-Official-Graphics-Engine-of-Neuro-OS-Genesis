/*
 * SIGLO22 GPU DRIVER - U9 OPTIMIZED KERNEL
 * "Hardcoded" for maximum throughput on Windows 11
 */

#include "dvtrga_api.h"
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define TARGET_FPS 4000
#define NUM_PARTICLES 1000000

// Colors from screenshot
#define COL_CYAN 0x00FFFF
#define COL_MAGENTA 0xFF00FF
#define COL_YELLOW 0xFFFF00
#define COL_WHITE 0xFFFFFF
#define COL_BLACK 0x000000
#define COL_GREEN 0x00FF00
#define COL_RED 0xFF0000

typedef struct {
  float x, y;
  float vx, vy;
  uint32_t color;
} Particle;

Particle *particles = NULL;
uint32_t *raw_buffer = NULL;
int SCREEN_W = 1024;
int SCREEN_H = 768;

// Minimal 5x7 Font (A-Z, 0-9, space, punctuation)
const uint8_t font_5x7[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, // Space
    0x00, 0x5F, 0x00, 0x00, 0x00, // !
    0x07, 0x00, 0x07, 0x00, 0x00, // "
    0x14, 0x7F, 0x14, 0x7F, 0x14, // #
    0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
    0x23, 0x13, 0x08, 0x64, 0x62, // %
    0x36, 0x49, 0x55, 0x22, 0x50, // &
    0x00, 0x05, 0x03, 0x00, 0x00, // '
    0x00, 0x1C, 0x22, 0x41, 0x00, // (
    0x00, 0x41, 0x22, 0x1C, 0x00, // )
    0x14, 0x08, 0x3E, 0x08, 0x14, // *
    0x08, 0x08, 0x3E, 0x08, 0x08, // +
    0x00, 0x50, 0x30, 0x00, 0x00, // ,
    0x08, 0x08, 0x08, 0x08, 0x08, // -
    0x00, 0x60, 0x60, 0x00, 0x00, // .
    0x20, 0x10, 0x08, 0x04, 0x02, // /
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x42, 0x61, 0x51, 0x49, 0x46, // 2
    0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x06, 0x49, 0x49, 0x29, 0x1E, // 9
    0x00, 0x36, 0x36, 0x00, 0x00, // :
    0x00, 0x56, 0x36, 0x00, 0x00, // ;
    0x08, 0x14, 0x22, 0x41, 0x00, // <
    0x14, 0x14, 0x14, 0x14, 0x14, // =
    0x00, 0x41, 0x22, 0x14, 0x08, // >
    0x02, 0x01, 0x51, 0x09, 0x06, // ?
    0x32, 0x49, 0x79, 0x41, 0x3E, // @
    0x7E, 0x11, 0x11, 0x11, 0x7E, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x7F, 0x09, 0x09, 0x09, 0x01, // F
    0x3E, 0x41, 0x49, 0x49, 0x7A, // G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x7F, 0x09, 0x09, 0x09, 0x06, // P
    0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x46, 0x49, 0x49, 0x49, 0x31, // S
    0x01, 0x01, 0x7F, 0x01, 0x01, // T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x3F, 0x40, 0x38, 0x40, 0x3F, // W
    0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x07, 0x08, 0x70, 0x08, 0x07, // Y
    0x61, 0x51, 0x49, 0x45, 0x43, // Z
};

// High precision timer
double get_time() {
  static LARGE_INTEGER freq;
  static int init = 0;
  if (!init) {
    QueryPerformanceFrequency(&freq);
    init = 1;
  }
  LARGE_INTEGER count;
  QueryPerformanceCounter(&count);
  return (double)count.QuadPart / (double)freq.QuadPart;
}

// Draw a single character scaled 2x
void draw_char(int x, int y, char c, uint32_t color) {
  if (c < 32 || c > 90)
    c = 32;
  int index = (c - 32) * 5;

  // Scale 2x
  for (int i = 0; i < 5; ++i) {
    uint8_t line = font_5x7[index + i];
    for (int j = 0; j < 8; ++j) {
      if ((line >> j) & 1) {
        // 2x2 pixel
        if (x + i * 2 < SCREEN_W && y + j * 2 < SCREEN_H)
          raw_buffer[(y + j * 2) * SCREEN_W + (x + i * 2)] = color;
        if (x + i * 2 + 1 < SCREEN_W && y + j * 2 < SCREEN_H)
          raw_buffer[(y + j * 2) * SCREEN_W + (x + i * 2 + 1)] = color;
        if (x + i * 2 < SCREEN_W && y + j * 2 + 1 < SCREEN_H)
          raw_buffer[(y + j * 2 + 1) * SCREEN_W + (x + i * 2)] = color;
        if (x + i * 2 + 1 < SCREEN_W && y + j * 2 + 1 < SCREEN_H)
          raw_buffer[(y + j * 2 + 1) * SCREEN_W + (x + i * 2 + 1)] = color;
      }
    }
  }
}

// Draw string with spacing
void draw_string(int x, int y, const char *str, uint32_t color) {
  while (*str) {
    draw_char(x, y, *str++, color);
    x += 12; // 5*2 + 2 spacing
  }
}

// Fast LCG Random
unsigned int fast_rand(unsigned int *seed) {
  *seed = *seed * 1103515245 + 12345;
  return (*seed / 65536) % 32768;
}

int main() {
  // Dynamic Resolution
  SCREEN_W = GetSystemMetrics(SM_CXSCREEN);
  SCREEN_H = GetSystemMetrics(SM_CYSCREEN);
  if (SCREEN_W == 0)
    SCREEN_W = 1920;
  if (SCREEN_H == 0)
    SCREEN_H = 1080;

  // SAFETY LIMIT: Leave 2 cores for Windows OS/Background tasks to prevent
  // "watchdog kill" 5100 FPS at Realtime priority starves the kernel.
  int num_procs = omp_get_num_procs();
  if (num_procs > 4) {
    omp_set_num_threads(num_procs - 2);
    printf("SAFE MODE: Using %d/%d Cores to prevent System Freeze.\n",
           num_procs - 2, num_procs);
  }

  // Use HIGH instead of REALTIME to allow mouse/keyboard/OS input processing
  SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

  if (DvtrgaInit(SCREEN_W, SCREEN_H, "DVTRGAV001") != 0) {
    return 1;
  }

  raw_buffer = DvtrgaGetRawBuffer();
  particles = (Particle *)malloc(sizeof(Particle) * NUM_PARTICLES);

  // Init Particles
  srand((unsigned int)time(NULL));
  for (int i = 0; i < NUM_PARTICLES; ++i) {
    particles[i].x = (float)(rand() % SCREEN_W);
    particles[i].y = (float)(rand() % SCREEN_H);
    particles[i].vx = 0;
    particles[i].vy = (float)((rand() % 5) + 2);
    particles[i].color = (rand() % 10 > 8) ? COL_WHITE : COL_CYAN;
  }

  double last_time = get_time();
  double frame_timer = last_time;
  double render_timer = last_time;

  long long sim_frames = 0;
  double current_sim_fps = 0.0;
  char hud_buffer[256];

  printf("SIGLO22 GPU DRIVER STARTED. PRESS ESC TO EXIT.\n");

  while (1) {
    double now = get_time();

    // Simulation Loop - UNLIMITED SPEED
    sim_frames++;

    // Calculate SIM FPS
    if (now - frame_timer >= 1.0) {
      current_sim_fps = (double)sim_frames / (now - frame_timer);
      sim_frames = 0;
      frame_timer = now;
      printf("\rREAL FPS: %.0f | PPS: %.2f M | PROFILE: (TURBO)",
             current_sim_fps, (current_sim_fps * NUM_PARTICLES) / 1000000.0);
    }

    // Parallel Update - NO LOCKS
    int i;
#pragma omp parallel for private(i)
    for (i = 0; i < NUM_PARTICLES; ++i) {
      Particle *p = &particles[i];

      // Update Physics
      p->y += p->vy;

      // Boundary Wrap
      if (p->y >= SCREEN_H) {
        p->y = 0;
        // Deterministic pseudo-random re-spawn to avoid global rand() lock
        // LCG step: x = (a * x + c) % m
        unsigned int seed = (unsigned int)(p->x) + i;
        seed = seed * 1103515245 + 12345;
        p->x = (float)((seed / 65536) % SCREEN_W);
      }
    }

    // Render Loop (Capped to 60 FPS for display if needed, but we decoupled so
    // we just present last state) Actually in previous fast ver we rendered at
    // 60hz separately? Wait, the "fastest" version decoupled rendering.
    if (now - render_timer > 0.016) {
      render_timer = now;

      // Clear
      memset(raw_buffer, 0, SCREEN_W * SCREEN_H * 4);

      // Draw Particles
#pragma omp parallel for private(i)
      for (i = 0; i < NUM_PARTICLES; ++i) {
        Particle *p = &particles[i];
        int px = (int)p->x;
        int py = (int)p->y;
        // Unsafe check is faster, boundary logic in Sim handles it mostly
        if (px >= 0 && px < SCREEN_W && py >= 0 && py < SCREEN_H) {
          raw_buffer[py * SCREEN_W + px] = p->color;
        }
      }

      // HUD
      double pps_m = (current_sim_fps * NUM_PARTICLES) / 1000000.0;

      draw_string(20, 20, "SO EXPERIMENTAL PROPIO - DVTRGA CORE", COL_YELLOW);

      sprintf_s(hud_buffer, 256, "SIGLO22 GPU DRIVER");
      draw_string(SCREEN_W - 350, SCREEN_H - 120, hud_buffer, COL_WHITE);

      sprintf_s(hud_buffer, 256, "TARGET FPS: %d", TARGET_FPS);
      draw_string(SCREEN_W - 350, SCREEN_H - 100, hud_buffer, COL_GREEN);

      sprintf_s(hud_buffer, 256, "REAL FPS:   %.0f", current_sim_fps);
      draw_string(SCREEN_W - 350, SCREEN_H - 80, hud_buffer, COL_MAGENTA);

      sprintf_s(hud_buffer, 256, "PPS:        %.2f M", pps_m);
      draw_string(SCREEN_W - 350, SCREEN_H - 60, hud_buffer, COL_CYAN);

      draw_string(20, SCREEN_H - 40, "[ESC] EXIT | [S] SCREENSHOT", COL_WHITE);

      DvtrgaPresent();

      // Input
      MSG msg;
      while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT)
          return 0;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        break;
      if (GetAsyncKeyState('S') & 0x8000) {
        DvtrgaSaveBMP("screenshot_u9_turbo.bmp");
        draw_string(20, 60, "SCREENSHOT SAVED", COL_GREEN);
        DvtrgaPresent();
      }
    }
  }

  DvtrgaClose();
  return 0;
}
