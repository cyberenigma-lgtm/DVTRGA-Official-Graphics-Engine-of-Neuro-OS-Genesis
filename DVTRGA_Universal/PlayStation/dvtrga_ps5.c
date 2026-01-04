/*
 * DVTRGA - PlayStation 5 Version
 * Direct Visual Transport & Render Graphics Architecture
 *
 * Copyright (C) 2025 José Manuel Moreno Cano
 * Todos los derechos reservados. All rights reserved.
 *
 * API: GNM/GNMX (PlayStation Graphics API)
 * Compilar: orbis-clang -o dvtrga_ps5.prx dvtrga_ps5.c -lSceGnmDriver
 */

#include <gnm.h>
#include <gnmx.h>
#include <kernel.h>
#include <stdint.h>

typedef struct {
  sce::Gnmx::GfxContext *gfxContext;
  void *framebuffer;
  uint32_t width;
  uint32_t height;
  uint8_t initialized;
} DVTRGA_PS5_CONTEXT;

static DVTRGA_PS5_CONTEXT g_ctx = {0};

// Inicializar DVTRGA para PS5
int DvtrgaInit_PS5(uint32_t width, uint32_t height) {
  if (g_ctx.initialized)
    return 0;

  // Inicializar GNM
  sce::Gnm::initialize();

  // Crear contexto gráfico
  g_ctx.gfxContext = new sce::Gnmx::GfxContext();

  // Alocar framebuffer en VRAM
  g_ctx.framebuffer = sce::Gnm::allocateVideoMemory(
      width * height * 4, sce::Gnm::kAlignmentOfBufferInBytes);

  g_ctx.width = width;
  g_ctx.height = height;
  g_ctx.initialized = 1;

  return 0;
}

// Limpiar pantalla
void DvtrgaClear_PS5(uint32_t color) {
  if (!g_ctx.initialized)
    return;

  uint32_t *fb = (uint32_t *)g_ctx.framebuffer;
  for (uint32_t i = 0; i < g_ctx.width * g_ctx.height; i++) {
    fb[i] = color;
  }
}

// Presentar en pantalla
void DvtrgaPresent_PS5(void) {
  if (!g_ctx.initialized)
    return;

  g_ctx.gfxContext->submit();
  g_ctx.gfxContext->waitForIdle();
}

// Cerrar
void DvtrgaClose_PS5(void) {
  if (!g_ctx.initialized)
    return;

  if (g_ctx.framebuffer) {
    sce::Gnm::deallocateVideoMemory(g_ctx.framebuffer);
  }

  delete g_ctx.gfxContext;
  sce::Gnm::finalize();

  g_ctx.initialized = 0;
}
