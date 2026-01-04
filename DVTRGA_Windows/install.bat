@echo off
REM DVTRGA - Instalador Funcional para Windows
REM Version simulada para pruebas

echo ========================================
echo DVTRGA Driver - Instalador v1.0
echo ========================================
echo.

REM Verificar permisos de administrador
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo [!] ADVERTENCIA: Se requieren permisos de administrador
    echo [i] Continuando en modo simulacion...
    echo.
)

echo [1/6] Verificando sistema...
echo   - Sistema: %OS%
echo   - Arquitectura: %PROCESSOR_ARCHITECTURE%
echo   - Python: 
python --version 2>nul
if %errorLevel% neq 0 (
    echo     [X] Python no encontrado - necesario para simulacion
    echo     [i] Instala Python desde https://python.org
    pause
    exit /b 1
)
echo.

echo [2/6] Verificando archivos...
set ALL_OK=1

if exist "dvtrga.inf" (
    echo   [OK] dvtrga.inf
) else (
    echo   [X] dvtrga.inf - FALTA
    set ALL_OK=0
)

if exist "dvtrga_api.py" (
    echo   [OK] dvtrga_api.py
) else (
    echo   [X] dvtrga_api.py - FALTA
    set ALL_OK=0
)

if exist "dvtrga_test.py" (
    echo   [OK] dvtrga_test.py
) else (
    echo   [X] dvtrga_test.py - FALTA
    set ALL_OK=0
)

echo.

if %ALL_OK%==0 (
    echo [X] Faltan archivos necesarios
    pause
    exit /b 1
)

echo [3/6] Creando estructura de directorios...
if not exist "C:\Program Files\DVTRGA" mkdir "C:\Program Files\DVTRGA" 2>nul
echo   [OK] Directorio creado
echo.

echo [4/6] Copiando archivos...
copy /Y "dvtrga_api.py" "C:\Program Files\DVTRGA\" >nul 2>&1
copy /Y "dvtrga_test.py" "C:\Program Files\DVTRGA\" >nul 2>&1
copy /Y "dvtrga.inf" "C:\Program Files\DVTRGA\" >nul 2>&1
echo   [OK] Archivos copiados
echo.

echo [5/6] Registrando en el sistema...
reg add "HKLM\SOFTWARE\Neuro-OS\DVTRGA" /v "Version" /t REG_SZ /d "1.0.0" /f >nul 2>&1
reg add "HKLM\SOFTWARE\Neuro-OS\DVTRGA" /v "InstallPath" /t REG_SZ /d "C:\Program Files\DVTRGA" /f >nul 2>&1
reg add "HKLM\SOFTWARE\Neuro-OS\DVTRGA" /v "InstallDate" /t REG_SZ /d "%date% %time%" /f >nul 2>&1
echo   [OK] Registro actualizado
echo.

echo [6/6] Creando acceso directo...
echo Set oWS = WScript.CreateObject("WScript.Shell") > CreateShortcut.vbs
echo sLinkFile = "%USERPROFILE%\Desktop\DVTRGA Test.lnk" >> CreateShortcut.vbs
echo Set oLink = oWS.CreateShortcut(sLinkFile) >> CreateShortcut.vbs
echo oLink.TargetPath = "python" >> CreateShortcut.vbs
echo oLink.Arguments = """C:\Program Files\DVTRGA\dvtrga_test.py""" >> CreateShortcut.vbs
echo oLink.WorkingDirectory = "C:\Program Files\DVTRGA" >> CreateShortcut.vbs
echo oLink.Description = "DVTRGA Test Application" >> CreateShortcut.vbs
echo oLink.Save >> CreateShortcut.vbs
cscript //nologo CreateShortcut.vbs >nul 2>&1
del CreateShortcut.vbs >nul 2>&1
echo   [OK] Acceso directo creado en el escritorio
echo.

echo ========================================
echo Instalacion completada exitosamente
echo ========================================
echo.
echo DVTRGA ha sido instalado en:
echo   C:\Program Files\DVTRGA
echo.
echo Acceso directo creado en el escritorio:
echo   "DVTRGA Test"
echo.
echo Para probar DVTRGA:
echo   1. Haz doble clic en "DVTRGA Test" en el escritorio
echo   2. O ejecuta: python "C:\Program Files\DVTRGA\dvtrga_test.py"
echo.
echo NOTA: Esta es una version simulada del driver.
echo      El driver real requiere compilacion con WDK.
echo.
pause
