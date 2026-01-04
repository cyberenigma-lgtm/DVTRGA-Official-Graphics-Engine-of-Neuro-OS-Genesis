/*
 * DVTRGA - Xbox Series X|S Version
 * Direct Visual Transport & Render Graphics Architecture
 *
 * Copyright (C) 2025 José Manuel Moreno Cano
 * Todos los derechos reservados. All rights reserved.
 *
 * API: DirectX 12
 * Compilar: cl /c dvtrga_xbox.cpp && link /DLL /OUT:dvtrga_xbox.dll
 */

#include <d3d12.h>
#include <dxgi1_6.h>
#include <stdint.h>
#include <wrl/client.h>


using Microsoft::WRL::ComPtr;

typedef struct {
  ComPtr<ID3D12Device> device;
  ComPtr<ID3D12CommandQueue> commandQueue;
  ComPtr<ID3D12Resource> renderTarget;
  uint32_t width;
  uint32_t height;
  uint8_t initialized;
} DVTRGA_XBOX_CONTEXT;

static DVTRGA_XBOX_CONTEXT g_ctx = {0};

// Inicializar DVTRGA para Xbox
int DvtrgaInit_Xbox(uint32_t width, uint32_t height) {
  if (g_ctx.initialized)
    return 0;

  // Crear dispositivo D3D12
  HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0,
                                 IID_PPV_ARGS(&g_ctx.device));

  if (FAILED(hr))
    return -1;

  // Crear command queue
  D3D12_COMMAND_QUEUE_DESC queueDesc = {};
  queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

  hr = g_ctx.device->CreateCommandQueue(&queueDesc,
                                        IID_PPV_ARGS(&g_ctx.commandQueue));
  if (FAILED(hr))
    return -1;

  // Crear render target
  D3D12_HEAP_PROPERTIES heapProps = {};
  heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

  D3D12_RESOURCE_DESC resourceDesc = {};
  resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
  resourceDesc.Width = width;
  resourceDesc.Height = height;
  resourceDesc.DepthOrArraySize = 1;
  resourceDesc.MipLevels = 1;
  resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  resourceDesc.SampleDesc.Count = 1;

  hr = g_ctx.device->CreateCommittedResource(
      &heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc,
      D3D12_RESOURCE_STATE_RENDER_TARGET, nullptr,
      IID_PPV_ARGS(&g_ctx.renderTarget));

  if (FAILED(hr))
    return -1;

  g_ctx.width = width;
  g_ctx.height = height;
  g_ctx.initialized = 1;

  return 0;
}

// Limpiar pantalla
void DvtrgaClear_Xbox(uint32_t color) {
  if (!g_ctx.initialized)
    return;

  // Implementar clear con DirectX 12
  float clearColor[4] = {
      ((color >> 16) & 0xFF) / 255.0f, ((color >> 8) & 0xFF) / 255.0f,
      (color & 0xFF) / 255.0f, ((color >> 24) & 0xFF) / 255.0f};

  // Clear render target (requiere command list)
}

// Presentar
void DvtrgaPresent_Xbox(void) {
  if (!g_ctx.initialized)
    return;

  // Submit command queue
  g_ctx.commandQueue->Signal(nullptr, 0);
}

// Cerrar
void DvtrgaClose_Xbox(void) {
  if (!g_ctx.initialized)
    return;

  g_ctx.renderTarget.Reset();
  g_ctx.commandQueue.Reset();
  g_ctx.device.Reset();

  g_ctx.initialized = 0;
}
