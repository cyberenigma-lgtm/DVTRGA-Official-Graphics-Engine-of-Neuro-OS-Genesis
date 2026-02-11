/*
 * DVTRGA2 SIGLO22 - "STEEL NOMAD" WORLD RANKING EDITION (v40)
 * Developed by Neuro-OS Genesis Team.
 * Driver Signature: DVTRGA2 SIGLO22
 */

#define WIN32_LEAN_AND_MEAN 1
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// --- GL CONSTANTS ---
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_POINTS 0x0000
#define GL_TRIANGLE_STRIP 0x0005
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPUTE_SHADER 0x91B9
#define GL_SHADER_STORAGE_BUFFER 0x90D2
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_SHADER_STORAGE_BARRIER_BIT 0x00002000
#define GL_PROGRAM_POINT_SIZE 0x8642
#define GL_DYNAMIC_DRAW 0x88E8

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001

typedef char GLchar;
typedef float GLfloat;
typedef unsigned int GLuint;
typedef int GLint;
typedef unsigned int GLenum;
typedef int GLsizei;
typedef unsigned int GLbitfield;
typedef ptrdiff_t GLsizeiptr;

typedef HGLRC(WINAPI *PFNWGLCREATECONTEXT)(HDC);
typedef BOOL(WINAPI *PFNWGLMAKECURRENT)(HDC, HGLRC);
typedef BOOL(WINAPI *PFNWGLDELETECONTEXT)(HGLRC);
typedef PROC(WINAPI *PFNWGLGETPROCADDRESS)(LPCSTR);
typedef HGLRC(WINAPI *PFNWGLCREATECONTEXTATTRIBSARB)(HDC, HGLRC, const int *);
typedef BOOL(WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);

typedef void(WINAPI *PFNGLCLEAR)(GLbitfield mask);
typedef void(WINAPI *PFNGLDRAWARRAYS)(GLenum mode, GLint first, GLsizei count);
typedef void(WINAPI *PFNGLCLEARCOLOR)(float, float, float, float);
typedef void(WINAPI *PFNGLVIEWPORT)(GLint x, GLint y, GLsizei width,
                                    GLsizei height);
typedef void(WINAPI *PFNGLUSEPROGRAM)(GLuint program);
typedef void(WINAPI *PFNGLDISPATCHCOMPUTE)(GLuint x, GLuint y, GLuint z);
typedef void(WINAPI *PFNGLGENBUFFERS)(GLsizei n, GLuint *buffers);
typedef void(WINAPI *PFNGLBINDBUFFER)(GLenum target, GLuint buffer);
typedef void(WINAPI *PFNGLBINDBUFFERBASE)(GLenum target, GLuint index,
                                          GLuint buffer);
typedef GLuint(WINAPI *PFNGLCREATESHADER)(GLenum type);
typedef void(WINAPI *PFNGLSHADERSOURCE)(GLuint shader, GLsizei count,
                                        const GLchar *const *string,
                                        const GLint *length);
typedef void(WINAPI *PFNGLCOMPILESHADER)(GLuint shader);
typedef GLuint(WINAPI *PFNGLCREATEPROGRAM)(void);
typedef void(WINAPI *PFNGLATTACHSHADER)(GLuint program, GLuint shader);
typedef void(WINAPI *PFNGLLINKPROGRAM)(GLuint program);
typedef GLint(WINAPI *PFNGLGETUNIFORMLOCATION)(GLuint program,
                                               const GLchar *name);
typedef void(WINAPI *PFNGLUNIFORM1I)(GLuint location, int v0);
typedef void(WINAPI *PFNGLUNIFORM2F)(GLuint location, float v0, float v1);
typedef void(WINAPI *PFNGLUNIFORM4F)(GLuint location, float v0, float v1,
                                     float v2, float v3);
typedef void(WINAPI *PFNGLMEMORYBARRIER)(GLbitfield barriers);
typedef void(WINAPI *PFNGLENABLE)(GLenum cap);
typedef void(WINAPI *PFNGLDISABLE)(GLenum cap);
typedef void(WINAPI *PFNGLGENVERTEXARRAYS)(GLsizei n, GLuint *arrays);
typedef void(WINAPI *PFNGLBINDVERTEXARRAY)(GLuint array);

