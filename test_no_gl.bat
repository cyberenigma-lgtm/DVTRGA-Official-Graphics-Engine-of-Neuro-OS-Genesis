@echo off
REM Set up VS Environment
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64 >nul 2>&1
if %errorlevel% neq 0 (
    call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64 >nul 2>&1
)

echo Compiling sanity_check.c (NO GL)...
cl sanity_check.c user32.lib
if %errorlevel% neq 0 (
    echo [X] BUILD FAILED
    exit /b 1
)

echo [OK] Running Sanity Check (Should MessageBox)...
sanity_check.exe
