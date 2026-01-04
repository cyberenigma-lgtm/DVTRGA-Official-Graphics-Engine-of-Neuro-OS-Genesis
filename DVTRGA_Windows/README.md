# DVTRGA Windows

**High-Performance Graphics Driver for Windows**

Part of the [Neuro-OS Genesis](https://github.com/your-org/Neuro-OS-Genesis) ecosystem.

---

## 📋 Overview

DVTRGA Windows provides native Windows graphics acceleration with DLL support for easy integration into Windows applications. Optimized for DirectX compatibility and high-performance rendering.

---

## 🏛️ Intellectual Property

**Part of Neuro-OS Genesis Ecosystem**

- **Owner**: José Manuel Moreno Cano
- **ID**: Verified Author (Neuro-OS Genesis)
- **Registration**: January 1, 2026
- **Jurisdiction**: Spain (ES)

**Note**: Verification credentials are stored securely and are not publicly disclosed.

---

## 🎯 Supported Platforms

- ✅ **Windows 10/11** (x64)
- ✅ **Windows Server 2019+**
- ✅ **Visual Studio 2017+**
- ✅ **MinGW-w64**

---

## 📦 Installation

### Prerequisites

**Visual Studio**:
- Visual Studio 2017 or later
- Windows SDK 10.0+
- C/C++ build tools

**MinGW**:
```bash
# Install MinGW-w64
choco install mingw
```

**Python (Optional)**:
```bash
pip install ctypes pillow
```

---

## 🔨 Compilation

### Visual Studio 2017/2019

```cmd
compile_vs18.bat
```

**Output**:
- `dvtrga_api.dll` - Main library
- `dvtrga_api.lib` - Import library
- `dvtrga_test.exe` - Test executable

### Visual Studio (Manual)

```cmd
cl /c dvtrga_api.c /LD
link /DLL /OUT:dvtrga_api.dll dvtrga_api.obj user32.lib gdi32.lib
cl dvtrga_test.c dvtrga_api.lib
```

### MinGW-w64

```bash
./compile_mingw.sh
```

Or manually:
```bash
gcc -c dvtrga_api.c -o dvtrga_api.o
gcc -shared -o dvtrga_api.dll dvtrga_api.o -lgdi32 -luser32
gcc dvtrga_test.c -o dvtrga_test.exe -L. -ldvtrga_api
```

### GCC (WSL/Cygwin)

```bash
./compile_gcc.sh
```

---

## 🚀 Usage

### C/C++ Application

```c
#include "dvtrga_api.h"

int main() {
    // Initialize DVTRGA
    dvtrga_init(1920, 1080, 32);
    
    // Get framebuffer
    uint32_t *fb = dvtrga_get_framebuffer();
    
    // Draw a red pixel at (100, 100)
    fb[100 * 1920 + 100] = 0xFF0000;
    
    // Present frame
    dvtrga_blit();
    
    // Cleanup
    dvtrga_shutdown();
    return 0;
}
```

**Compile**:
```cmd
cl your_app.c dvtrga_api.lib
```

### Python Application

```python
from dvtrga_api import DVTRGA

# Initialize
dvtrga = DVTRGA(1920, 1080)

# Get framebuffer
fb = dvtrga.get_framebuffer()

# Draw red pixel at (100, 100)
fb[100 * 1920 + 100] = 0xFF0000

# Present
dvtrga.blit()

# Cleanup
dvtrga.shutdown()
```

**Run**:
```cmd
python your_app.py
```

---

## 🧪 Testing

### Run Test Suite

```cmd
test.bat
```

Or manually:
```cmd
dvtrga_test.exe
```

**Expected Output**:
- Window opens at 1920x1080
- Color gradient test
- FPS counter display
- Screenshot saved to `dvtrga_output.bmp`

### Python Test

```cmd
python dvtrga_test.py
```

---

## 📊 Performance

**Benchmark Results** (Windows 11, Intel i7):
- **Average FPS**: 600+
- **Frame Time**: ~1.6ms
- **Memory**: 8MB (1920x1080x32bit)
- **DLL Size**: ~100KB

---

## 🔧 Installation as System Driver

### Install Driver

```cmd
install.bat
```

This will:
1. Copy `dvtrga_api.dll` to `C:\Windows\System32\`
2. Register the driver in Windows Registry
3. Create system restore point

### Uninstall Driver

```cmd
uninstall.bat
```

---

## 🔧 Troubleshooting

### DLL Not Found

```cmd
# Add to PATH
set PATH=%PATH%;C:\path\to\dvtrga

# Or copy to System32
copy dvtrga_api.dll C:\Windows\System32\
```

### Missing Dependencies

```cmd
# Install Visual C++ Redistributable
# Download from Microsoft
```

### Python ctypes Error

```cmd
pip install --upgrade ctypes
```

---

## 📚 API Reference

### Core Functions

| Function | Description |
|----------|-------------|
| `dvtrga_init(w, h, bpp)` | Initialize graphics context |
| `dvtrga_shutdown()` | Release resources |
| `dvtrga_get_framebuffer()` | Get direct framebuffer pointer |
| `dvtrga_clear(color)` | Clear screen to color |
| `dvtrga_blit()` | Present framebuffer to display |
| `dvtrga_save_bmp(path)` | Save framebuffer to BMP file |

---

## 🌍 Use Cases

- **Windows Applications**: Native graphics acceleration
- **Game Engines**: Lightweight rendering backend
- **Scientific Visualization**: High-performance plotting
- **Digital Art**: Real-time pixel manipulation

---

## 📄 License

Proprietary - Neuro-OS Genesis Ecosystem  
Copyright © 2026 José Manuel Moreno Cano

For licensing inquiries, contact the author.

---

## 🔗 Related Projects

- [DVTRGA Multiplataforma](https://github.com/your-org/DVTRGA_Multiplataforma) - Linux/Android
- [DVTRGA Universal](https://github.com/your-org/DVTRGA_Universal) - Console support
- [Neuro-OS Genesis](https://github.com/your-org/Neuro-OS-Genesis) - Full OS

---

🤜⚖️🔱🛡️ **Neuro-OS Genesis - Siglo 22** 🛡️🔱⚖️🤜
