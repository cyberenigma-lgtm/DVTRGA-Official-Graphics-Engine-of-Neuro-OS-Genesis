@echo off
REM Set up VS Environment
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64 >nul 2>&1
if %errorlevel% neq 0 (
    call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64 >nul 2>&1
)

echo Compiling test_gl.c...
cl test_gl.c /link user32.lib gdi32.lib opengl32.lib
if %errorlevel% neq 0 (
    echo [X] TEST FAILED
    exit /b 1
)
echo [OK] TEST PASSED
