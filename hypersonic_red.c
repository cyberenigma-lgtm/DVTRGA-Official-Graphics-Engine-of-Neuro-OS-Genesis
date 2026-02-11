/*
 * HYPERSONIC RED - INTEL iGPU CONTEXT TEST
 * Purely Dynamic. No Shaders. Just Clear to Red.
 */

#define WIN32_LEAN_AND_MEAN 1
#include <stdio.h>
#include <windows.h>


typedef HGLRC(WINAPI *PFNWGLCREATECONTEXT)(HDC);
typedef BOOL(WINAPI *PFNWGLMAKECURRENT)(HDC, HGLRC);
typedef void(WINAPI *PFNGLCLEARCOLOR)(float, float, float, float);
typedef void(WINAPI *PFNGLCLEAR)(unsigned int);

int main() {
  HMODULE hGL = LoadLibraryA("opengl32.dll");
  if (!hGL)
    return 1;

  PFNWGLCREATECONTEXT ptr_wglCreateContext =
      (PFNWGLCREATECONTEXT)GetProcAddress(hGL, "wglCreateContext");
  PFNWGLMAKECURRENT ptr_wglMakeCurrent =
      (PFNWGLMAKECURRENT)GetProcAddress(hGL, "wglMakeCurrent");
  PFNGLCLEARCOLOR ptr_glClearColor =
      (PFNGLCLEARCOLOR)GetProcAddress(hGL, "glClearColor");
  PFNGLCLEAR ptr_glClear = (PFNGLCLEAR)GetProcAddress(hGL, "glClear");

  WNDCLASSA wc = {0};
  wc.lpfnWndProc = DefWindowProcA;
  wc.hInstance = GetModuleHandle(NULL);
  wc.lpszClassName = "Red_Test";
  RegisterClassA(&wc);

  HWND hWnd = CreateWindowExA(0, "Red_Test", "INTEL GPU TEST - SHOULD BE RED",
                              WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 800,
                              600, NULL, NULL, wc.hInstance, NULL);
  HDC hDC = GetDC(hWnd);

  PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR),
                               1,
                               PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
                                   PFD_DOUBLEBUFFER,
                               PFD_TYPE_RGBA,
                               32,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               24,
                               8,
                               0,
                               PFD_MAIN_PLANE,
                               0,
                               0,
                               0,
                               0};
  SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
  HGLRC hRC = ptr_wglCreateContext(hDC);
  ptr_wglMakeCurrent(hDC, hRC);

  if (!hRC) {
    MessageBoxA(NULL, "Failed to create GL Context on Intel iGPU", "Error",
                MB_OK);
    return 1;
  }

  MessageBoxA(NULL, "Context Created. Press OK to attempt Red Clear.",
              "Intel Debug", MB_OK);

  while (1) {
    ptr_glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    ptr_glClear(0x00004000); // GL_COLOR_BUFFER_BIT
    SwapBuffers(hDC);

    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT)
        return 0;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    if (GetAsyncKeyState(VK_ESCAPE))
      break;
  }

  return 0;
}