static HMODULE hGL = NULL;
static PFNWGLGETPROCADDRESS g_wglGPA = NULL;
PROC fetch(const char *n) {
  PROC p = g_wglGPA ? g_wglGPA(n) : NULL;
  if (!p || (p == (PROC)1) || (p == (PROC)-1))
    p = GetProcAddress(hGL, n);
  return p;
}

PFNGLCLEAR ptr_glClear;
PFNGLDRAWARRAYS ptr_glDrawArrays;
PFNGLCLEARCOLOR ptr_glClearColor;
PFNGLVIEWPORT ptr_glViewport;
PFNGLUSEPROGRAM ptr_glUseProgram;
PFNGLDISPATCHCOMPUTE ptr_glDispatchCompute;
PFNGLGENBUFFERS ptr_glGenBuffers;
PFNGLBINDBUFFER ptr_glBindBuffer;
PFNGLBINDBUFFERBASE ptr_glBindBufferBase;
PFNGLCREATESHADER ptr_glCreateShader;
PFNGLSHADERSOURCE ptr_glShaderSource;
PFNGLCOMPILESHADER ptr_glCompileShader;
PFNGLCREATEPROGRAM ptr_glCreateProgram;
PFNGLATTACHSHADER ptr_glAttachShader;
PFNGLLINKPROGRAM ptr_glLinkProgram;
PFNGLGETUNIFORMLOCATION ptr_glGetUniformLocation;
PFNGLUNIFORM1I ptr_glUniform1i;
PFNGLUNIFORM2F ptr_glUniform2f;
PFNGLUNIFORM4F ptr_glUniform4f;
PFNGLMEMORYBARRIER ptr_glMemoryBarrier;
PFNGLENABLE ptr_glEnable;
PFNGLDISABLE ptr_glDisable;
PFNGLGENVERTEXARRAYS ptr_glGenVertexArrays;
PFNGLBINDVERTEXARRAY ptr_glBindVertexArray;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

const char *K_I =
    "#version 430 core\n"
    "layout(local_size_x=256) in; layout(std430, binding=0) buffer P { vec2 "
    "p[]; };\n"
    "layout(std430, binding=1) buffer V { vec2 v[]; }; uniform vec2 s_s;\n"
    "uint has(uint x) { x=((x>>16)^x)*0x45d9f3b; x=((x>>16)^x)*0x45d9f3b; "
    "return (x>>16)^x; }\n"
    "void main() { uint id=gl_GlobalInvocationID.x; if(id>=p.length()) "
    "return;\n"
    "uint h=has(id); p[id]=vec2(float(h%uint(s_s.x)), "
    "float((h/1000)%uint(s_s.y)));\n"
    "v[id]=vec2((float(h%100)-50.0)/400.0, 0.1+float(h%10)/30.0); }";

const char *K_C =
    "#version 430 core\n"
    "layout(local_size_x=256) in; \n"
    "layout(std430, binding=0) buffer restrict PI { vec2 pi[]; }; "
    "layout(std430, binding=1) buffer restrict PO { vec2 po[]; };\n"
    "layout(std430, binding=2) buffer restrict V { vec2 v[]; }; uniform vec2 "
    "s_s;\n"
    "void main() { uint id=gl_GlobalInvocationID.x; if(id>=pi.length()) "
    "return;\n"
    "vec2 p=pi[id]+v[id]; if(p.y>s_s.y) p.y=0; po[id]=p; }";

const char *K_V =
    "#version 430 core\n"
    "layout(std430, binding=0) buffer P { vec2 p[]; }; uniform vec2 s_s;\n"
    "void main() { vec2 pos=p[gl_VertexID]; \n"
    "gl_Position=vec4((pos.x/s_s.x)*2.0-1.0, 1.0-(pos.y/s_s.y)*2.0, 0, 1); "
    "gl_PointSize=1.0; }";

const char *K_F =
    "#version 430 core\nout vec4 o; void main() { o=vec4(1,1,1,1); }";

const char *KH_V =
    "#version 430 core\n"
    "const vec2 v[4]=vec2[](vec2(-1,-1), vec2(1,-1), vec2(-1,1), vec2(1,1));\n"
    "void main() { gl_Position=vec4(v[gl_VertexID],0,1); }";

