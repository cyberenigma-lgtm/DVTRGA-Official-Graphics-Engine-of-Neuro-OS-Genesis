#define WIN32_LEAN_AND_MEAN 1
#include <stdio.h>
#include <windows.h>


// Manual Fixes for Broken Windows SDK Headers
#ifndef WINGDIAPI
#define WINGDIAPI __declspec(dllimport)
#endif
#ifndef APIENTRY
#define APIENTRY __stdcall
#endif

#include <GL/gl.h>

int main() {
  WNDCLASSA wc = {0};
  wc.style = CS_OWNDC;
  wc.lpfnWndProc = DefWindowProcA;
  wc.hInstance = GetModuleHandle(NULL);
  wc.lpszClassName = "GPU_Info_Class";
  RegisterClassA(&wc);

  HWND hWnd = CreateWindowExA(0, "GPU_Info_Class", "GPU Info", WS_POPUP, 0, 0,
                              100, 100, NULL, NULL, wc.hInstance, NULL);
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
  HGLRC hRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, hRC);

  const GLubyte *vendor = glGetString(GL_VENDOR);
  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *version = glGetString(GL_VERSION);

  char msg[1024];
  sprintf(msg, "Vendor: %s\nRenderer: %s\nVersion: %s\n", vendor, renderer,
          version);
  MessageBoxA(NULL, msg, "GPU Information", MB_OK);

  FILE *f = fopen("gpu_info.txt", "w");
  if (f) {
    fprintf(f, "%s", msg);
    fclose(f);
  }

  return 0;
}
