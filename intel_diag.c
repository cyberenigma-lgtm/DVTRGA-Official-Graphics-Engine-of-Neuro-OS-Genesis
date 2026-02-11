#define WIN32_LEAN_AND_MEAN 1
#include <stdio.h>
#include <windows.h>


FILE *g_f = NULL;
void vlog(const char *msg) {
  if (!g_f)
    g_f = fopen("intel_crash_log.txt", "w");
  if (g_f) {
    fprintf(g_f, "%s\n", msg);
    fflush(g_f);
  }
}

typedef HGLRC(WINAPI *PFNWGLCREATECONTEXT)(HDC);
typedef BOOL(WINAPI *PFNWGLMAKECURRENT)(HDC, HGLRC);

int main() {
  vlog("DIAGNOSTIC START");

  vlog("Loading opengl32.dll...");
  HMODULE hGL = LoadLibraryA("opengl32.dll");
  if (!hGL) {
    vlog("FAIL: opengl32.dll not found");
    return 1;
  }
  vlog("SUCCESS: opengl32.dll loaded");

  PFNWGLCREATECONTEXT ptr_wglCreateContext =
      (PFNWGLCREATECONTEXT)GetProcAddress(hGL, "wglCreateContext");
  if (!ptr_wglCreateContext) {
    vlog("FAIL: wglCreateContext not found in DLL");
    return 1;
  }
  vlog("SUCCESS: wglCreateContext pointer acquired");

  WNDCLASSA wc = {0};
  wc.lpfnWndProc = DefWindowProcA;
  wc.hInstance = GetModuleHandle(NULL);
  wc.lpszClassName = "Diag_Class";
  vlog("Registering Window Class...");
  RegisterClassA(&wc);
  vlog("SUCCESS: Class registered");

  vlog("Creating Window...");
  HWND hWnd = CreateWindowExA(0, "Diag_Class", "Intel Diagnostic",
                              WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 400,
                              400, NULL, NULL, wc.hInstance, NULL);
  if (!hWnd) {
    vlog("FAIL: CreateWindow failed");
    return 1;
  }
  vlog("SUCCESS: Window created");

  HDC hDC = GetDC(hWnd);
  vlog("Setting Pixel Format...");
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
  int pf = ChoosePixelFormat(hDC, &pfd);
  if (!pf) {
    vlog("FAIL: ChoosePixelFormat failed");
    return 1;
  }
  if (!SetPixelFormat(hDC, pf, &pfd)) {
    vlog("FAIL: SetPixelFormat failed");
    return 1;
  }
  vlog("SUCCESS: Pixel Format set");

  vlog("Attemping wglCreateContext...");
  HGLRC hRC = ptr_wglCreateContext(hDC);
  if (!hRC) {
    vlog("FAIL: wglCreateContext returned NULL");
    return 1;
  }
  vlog("SUCCESS: Context Created!");

  vlog("Attempting wglMakeCurrent...");
  PFNWGLMAKECURRENT ptr_wglMakeCurrent =
      (PFNWGLMAKECURRENT)GetProcAddress(hGL, "wglMakeCurrent");
  if (!ptr_wglMakeCurrent(hDC, hRC)) {
    vlog("FAIL: wglMakeCurrent failed");
    return 1;
  }
  vlog("SUCCESS: Context is CURRENT");

  vlog("DIAGNOSTIC COMPLETE - SYSTEM IS OPENGL CAPABLE");
  MessageBoxA(NULL, "SISTEMA COMPATIBLE CON OPENGL DINAMICO", "Exito", MB_OK);

  return 0;
}
