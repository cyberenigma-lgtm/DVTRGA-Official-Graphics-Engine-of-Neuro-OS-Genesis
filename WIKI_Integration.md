# Integration Guide

## 🔌 How to Integrate DVTRGA

This guide explains how to integrate DVTRGA into your project.

**⚠️ IMPORTANT**: Integration requires a commercial license. Contact the author before proceeding.

---

## 📦 Step 1: Obtain Files

### Windows
Required files:
- `dvtrga_api.dll` (~103 KB)
- `dvtrga_api.h`
- `dvtrga_api.lib` (for linking)

### Linux
Required files:
- `libdvtrga.so`
- `dvtrga_linux.h`

---

## 🔧 Step 2: Project Setup

### Visual Studio (Windows)

1. **Copy files to your project**:
   ```
   YourProject/
   ├── include/
   │   └── dvtrga_api.h
   ├── lib/
   │   └── dvtrga_api.lib
   └── bin/
       └── dvtrga_api.dll
   ```

2. **Configure project settings**:
   - **C/C++** → **General** → **Additional Include Directories**: `$(ProjectDir)include`
   - **Linker** → **General** → **Additional Library Directories**: `$(ProjectDir)lib`
   - **Linker** → **Input** → **Additional Dependencies**: `dvtrga_api.lib`

3. **Copy DLL to output directory**:
   - **Build Events** → **Post-Build Event**:
     ```cmd
     copy "$(ProjectDir)bin\dvtrga_api.dll" "$(OutDir)"
     ```

### GCC/MinGW (Windows)

```bash
gcc your_app.c -o your_app.exe \
    -I./include \
    -L./lib \
    -ldvtrga_api
```

### GCC (Linux)

```bash
gcc your_app.c -o your_app \
    -I./include \
    -L./lib \
    -ldvtrga \
    -lSDL2 \
    -Wl,-rpath,./lib
```

---

## 💻 Step 3: Initialize DVTRGA

### Basic Initialization

```c
#include "dvtrga_api.h"

int main() {
    // Initialize with desired resolution
    if (dvtrga_init(1920, 1080, 32) != 0) {
        fprintf(stderr, "Failed to initialize DVTRGA\n");
        return 1;
    }
    
    // Your rendering loop here
    
    // Cleanup
    dvtrga_shutdown();
    return 0;
}
```

### With Error Handling

```c
#include "dvtrga_api.h"
#include <stdio.h>

int main() {
    // Initialize
    int result = dvtrga_init(1920, 1080, 32);
    if (result != 0) {
        fprintf(stderr, "DVTRGA initialization failed: %d\n", result);
        return 1;
    }
    
    // Get framebuffer
    uint32_t *fb = dvtrga_get_framebuffer();
    if (!fb) {
        fprintf(stderr, "Failed to get framebuffer\n");
        dvtrga_shutdown();
        return 1;
    }
    
    // Rendering code here
    
    dvtrga_shutdown();
    return 0;
}
```

---

## 🎮 Step 4: Rendering Loop

### Simple Loop

```c
while (running) {
    // Clear screen
    dvtrga_clear(0x000000);
    
    // Get framebuffer
    uint32_t *fb = dvtrga_get_framebuffer();
    
    // Draw your graphics
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fb[y * width + x] = calculate_color(x, y);
        }
    }
    
    // Present frame
    dvtrga_blit();
    
    // Handle events (platform-specific)
}
```

### With FPS Limiting

```c
#include <time.h>

const int TARGET_FPS = 60;
const double FRAME_TIME = 1.0 / TARGET_FPS;

while (running) {
    clock_t start = clock();
    
    // Rendering code
    dvtrga_clear(0x000000);
    // ... draw ...
    dvtrga_blit();
    
    // FPS limiting
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    if (elapsed < FRAME_TIME) {
        Sleep((DWORD)((FRAME_TIME - elapsed) * 1000));
    }
}
```

---

## 🔗 Step 5: Integration Patterns

### Game Engine Integration

```c
// engine.h
typedef struct {
    uint32_t *framebuffer;
    int width;
    int height;
} GraphicsEngine;

// engine.c
GraphicsEngine* engine_init(int w, int h) {
    GraphicsEngine *engine = malloc(sizeof(GraphicsEngine));
    dvtrga_init(w, h, 32);
    engine->framebuffer = dvtrga_get_framebuffer();
    engine->width = w;
    engine->height = h;
    return engine;
}

void engine_render(GraphicsEngine *engine) {
    // Your rendering logic
    dvtrga_blit();
}

void engine_shutdown(GraphicsEngine *engine) {
    dvtrga_shutdown();
    free(engine);
}
```

### Custom OS Integration

```c
// For custom OS like Neuro-OS Genesis
void os_graphics_init(void) {
    dvtrga_init(SCREEN_WIDTH, SCREEN_HEIGHT, 32);
}

void os_graphics_draw_desktop(void) {
    uint32_t *fb = dvtrga_get_framebuffer();
    // Draw desktop elements
    dvtrga_blit();
}
```

---

## 📋 API Reference

### Initialization
```c
int dvtrga_init(int width, int height, int bpp);
```
- **Returns**: 0 on success, non-zero on error

### Framebuffer Access
```c
uint32_t* dvtrga_get_framebuffer(void);
```
- **Returns**: Pointer to framebuffer (ARGB8888 format)

### Clear Screen
```c
void dvtrga_clear(uint32_t color);
```
- **color**: ARGB8888 format (0xAARRGGBB)

### Present Frame
```c
void dvtrga_blit(void);
```
- Presents the framebuffer to the display

### Cleanup
```c
void dvtrga_shutdown(void);
```
- Releases all resources

---

## ⚠️ Important Considerations

### Thread Safety
- DVTRGA is **not thread-safe** by default
- Use mutexes if accessing from multiple threads

### Memory Management
- Do **not** free the framebuffer pointer
- DVTRGA manages its own memory

### Performance
- Call `dvtrga_blit()` only when frame is complete
- Avoid unnecessary `dvtrga_clear()` calls

---

## 🔒 Licensing

**Commercial integration requires a license.**

Contact the author for:
- Commercial use
- Redistribution
- Integration into products
- Custom modifications

---

**Copyright © 2026 José Manuel Moreno Cano**  
**All rights reserved**
