@echo off
echo ========================================
echo HYPERSONIC ENGINE BUILDER
echo ========================================

REM Set up VS Environment
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64 >nul 2>&1
if %errorlevel% neq 0 (
    call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64 >nul 2>&1
)

if not exist DVTRGA_Hypersonic\bin mkdir DVTRGA_Hypersonic\bin

echo [1/2] Compiling HYPERSONIC KERNEL (hypersonic_main.c)...
cl /Zi DVTRGA_Hypersonic\hypersonic_main.c /Fe:DVTRGA_Hypersonic\bin\hypersonic.exe user32.lib gdi32.lib

if %errorLevel% neq 0 (
    echo [X] BUILD FAILED
    pause
    exit /b 1
)

echo [2/2] Copying Shaders...
if not exist DVTRGA_Hypersonic\bin\DVTRGA_Hypersonic\shaders mkdir DVTRGA_Hypersonic\bin\DVTRGA_Hypersonic\shaders
copy DVTRGA_Hypersonic\shaders\*.glsl DVTRGA_Hypersonic\bin\DVTRGA_Hypersonic\shaders\ >nul

echo.
echo [OK] HYPERSONIC ENGINE READY.
echo Run: DVTRGA_Hypersonic\bin\hypersonic.exe
echo.
