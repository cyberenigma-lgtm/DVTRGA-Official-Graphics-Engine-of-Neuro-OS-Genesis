/*
 * DVTRGA - NVIDIA CUDA Version
 * Direct Visual Transport & Render Graphics Architecture
 *
 * Copyright (C) 2025 José Manuel Moreno Cano
 * Todos los derechos reservados. All rights reserved.
 *
 * API: CUDA + Vulkan
 * Compilar: nvcc -o libdvtrga_nvidia.so --shared dvtrga_nvidia.cu -lcuda
 * -lvulkan
 */

#include <cuda_runtime.h>
#include <stdint.h>
#include <vulkan/vulkan.h>


typedef struct {
  uint32_t *d_framebuffer; // Device framebuffer
  uint32_t *h_framebuffer; // Host framebuffer
  uint32_t width;
  uint32_t height;
  cudaStream_t stream;
  uint8_t initialized;
} DVTRGA_NVIDIA_CONTEXT;

static DVTRGA_NVIDIA_CONTEXT g_ctx = {0};

// Kernel CUDA para limpiar pantalla
__global__ void dvtrga_clear_kernel(uint32_t *fb, uint32_t color, int size) {
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  if (idx < size) {
    fb[idx] = color;
  }
}

// Kernel CUDA para dibujar píxel
__global__ void dvtrga_putpixel_kernel(uint32_t *fb, int x, int y, int width,
                                       uint32_t color) {
  fb[y * width + x] = color;
}

// Inicializar DVTRGA con CUDA
int DvtrgaInit_NVIDIA(uint32_t width, uint32_t height) {
  if (g_ctx.initialized)
    return 0;

  // Seleccionar GPU
  cudaSetDevice(0);

  // Alocar framebuffer en GPU
  size_t size = width * height * sizeof(uint32_t);
  cudaMalloc(&g_ctx.d_framebuffer, size);

  // Alocar framebuffer en CPU
  cudaMallocHost(&g_ctx.h_framebuffer, size);

  // Crear stream
  cudaStreamCreate(&g_ctx.stream);

  g_ctx.width = width;
  g_ctx.height = height;
  g_ctx.initialized = 1;

  return 0;
}

// Limpiar pantalla
void DvtrgaClear_NVIDIA(uint32_t color) {
  if (!g_ctx.initialized)
    return;

  int size = g_ctx.width * g_ctx.height;
  int threadsPerBlock = 256;
  int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;

  dvtrga_clear_kernel<<<blocksPerGrid, threadsPerBlock, 0, g_ctx.stream>>>(
      g_ctx.d_framebuffer, color, size);
}

// Dibujar píxel
void DvtrgaPutPixel_NVIDIA(uint32_t x, uint32_t y, uint32_t color) {
  if (!g_ctx.initialized)
    return;
  if (x >= g_ctx.width || y >= g_ctx.height)
    return;

  dvtrga_putpixel_kernel<<<1, 1, 0, g_ctx.stream>>>(g_ctx.d_framebuffer, x, y,
                                                    g_ctx.width, color);
}

// Copiar framebuffer a CPU
void DvtrgaCopyToHost_NVIDIA(void) {
  if (!g_ctx.initialized)
    return;

  size_t size = g_ctx.width * g_ctx.height * sizeof(uint32_t);
  cudaMemcpyAsync(g_ctx.h_framebuffer, g_ctx.d_framebuffer, size,
                  cudaMemcpyDeviceToHost, g_ctx.stream);
  cudaStreamSynchronize(g_ctx.stream);
}

// Cerrar
void DvtrgaClose_NVIDIA(void) {
  if (!g_ctx.initialized)
    return;

  cudaStreamDestroy(g_ctx.stream);
  cudaFree(g_ctx.d_framebuffer);
  cudaFreeHost(g_ctx.h_framebuffer);

  g_ctx.initialized = 0;
}
