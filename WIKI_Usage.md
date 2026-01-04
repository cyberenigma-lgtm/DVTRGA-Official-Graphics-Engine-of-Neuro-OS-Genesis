# Usage Guide

## 🎯 What is DVTRGA For?

DVTRGA is a **high-performance graphics engine** designed for:

- ✅ **Real-time rendering** applications
- ✅ **Game engines** requiring low-level graphics access
- ✅ **Scientific visualization** with high frame rates
- ✅ **Embedded systems** with limited resources
- ✅ **Custom OS development** (like Neuro-OS Genesis)

---

## 📋 Prerequisites

### Windows
- Windows 10/11 (x64)
- Visual Studio 2017+ or MinGW-w64
- Windows SDK 10.0+

### Linux
- GCC 7+
- SDL2 development libraries
- Make or CMake

### Android
- Android NDK
- API Level 21+

---

## 🔨 How to Compile

### Windows (Visual Studio)

```cmd
cd DVTRGA_Windows
compile_vs18.bat
```

**Output**:
- `dvtrga_api.dll` (~103 KB)
- `dvtrga_api.lib`
- `dvtrga_test.exe`

### Windows (MinGW)

```bash
cd DVTRGA_Windows
./compile_mingw.sh
```

### Linux

```bash
cd DVTRGA_Multiplataforma
chmod +x compile.sh
./compile.sh
```

**Output**:
- `libdvtrga.so`
- `dvtrga_test`

---

## ▶️ How to Execute

### Windows

```cmd
cd DVTRGA_Windows
dvtrga_test.exe
```

**Expected behavior**:
1. Window opens at 1920x1080
2. Color gradient test displays
3. FPS counter shows (600+ expected)
4. Screenshot saved to `dvtrga_output.bmp`

### Linux

```bash
cd DVTRGA_Multiplataforma
./dvtrga_test
```

**Expected behavior**:
1. SDL window opens
2. RGB color cycle test
3. FPS counter in console

---

## 💻 Basic Usage Example

### C/C++ Code

```c
#include "dvtrga_api.h"

int main() {
    // Initialize DVTRGA
    dvtrga_init(1920, 1080, 32);
    
    // Get direct framebuffer access
    uint32_t *framebuffer = dvtrga_get_framebuffer();
    
    // Draw a red pixel at (100, 100)
    framebuffer[100 * 1920 + 100] = 0xFF0000;
    
    // Present the frame
    dvtrga_blit();
    
    // Cleanup
    dvtrga_shutdown();
    
    return 0;
}
```

### Compile Your Application

**Windows**:
```cmd
cl your_app.c dvtrga_api.lib
```

**Linux**:
```bash
gcc your_app.c -o your_app -L. -ldvtrga -lSDL2
```

---

## 🎨 Drawing Functions

### Clear Screen
```c
dvtrga_clear(0x000000);  // Black
```

### Draw Pixel
```c
uint32_t *fb = dvtrga_get_framebuffer();
fb[y * width + x] = color;
```

### Draw Rectangle
```c
for (int y = y1; y < y2; y++) {
    for (int x = x1; x < x2; x++) {
        fb[y * width + x] = color;
    }
}
```

---

## 📊 Performance Tips

1. **Direct Framebuffer Access**: Use `dvtrga_get_framebuffer()` for maximum speed
2. **Batch Updates**: Update multiple pixels before calling `dvtrga_blit()`
3. **Avoid Unnecessary Clears**: Only clear when needed
4. **Use Proper Data Types**: `uint32_t` for ARGB8888 format

---

## ⚠️ Important Notes

- **License Required**: Commercial use requires explicit permission
- **No Modifications**: Do not modify the DLL/SO files
- **Reference Only**: This documentation is for evaluation purposes

---

**For commercial licensing, contact the author.**
