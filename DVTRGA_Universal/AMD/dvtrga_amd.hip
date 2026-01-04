/*
 * DVTRGA - AMD ROCm Version
 * Direct Visual Transport & Render Graphics Architecture
 * 
 * Copyright (C) 2025 José Manuel Moreno Cano
 * Todos los derechos reservados. All rights reserved.
 * 
 * API: HIP (ROCm) + Vulkan
 * Compilar: hipcc -o libdvtrga_amd.so --shared dvtrga_amd.hip -lvulkan
 */

#include <hip/hip_runtime.h>
#include <vulkan/vulkan.h>
#include <stdint.h>

typedef struct {
    uint32_t *d_framebuffer;  // Device framebuffer
    uint32_t *h_framebuffer;  // Host framebuffer
    uint32_t width;
    uint32_t height;
    hipStream_t stream;
    uint8_t initialized;
} DVTRGA_AMD_CONTEXT;

static DVTRGA_AMD_CONTEXT g_ctx = {0};

// Kernel HIP para limpiar pantalla
__global__ void dvtrga_clear_hip(uint32_t *fb, uint32_t color, int size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size) {
        fb[idx] = color;
    }
}

// Kernel HIP para dibujar píxel
__global__ void dvtrga_putpixel_hip(uint32_t *fb, int x, int y, int width, uint32_t color) {
    fb[y * width + x] = color;
}

// Inicializar DVTRGA con HIP
int DvtrgaInit_AMD(uint32_t width, uint32_t height) {
    if (g_ctx.initialized) return 0;
    
    // Seleccionar GPU AMD
    hipSetDevice(0);
    
    // Alocar framebuffer en GPU
    size_t size = width * height * sizeof(uint32_t);
    hipMalloc(&g_ctx.d_framebuffer, size);
    
    // Alocar framebuffer en CPU
    hipHostMalloc(&g_ctx.h_framebuffer, size);
    
    // Crear stream
    hipStreamCreate(&g_ctx.stream);
    
    g_ctx.width = width;
    g_ctx.height = height;
    g_ctx.initialized = 1;
    
    return 0;
}

// Limpiar pantalla
void DvtrgaClear_AMD(uint32_t color) {
    if (!g_ctx.initialized) return;
    
    int size = g_ctx.width * g_ctx.height;
    int threadsPerBlock = 256;
    int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;
    
    hipLaunchKernelGGL(dvtrga_clear_hip,
                       dim3(blocksPerGrid), dim3(threadsPerBlock),
                       0, g_ctx.stream,
                       g_ctx.d_framebuffer, color, size);
}

// Dibujar píxel
void DvtrgaPutPixel_AMD(uint32_t x, uint32_t y, uint32_t color) {
    if (!g_ctx.initialized) return;
    if (x >= g_ctx.width || y >= g_ctx.height) return;
    
    hipLaunchKernelGGL(dvtrga_putpixel_hip,
                       dim3(1), dim3(1),
                       0, g_ctx.stream,
                       g_ctx.d_framebuffer, x, y, g_ctx.width, color);
}

// Copiar framebuffer a CPU
void DvtrgaCopyToHost_AMD(void) {
    if (!g_ctx.initialized) return;
    
    size_t size = g_ctx.width * g_ctx.height * sizeof(uint32_t);
    hipMemcpyAsync(g_ctx.h_framebuffer, g_ctx.d_framebuffer, size,
                   hipMemcpyDeviceToHost, g_ctx.stream);
    hipStreamSynchronize(g_ctx.stream);
}

// Cerrar
void DvtrgaClose_AMD(void) {
    if (!g_ctx.initialized) return;
    
    hipStreamDestroy(g_ctx.stream);
    hipFree(g_ctx.d_framebuffer);
    hipHostFree(g_ctx.h_framebuffer);
    
    g_ctx.initialized = 0;
}
