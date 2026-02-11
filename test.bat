@echo off
REM DVTRGA - Script de Prueba (sin instalacion real)
REM Este script solo muestra informacion del sistema

echo ========================================
echo DVTRGA Driver - Modo Prueba
echo ========================================
echo.

REM Verificar permisos de administrador
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo ADVERTENCIA: No tienes permisos de administrador
    echo Este es solo un script de prueba
    echo.
)

echo [INFO] Verificando sistema...
echo Sistema operativo: %OS%
echo Arquitectura: %PROCESSOR_ARCHITECTURE%
echo.

echo [INFO] Archivos necesarios para instalacion real:
echo   - dvtrga.sys (driver modo kernel)
echo   - dvtrga.inf (archivo de instalacion)
echo   - dvtrga_api.dll (biblioteca user-mode)
echo   - dvtrga_test.exe (aplicacion de prueba)
echo.

echo [INFO] Estado actual:
if exist "dvtrga.sys" (
    echo   [OK] dvtrga.sys encontrado
) else (
    echo   [X] dvtrga.sys NO encontrado - necesita compilacion
)

if exist "dvtrga.inf" (
    echo   [OK] dvtrga.inf encontrado
) else (
    echo   [X] dvtrga.inf NO encontrado - necesita creacion
)

if exist "dvtrga_api.dll" (
    echo   [OK] dvtrga_api.dll encontrado
) else (
    echo   [X] dvtrga_api.dll NO encontrado - necesita compilacion
)

if exist "dvtrga_test.exe" (
    echo   [OK] dvtrga_test.exe encontrado
) else (
    echo   [X] dvtrga_test.exe NO encontrado - necesita compilacion
)

echo.
echo ========================================
echo NOTA: Este es un script de prueba
echo ========================================
echo.
echo Para instalar DVTRGA realmente, necesitas:
echo 1. Compilar el driver con WDK (Windows Driver Kit)
echo 2. Firmar el driver con un certificado valido
echo 3. Crear los archivos .sys, .inf, .dll y .exe
echo 4. Ejecutar install.bat como administrador
echo.
echo Por ahora, DVTRGA esta funcionando en Neuro-OS
echo (ver kernel compilado en ../kernel/build/)
echo.
pause
