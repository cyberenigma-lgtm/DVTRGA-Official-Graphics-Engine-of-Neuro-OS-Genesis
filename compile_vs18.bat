@echo off
REM Script de compilación DVTRGA con rutas completas

set MSVC_PATH=C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717
set SDK_PATH=C:\Program Files (x86)\Windows Kits\10
set SDK_VER=10.0.22621.0

echo ========================================
echo DVTRGA - Compilación con VS 18
echo ========================================
echo.

REM Compilar DLL
echo [1/2] Compilando dvtrga_api.dll...
"%MSVC_PATH%\bin\Hostx64\x64\cl.exe" /LD /O2 ^
  /I"%MSVC_PATH%\include" ^
  /I"%SDK_PATH%\Include\%SDK_VER%\um" ^
  /I"%SDK_PATH%\Include\%SDK_VER%\shared" ^
  /I"%SDK_PATH%\Include\%SDK_VER%\ucrt" ^
  dvtrga_api.c /Fe:dvtrga_api.dll ^
  /link ^
  /LIBPATH:"%MSVC_PATH%\lib\x64" ^
  /LIBPATH:"%SDK_PATH%\Lib\%SDK_VER%\um\x64" ^
  /LIBPATH:"%SDK_PATH%\Lib\%SDK_VER%\ucrt\x64" ^
  user32.lib gdi32.lib

if %ERRORLEVEL% NEQ 0 (
    echo [X] Error compilando DLL
    pause
    exit /b 1
)

echo [OK] DLL compilada
echo.

REM Compilar EXE
echo [2/2] Compilando dvtrga_test.exe...
"%MSVC_PATH%\bin\Hostx64\x64\cl.exe" /O2 ^
  /I"%MSVC_PATH%\include" ^
  /I"%SDK_PATH%\Include\%SDK_VER%\um" ^
  /I"%SDK_PATH%\Include\%SDK_VER%\shared" ^
  /I"%SDK_PATH%\Include\%SDK_VER%\ucrt" ^
  dvtrga_test.c dvtrga_api.lib ^
  /link ^
  /LIBPATH:"%MSVC_PATH%\lib\x64" ^
  /LIBPATH:"%SDK_PATH%\Lib\%SDK_VER%\um\x64" ^
  /LIBPATH:"%SDK_PATH%\Lib\%SDK_VER%\ucrt\x64" ^
  user32.lib gdi32.lib

if %ERRORLEVEL% NEQ 0 (
    echo [X] Error compilando EXE
    pause
    exit /b 1
)

echo [OK] EXE compilado
echo.
echo ========================================
echo [OK] DVTRGA compilado exitosamente
echo ========================================
echo.
echo Archivos generados:
echo   - dvtrga_api.dll
echo   - dvtrga_api.lib
echo   - dvtrga_test.exe
echo.
pause
