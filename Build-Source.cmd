@echo off
setlocal
cd /d "%~dp0"
if not exist "assets\default.xbe" (
 echo Supply your extracted Xbox game in assets first. See START-HERE.txt.
 exit /b 1
)
where clang++ >nul 2>&1
if errorlevel 1 (
 echo Run from an x64 Visual Studio developer prompt with LLVM, CMake and Ninja on PATH.
 exit /b 1
)
cmake --preset release
if errorlevel 1 exit /b 1
cmake --build --preset release --target NCAA06 --parallel 4
if errorlevel 1 exit /b 1
copy /y "out\build\release\NCAA06.exe" "NCAA06.exe" >nul
if errorlevel 1 exit /b 1
echo Built NCAA06.exe beside assets. No game was launched.