const char *KH_F =
    "#version 430 core\n"
    "out vec4 o; uniform vec2 s_s; uniform vec4 data; \n"
    "int "
    "n[15]=int[](31599,18738,29671,31207,18925,31183,31695,18727,31727,31215,"
    "31183,18738,31591,18727,31599);\n"
    "float dr(vec2 p, int v) {\n"
    "  p.y = 5.0 - p.y; \n"
    "  if(p.x<0.0||p.x>2.9||p.y<0.0||p.y>4.9) return 0.0;\n"
    "  int b=n[abs(v)%15]; return (b&(1<<(int(p.x)+int(p.y)*3)))!=0?1.0:0.0;\n"
    "}\n"
    "void main() {\n"
    "  vec2 coord = gl_FragCoord.xy; o=vec4(0);\n"
    "  if(coord.x < 1200 && coord.y < 800) {\n"
    "    vec2 p = (coord - vec2(100,100)) / 28.0;\n"
    "    int fps=int(data.x), knt=int(data.z)/1000, pps=int((data.x * "
    "data.z)/1e6);\n"
    "    // Row 2: FPS (Cyan)\n"
    "    o.gb += dr(p - vec2(0,14), fps/100);\n"
    "    o.gb += dr(p - vec2(4,14), (fps/10)%10);\n"
    "    o.gb += dr(p - vec2(8,14), fps%10);\n"
    "    // Row 1: COUNT K (Magenta)\n"
    "    o.rb += dr(p - vec2(0,7), knt/1000);\n"
    "    o.rb += dr(p - vec2(4,7), (knt/100)%10);\n"
    "    o.rb += dr(p - vec2(8,7), (knt/10)%10);\n"
    "    o.rb += dr(p - vec2(12,7), knt%10);\n"
    "    // Row 0: PPS (White)\n"
    "    o.rgb += dr(p, pps/100);\n"
    "    o.rgb += dr(p - vec2(4,0), (pps/10)%10);\n"
    "    o.rgb += dr(p - vec2(8,0), pps%10);\n"
    "    // Branding: SIGLO22 (Yellow)\n"
    "    o.rg += dr(p - vec2(18,7), 10); // S\n"
    "    o.rg += dr(p - vec2(22,7), 11); // I\n"
    "    o.rg += dr(p - vec2(26,7), 12); // G\n"
    "    o.rg += dr(p - vec2(30,7), 13); // L\n"
    "    o.rg += dr(p - vec2(34,7), 14); // O\n"
    "    o.rg += dr(p - vec2(40,7), 2); // 2\n"
    "    o.rg += dr(p - vec2(44,7), 2); // 2\n"
    "  }\n"
    "  if(length(o.rgb) < 0.1) discard;\n"
    "}";

GLuint b_s(GLenum t, const char *s) {
  GLuint h = ptr_glCreateShader(t);
  ptr_glShaderSource(h, 1, &s, NULL);
  ptr_glCompileShader(h);
  return h;
}
GLuint l_p(GLuint v, GLuint f) {
  GLuint p = ptr_glCreateProgram();
  ptr_glAttachShader(p, v);
  if (f)
    ptr_glAttachShader(p, f);
  ptr_glLinkProgram(p);
  return p;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  if (msg == WM_DESTROY)
    PostQuitMessage(0);
  return DefWindowProcA(hWnd, msg, wParam, lParam);
}

