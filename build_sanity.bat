@echo off
REM Set up VS Environment
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64 >nul 2>&1
if %errorlevel% neq 0 (
    call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64 >nul 2>&1
)

echo Compiling hypersonic_red.c...
cl hypersonic_red.c user32.lib gdi32.lib
if %errorlevel% neq 0 (
    echo [X] SANITY BUILD FAILED
    pause
    exit /b 1
)

echo [OK] SANITY BUILD PASSED
echo Running Hypersonic Red...
hypersonic_red.exe
