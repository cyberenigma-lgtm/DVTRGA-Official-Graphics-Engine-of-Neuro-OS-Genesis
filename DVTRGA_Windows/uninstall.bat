@echo off
REM DVTRGA Uninstaller Script for Windows 11
REM Ejecutar como Administrador

echo ========================================
echo DVTRGA Driver Uninstaller v1.0
echo ========================================
echo.

REM Verificar permisos de administrador
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo ERROR: Este script requiere permisos de administrador
    pause
    exit /b 1
)

echo [1/4] Deteniendo servicio...
sc stop DVTRGA >nul 2>&1

echo [2/4] Eliminando servicio...
sc delete DVTRGA >nul 2>&1

echo [3/4] Desinstalando driver...
pnputil /delete-driver dvtrga.inf /uninstall /force >nul 2>&1

echo [4/4] Eliminando archivos...
del /F /Q "C:\Windows\System32\drivers\dvtrga.sys" >nul 2>&1
del /F /Q "C:\Windows\System32\dvtrga_api.dll" >nul 2>&1
rd /S /Q "C:\Program Files\DVTRGA" >nul 2>&1

REM Eliminar entradas del registro
reg delete "HKLM\SOFTWARE\Neuro-OS\DVTRGA" /f >nul 2>&1

echo.
echo ========================================
echo Desinstalacion completada
echo ========================================
echo.
pause