int main() {
  printf("[DVTRGA2 SIGLO22] Initializing World Ranking Edition.\n");
  fflush(stdout);

  hGL = LoadLibraryA("opengl32.dll");
  g_wglGPA = (PFNWGLGETPROCADDRESS)GetProcAddress(hGL, "wglGetProcAddress");
  PFNWGLCREATECONTEXT wglCC =
      (PFNWGLCREATECONTEXT)GetProcAddress(hGL, "wglCreateContext");
  PFNWGLMAKECURRENT wglMC =
      (PFNWGLMAKECURRENT)GetProcAddress(hGL, "wglMakeCurrent");
  PFNWGLDELETECONTEXT wglDC =
      (PFNWGLDELETECONTEXT)GetProcAddress(hGL, "wglDeleteContext");

  WNDCLASSA wc = {0};
  wc.lpfnWndProc = WndProc;
  wc.hInstance = GetModuleHandle(NULL);
  wc.lpszClassName = "Siglo22";
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  RegisterClassA(&wc);
  HWND dw = CreateWindowExA(0, "Siglo22", "D", 0, 0, 0, 1, 1, NULL, NULL,
                            wc.hInstance, NULL);
  HDC dc = GetDC(dw);
  PIXELFORMATDESCRIPTOR pfd = {sizeof(pfd),
                               1,
                               PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
                                   PFD_DOUBLEBUFFER,
                               PFD_TYPE_RGBA,
                               32,
                               24,
                               8};
  SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd);
  HGLRC rc = wglCC(dc);
  wglMC(dc, rc);
  PFNWGLCREATECONTEXTATTRIBSARB wglCCAA =
      (PFNWGLCREATECONTEXTATTRIBSARB)fetch("wglCreateContextAttribsARB");
  wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)fetch("wglSwapIntervalEXT");
  wglMC(NULL, NULL);
  wglDC(rc);
  DestroyWindow(dw);

  int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN);
  HWND hWnd = CreateWindowExA(WS_EX_TOPMOST, "Siglo22", "DVTRGA2 SIGLO22",
                              WS_POPUP | WS_VISIBLE, 0, 0, sw, sh, NULL, NULL,
                              wc.hInstance, NULL);
  HDC hDC = GetDC(hWnd);
  SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
  int attr[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,
                4,
                WGL_CONTEXT_MINOR_VERSION_ARB,
                3,
                WGL_CONTEXT_PROFILE_MASK_ARB,
                WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                0};
  HGLRC hRC = wglCCAA(hDC, NULL, attr);
  wglMC(hDC, hRC);
  if (wglSwapIntervalEXT)
    wglSwapIntervalEXT(0);

