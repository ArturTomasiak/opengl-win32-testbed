@echo off
setlocal enabledelayedexpansion

set COMPILER=gcc
set OUTPUT=exe\main.exe

set SOURCES =
for /R src %%f in (*.c) do (
    set sources=!sources! "%%f"
)

set INCLUDES=-I"C:\glew-2.1.0\include"
set LIBRARIES=-L"C:\glew-2.1.0\lib\Release\x64"
set FLAGS=-lopengl32 -lglew32 -lgdi32 -Wall

where %COMPILER% >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: GCC compiler not found. Please install MinGW or another compatible compiler.
    exit /b 1
)

echo Compiling...
%COMPILER% %INCLUDES% %SOURCES% -o %OUTPUT% %LIBRARIES% %FLAGS%

if %errorlevel% neq 0 (
    echo Compilation failed.
    exit /b 1
)
  
echo Compilation successful! Run .\%OUTPUT% to execute.

endlocal