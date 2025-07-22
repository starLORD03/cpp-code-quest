@echo off
setlocal

REM Check for build directory
if not exist "%~dp0build" (
    echo [ERROR] Build directory does not exist.
    exit /b 1
)

REM Change to build directory
cd /d "%~dp0build"

REM Build all targets with TLS verification enabled
cmake --build . --config Release --verbose

endlocal
