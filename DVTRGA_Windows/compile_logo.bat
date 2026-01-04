@echo off
REM Compilar demo del logo de Neuro-OS

set MSVC_PATH=C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717
set SDK_PATH=C:\Program Files (x86)\Windows Kits\10
set SDK_VER=10.0.22621.0

echo ========================================
echo NEURO-OS LOGO - Compilacion
echo ========================================
echo.

echo Compilando neuro_logo.exe...
"%MSVC_PATH%\bin\Hostx64\x64\cl.exe" /O2 ^
  /I"%MSVC_PATH%\include" ^
  /I"%SDK_PATH%\Include\%SDK_VER%\um" ^
  /I"%SDK_PATH%\Include\%SDK_VER%\shared" ^
  /I"%SDK_PATH%\Include\%SDK_VER%\ucrt" ^
  neuro_logo.c dvtrga_api.lib ^
  /link ^
  /LIBPATH:"%MSVC_PATH%\lib\x64" ^
  /LIBPATH:"%SDK_PATH%\Lib\%SDK_VER%\um\x64" ^
  /LIBPATH:"%SDK_PATH%\Lib\%SDK_VER%\ucrt\x64" ^
  user32.lib gdi32.lib

if %ERRORLEVEL% NEQ 0 (
    echo [X] Error compilando
    pause
    exit /b 1
)

echo [OK] Compilado exitosamente
echo.
echo ========================================
echo Ejecutando demo...
echo ========================================
echo.

neuro_logo.exe

echo.
pause