#define L_F(v, n, t) v = (t)fetch(n);
  L_F(ptr_glClear, "glClear", PFNGLCLEAR);
  L_F(ptr_glDrawArrays, "glDrawArrays", PFNGLDRAWARRAYS);
  L_F(ptr_glClearColor, "glClearColor", PFNGLCLEARCOLOR);
  L_F(ptr_glViewport, "glViewport", PFNGLVIEWPORT);
  L_F(ptr_glUseProgram, "glUseProgram", PFNGLUSEPROGRAM);
  L_F(ptr_glDispatchCompute, "glDispatchCompute", PFNGLDISPATCHCOMPUTE);
  L_F(ptr_glGenBuffers, "glGenBuffers", PFNGLGENBUFFERS);
  L_F(ptr_glBindBuffer, "glBindBuffer", PFNGLBINDBUFFER);
  L_F(ptr_glBindBufferBase, "glBindBufferBase", PFNGLBINDBUFFERBASE);
  L_F(ptr_glCreateShader, "glCreateShader", PFNGLCREATESHADER);
  L_F(ptr_glShaderSource, "glShaderSource", PFNGLSHADERSOURCE);
  L_F(ptr_glCompileShader, "glCompileShader", PFNGLCOMPILESHADER);
  L_F(ptr_glCreateProgram, "glCreateProgram", PFNGLCREATEPROGRAM);
  L_F(ptr_glAttachShader, "glAttachShader", PFNGLATTACHSHADER);
  L_F(ptr_glLinkProgram, "glLinkProgram", PFNGLLINKPROGRAM);
  L_F(ptr_glGetUniformLocation, "glGetUniformLocation",
      PFNGLGETUNIFORMLOCATION);
  L_F(ptr_glUniform1i, "glUniform1i", PFNGLUNIFORM1I);
  L_F(ptr_glUniform2f, "glUniform2f", PFNGLUNIFORM2F);
  L_F(ptr_glUniform4f, "glUniform4f", PFNGLUNIFORM4F);
  L_F(ptr_glMemoryBarrier, "glMemoryBarrier", PFNGLMEMORYBARRIER);
  L_F(ptr_glEnable, "glEnable", PFNGLENABLE);
  L_F(ptr_glDisable, "glDisable", PFNGLDISABLE);
  L_F(ptr_glGenVertexArrays, "glGenVertexArrays", PFNGLGENVERTEXARRAYS);
  L_F(ptr_glBindVertexArray, "glBindVertexArray", PFNGLBINDVERTEXARRAY);

  typedef void(WINAPI * PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size,
                                             const void *data, GLenum usage);
  PFNGLBUFFERDATAPROC ptr_glBufferData =
      (PFNGLBUFFERDATAPROC)fetch("glBufferData");

  GLuint pI = l_p(b_s(GL_COMPUTE_SHADER, K_I), 0),
         pC = l_p(b_s(GL_COMPUTE_SHADER, K_C), 0),
         pR = l_p(b_s(GL_VERTEX_SHADER, K_V), b_s(GL_FRAGMENT_SHADER, K_F));
  GLuint pH = l_p(b_s(GL_VERTEX_SHADER, KH_V), b_s(GL_FRAGMENT_SHADER, KH_F));

  GLuint vao;
  ptr_glGenVertexArrays(1, &vao);
  ptr_glBindVertexArray(vao);
  long long count = 1000000;
  GLuint bP[2], bV;
  ptr_glGenBuffers(2, bP);
  ptr_glGenBuffers(1, &bV);
  for (int i = 0; i < 2; i++) {
    ptr_glBindBuffer(GL_SHADER_STORAGE_BUFFER, bP[i]);
    ptr_glBufferData(GL_SHADER_STORAGE_BUFFER, 10000000 * 8, NULL,
                     GL_DYNAMIC_DRAW);
  }
  ptr_glBindBuffer(GL_SHADER_STORAGE_BUFFER, bV);
  ptr_glBufferData(GL_SHADER_STORAGE_BUFFER, 10000000 * 8, NULL,
                   GL_DYNAMIC_DRAW);

  ptr_glUseProgram(pI);
  ptr_glUniform2f(ptr_glGetUniformLocation(pI, "s_s"), (float)sw, (float)sh);
  ptr_glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bP[0]);
  ptr_glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, bV);
  ptr_glDispatchCompute(10000000 / 256 + 1, 1, 1);
  ptr_glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  ptr_glViewport(0, 0, sw, sh);
  ptr_glEnable(GL_PROGRAM_POINT_SIZE);
  ptr_glDisable(0x0B71);

  LARGE_INTEGER fq, s, e;
  QueryPerformanceFrequency(&fq);
  QueryPerformanceCounter(&s);
  long frames = 0;
  int ping = 0;
  float cur_fps = 120.0f;
  while (1) {
    MSG m;
    while (PeekMessage(&m, 0, 0, 0, PM_REMOVE)) {
      TranslateMessage(&m);
      DispatchMessage(&m);
    }
    int pong = 1 - ping;
    ptr_glUseProgram(pC);
    ptr_glUniform2f(ptr_glGetUniformLocation(pC, "s_s"), (float)sw, (float)sh);
    ptr_glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bP[ping]);
    ptr_glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, bP[pong]);
    ptr_glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, bV);
    ptr_glDispatchCompute((GLuint)((count + 255) / 256), 1, 1);
    ptr_glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    ptr_glClearColor(0, 0, 0, 1);
    ptr_glClear(GL_COLOR_BUFFER_BIT);
    ptr_glUseProgram(pR);
    ptr_glUniform2f(ptr_glGetUniformLocation(pR, "s_s"), (float)sw, (float)sh);
    ptr_glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bP[pong]);
    ptr_glDrawArrays(GL_POINTS, 0, (GLsizei)count);

    ptr_glUseProgram(pH);
    ptr_glUniform2f(ptr_glGetUniformLocation(pH, "s_s"), (float)sw, (float)sh);
    ptr_glUniform4f(ptr_glGetUniformLocation(pH, "data"), cur_fps, 0,
                    (float)count, 0);
    ptr_glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    SwapBuffers(hDC);
    frames++;
    ping = pong;
    if (frames % 60 == 0) {
      QueryPerformanceCounter(&e);
      double el = ((double)(e.QuadPart - s.QuadPart) / fq.QuadPart);
      if (el > 0)
        cur_fps = (float)(60.0 / el);
      printf("[SIGLO22] FPS: %.1f | P: %lld | PPS: %.1fM\n", cur_fps, count,
             (cur_fps * count) / 1e6);
      fflush(stdout);
      s = e;
    }
    if (GetAsyncKeyState('1'))
      count = 10000000;
    if (GetAsyncKeyState('2'))
      count = 5000000;
    if (GetAsyncKeyState('3'))
      count = 1000000;
    if (GetAsyncKeyState('4'))
      count = 500000;
    if (GetAsyncKeyState('5'))
      count = 250000;
    if (GetAsyncKeyState(VK_ESCAPE))
      break;
  }
  return 0;
}
