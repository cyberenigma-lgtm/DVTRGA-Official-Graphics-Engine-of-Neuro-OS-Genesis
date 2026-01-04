# DVTRGA Multiplataforma

**Cross-Platform Graphics Driver - Linux & Android Support**

Part of the [Neuro-OS Genesis](https://github.com/your-org/Neuro-OS-Genesis) ecosystem.

---

## 📋 Overview

DVTRGA Multiplataforma provides cross-platform graphics acceleration for Linux and Android systems. This implementation focuses on portability and performance across different Unix-like operating systems.

---

## 🏛️ Intellectual Property

**Part of Neuro-OS Genesis Ecosystem**

- **Owner**: José Manuel Moreno Cano
- **ID**: DNI 77453214-R
- **Registration**: January 1, 2026
- **Jurisdiction**: Spain (ES)

**Note**: Verification credentials are stored securely and are not publicly disclosed.

---

## 🎯 Supported Platforms

- ✅ **Linux** (Ubuntu, Debian, Fedora, Arch)
- ✅ **Android** (NDK required)
- ✅ **Raspberry Pi** (ARM support)

---

## 📦 Installation

### Prerequisites

**Linux**:
```bash
sudo apt-get install build-essential libsdl2-dev
```

**Android**:
```bash
# Install Android NDK
export ANDROID_NDK=/path/to/ndk
```

---

## 🔨 Compilation

### Linux (GCC)

```bash
# Compile the library
chmod +x compile.sh
./compile.sh

# This will generate:
# - libdvtrga.so (shared library)
# - dvtrga_test (test executable)
```

### Linux (Manual)

```bash
# Compile library
gcc -c dvtrga_linux.c -o dvtrga_linux.o -fPIC
gcc -shared -o libdvtrga.so dvtrga_linux.o -lSDL2

# Compile test
gcc dvtrga_test_linux.c -o dvtrga_test -L. -ldvtrga -lSDL2 -Wl,-rpath,.
```

### Android (NDK)

```bash
cd Android
ndk-build
# Output: libs/armeabi-v7a/libdvtrga.so
```

---

## 🚀 Usage

### Basic Example

```c
#include "dvtrga_linux.h"

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

### Compile Your Application

```bash
gcc your_app.c -o your_app -L. -ldvtrga -lSDL2 -Wl,-rpath,.
```

---

## 🧪 Testing

### Run Test Suite

```bash
# After compilation
./dvtrga_test

# Expected output:
# - Window opens at 1920x1080
# - Red, green, blue color test
# - FPS counter display
```

### Performance Test

```bash
# Run with performance monitoring
./dvtrga_test --benchmark

# Expected: 60+ FPS on modern hardware
```

---

## 📊 Performance

**Benchmark Results** (Ubuntu 22.04, Intel i7):
- **Average FPS**: 600+
- **Frame Time**: ~1.6ms
- **Memory**: 8MB (1920x1080x32bit)

---

## 🔧 Troubleshooting

### SDL2 Not Found

```bash
# Ubuntu/Debian
sudo apt-get install libsdl2-dev

# Fedora
sudo dnf install SDL2-devel

# Arch
sudo pacman -S sdl2
```

### Library Not Found at Runtime

```bash
# Add to LD_LIBRARY_PATH
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

# Or install system-wide
sudo cp libdvtrga.so /usr/local/lib/
sudo ldconfig
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

---

## 🌍 Use Cases

- **Embedded Systems**: Raspberry Pi, BeagleBone
- **Linux Gaming**: Lightweight game engines
- **Android Apps**: High-performance graphics
- **IoT Displays**: Digital signage, kiosks

---

## 📄 License

Proprietary - Neuro-OS Genesis Ecosystem  
Copyright © 2026 José Manuel Moreno Cano

For licensing inquiries, contact the author.

---

## 🔗 Related Projects

- [DVTRGA Universal](https://github.com/your-org/DVTRGA_Universal) - Console support
- [DVTRGA Windows](https://github.com/your-org/DVTRGA_Windows) - Windows implementation
- [Neuro-OS Genesis](https://github.com/your-org/Neuro-OS-Genesis) - Full OS

---

🤜⚖️🔱🛡️ **Neuro-OS Genesis - Siglo 22** 🛡️🔱⚖️🤜
