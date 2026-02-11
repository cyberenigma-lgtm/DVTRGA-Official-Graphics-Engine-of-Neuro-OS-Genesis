@echo off
echo ============================================================
echo DVTRGA2 SIGLO22 - WORLD RANKING PREP
echo ============================================================
echo.
echo [PASO 1] Branding del Sistema
echo ------------------------------------------------------------
echo El archivo 'siglo22_injector.reg' está listo. 
echo DEBES ejecutarlo como ADMINISTRADOR para inyectar 
echo el nombre "DVTRGA2 SIGLO22" en el registro de Windows.
echo Esto es vital para que 3DMark capture el nombre.
echo.
echo [PASO 2] Captura de Benchmark
echo ------------------------------------------------------------
echo Una vez inyectado el registro, abre 3DMark y ejecuta 
echo "Steel Nomad Light". El driver Hypersonic v40 ya tiene 
echo el HUD "SIGLO22" integrado para la prueba visual.
echo.
echo [VERIFICACION]
echo ------------------------------------------------------------
reg query "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Class\{4d36e968-e325-11ce-bfc1-08002be10318}\0000" /v DriverDesc
echo.
echo Presiona una tecla para salir...
pause >nul
