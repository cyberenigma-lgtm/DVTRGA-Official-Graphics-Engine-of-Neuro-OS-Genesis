@echo off
REM Compilar DVTRGA para Windows
REM Requiere Visual Studio o Build Tools instalado

echo ========================================
echo DVTRGA - Compilador para Windows
echo ========================================
echo.

REM Verificar si cl.exe esta disponible
where cl.exe >nul 2>&1
if %errorLevel% neq 0 (
    echo [!] ERROR: Compilador de Visual Studio no encontrado
    echo.
    echo Opciones:
    echo 1. Instalar Visual Studio 2022 Community
    echo 2. Instalar Build Tools for Visual Studio
    echo 3. Ejecutar desde "Developer Command Prompt for VS"
    echo.
    pause
    exit /b 1
)

echo [1/4] Compilando DLL (dvtrga_api.dll)...
cl /LD /O2 dvtrga_api.c /Fe:dvtrga_api.dll user32.lib gdi32.lib
if %errorLevel% neq 0 (
    echo [X] Error compilando DLL
    pause
    exit /b 1
)
echo [OK] DLL compilada
echo.

echo [2/4] Compilando aplicacion de prueba...
cl /O2 dvtrga_test.c dvtrga_api.lib user32.lib gdi32.lib
if %errorLevel% neq 0 (
    echo [X] Error compilando aplicacion
    pause
    exit /b 1
)
echo [OK] Aplicacion compilada
echo.

echo [3/4] Limpiando archivos temporales...
del *.obj >nul 2>&1
del *.exp >nul 2>&1
echo [OK] Limpieza completada
echo.

echo [4/4] Verificando archivos generados...
if exist "dvtrga_api.dll" (
    echo [OK] dvtrga_api.dll
) else (
    echo [X] dvtrga_api.dll - FALTA
)

if exist "dvtrga_test.exe" (
    echo [OK] dvtrga_test.exe
) else (
    echo [X] dvtrga_test.exe - FALTA
)
echo.

echo ========================================
echo Compilacion completada
echo ========================================
echo.
echo Archivos generados:
echo   - dvtrga_api.dll (biblioteca)
echo   - dvtrga_api.lib (import library)
echo   - dvtrga_test.exe (aplicacion de prueba)
echo.
echo Para probar:
echo   dvtrga_test.exe
echo.
pause
