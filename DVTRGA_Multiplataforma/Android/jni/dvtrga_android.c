/*
 * DVTRGA API - Android Version (NDK)
 * Direct Visual Transport & Render Graphics Architecture
 *
 * Copyright (C) 2025 José Manuel Moreno Cano
 * Todos los derechos reservados. All rights reserved.
 *
 * Autor: José Manuel Moreno Cano
 * Email: admin@neuro-os.es
 * Web: https://neuro-os.es
 *
 * Compilar con Android NDK:
 * ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=Android.mk
 */

#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <jni.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define LOG_TAG "DVTRGA"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Contexto DVTRGA
typedef struct {
  ANativeWindow *window;
  ANativeWindow_Buffer buffer;
  uint32_t *framebuffer;
  uint32_t width;
  uint32_t height;
  uint8_t initialized;
} DVTRGA_CONTEXT;

// Contexto global
static DVTRGA_CONTEXT g_ctx = {0};

// Inicializar DVTRGA
JNIEXPORT jint JNICALL Java_es_neuroos_dvtrga_DVTRGA_init(
    JNIEnv *env, jobject obj, jobject surface, jint width, jint height) {
  if (g_ctx.initialized)
    return 0;

  // Obtener native window desde Surface
  g_ctx.window = ANativeWindow_fromSurface(env, surface);
  if (!g_ctx.window) {
    LOGE("Failed to get native window");
    return -1;
  }

  // Configurar formato
  ANativeWindow_setBuffersGeometry(g_ctx.window, width, height,
                                   WINDOW_FORMAT_RGBA_8888);

  // Crear framebuffer
  g_ctx.framebuffer = (uint32_t *)malloc(width * height * sizeof(uint32_t));
  if (!g_ctx.framebuffer) {
    ANativeWindow_release(g_ctx.window);
    return -1;
  }

  g_ctx.width = width;
  g_ctx.height = height;
  g_ctx.initialized = 1;

  LOGI("DVTRGA initialized: %dx%d", width, height);
  return 0;
}

// Limpiar pantalla
JNIEXPORT void JNICALL Java_es_neuroos_dvtrga_DVTRGA_clear(JNIEnv *env,
                                                           jobject obj,
                                                           jint color) {
  if (!g_ctx.initialized)
    return;

  for (uint32_t i = 0; i < g_ctx.width * g_ctx.height; i++) {
    g_ctx.framebuffer[i] = (uint32_t)color;
  }
}

// Dibujar píxel
JNIEXPORT void JNICALL Java_es_neuroos_dvtrga_DVTRGA_putPixel(JNIEnv *env,
                                                              jobject obj,
                                                              jint x, jint y,
                                                              jint color) {
  if (!g_ctx.initialized)
    return;
  if (x < 0 || x >= (jint)g_ctx.width || y < 0 || y >= (jint)g_ctx.height)
    return;

  g_ctx.framebuffer[y * g_ctx.width + x] = (uint32_t)color;
}

// Dibujar rectángulo lleno
JNIEXPORT void JNICALL Java_es_neuroos_dvtrga_DVTRGA_fillRect(
    JNIEnv *env, jobject obj, jint x, jint y, jint w, jint h, jint color) {
  if (!g_ctx.initialized)
    return;

  for (jint dy = 0; dy < h; dy++) {
    for (jint dx = 0; dx < w; dx++) {
      jint px = x + dx;
      jint py = y + dy;
      if (px >= 0 && px < (jint)g_ctx.width && py >= 0 &&
          py < (jint)g_ctx.height) {
        g_ctx.framebuffer[py * g_ctx.width + px] = (uint32_t)color;
      }
    }
  }
}

// Dibujar línea
JNIEXPORT void JNICALL Java_es_neuroos_dvtrga_DVTRGA_drawLine(
    JNIEnv *env, jobject obj, jint x1, jint y1, jint x2, jint y2, jint color) {
  if (!g_ctx.initialized)
    return;

  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = x1 < x2 ? 1 : -1;
  int sy = y1 < y2 ? 1 : -1;
  int err = dx - dy;

  while (1) {
    if (x1 >= 0 && x1 < (jint)g_ctx.width && y1 >= 0 &&
        y1 < (jint)g_ctx.height) {
      g_ctx.framebuffer[y1 * g_ctx.width + x1] = (uint32_t)color;
    }

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
JNIEXPORT void JNICALL Java_es_neuroos_dvtrga_DVTRGA_present(JNIEnv *env,
                                                             jobject obj) {
  if (!g_ctx.initialized || !g_ctx.window)
    return;

  // Bloquear buffer
  if (ANativeWindow_lock(g_ctx.window, &g_ctx.buffer, NULL) < 0) {
    LOGE("Failed to lock window buffer");
    return;
  }

  // Copiar framebuffer al buffer nativo
  uint32_t *pixels = (uint32_t *)g_ctx.buffer.bits;
  for (uint32_t y = 0; y < g_ctx.height && y < (uint32_t)g_ctx.buffer.height;
       y++) {
    memcpy(&pixels[y * g_ctx.buffer.stride],
           &g_ctx.framebuffer[y * g_ctx.width], g_ctx.width * sizeof(uint32_t));
  }

  // Desbloquear y mostrar
  ANativeWindow_unlockAndPost(g_ctx.window);
}

// Cerrar DVTRGA
JNIEXPORT void JNICALL Java_es_neuroos_dvtrga_DVTRGA_close(JNIEnv *env,
                                                           jobject obj) {
  if (!g_ctx.initialized)
    return;

  if (g_ctx.framebuffer)
    free(g_ctx.framebuffer);
  if (g_ctx.window)
    ANativeWindow_release(g_ctx.window);

  memset(&g_ctx, 0, sizeof(g_ctx));
  LOGI("DVTRGA closed");
}
