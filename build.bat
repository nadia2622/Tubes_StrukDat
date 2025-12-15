@echo off
REM Build script FIXED - No circular includes
REM Compile semua file .cpp secara terpisah, lalu link jadi satu executable

echo ============================================
echo Comic Management System - Build Script
echo (FIXED VERSION - No Circular Includes)
echo ============================================
echo.

REM Clean previous build
echo Cleaning previous build...
if exist komik_system.exe del komik_system.exe
if exist *.o del *.o
if exist src\*.o del src\*.o
echo.

REM Compile source files
echo Compiling source files...
echo.

echo [1/6] Compiling main.cpp...
g++ -std=c++11 -Wall -I include -c main.cpp -o main.o
if errorlevel 1 (
    echo ERROR: Failed to compile main.cpp
    pause
    exit /b 1
)

echo [2/6] Compiling BST.cpp...
g++ -std=c++11 -Wall -I include -c src/BST.cpp -o src/BST.o
if errorlevel 1 (
    echo ERROR: Failed to compile BST.cpp
    pause
    exit /b 1
)

echo [3/6] Compiling KomikManager.cpp...
g++ -std=c++11 -Wall -I include -c src/KomikManager.cpp -o src/KomikManager.o
if errorlevel 1 (
    echo ERROR: Failed to compile KomikManager.cpp
    pause
    exit /b 1
)

@REM echo [4/6] Compiling SearchFilter.cpp...
@REM g++ -std=c++11 -Wall -I include -c src/SearchFilter.cpp -o src/SearchFilter.o
@REM if errorlevel 1 (
@REM     echo ERROR: Failed to compile SearchFilter.cpp
@REM     pause
@REM     exit /b 1
@REM )

echo [5/6] Compiling Auth.cpp...
g++ -std=c++11 -Wall -I include -c src/Auth.cpp -o src/Auth.o
if errorlevel 1 (
    echo ERROR: Failed to compile Auth.cpp
    pause
    exit /b 1
)

echo [6/6] Compiling MenuSystem.cpp...
g++ -std=c++11 -Wall -I include -c src/MenuSystem.cpp -o src/MenuSystem.o
if errorlevel 1 (
    echo ERROR: Failed to compile MenuSystem.cpp
    pause
    exit /b 1
)

echo.
echo Compilation successful!
echo.

REM Link object files
echo Linking...
g++ -o komik_system.exe main.o src/BST.o src/KomikManager.o src/Auth.o src/MenuSystem.o
if errorlevel 1 (
    echo ERROR: Linking failed!
    pause
    exit /b 1
)

echo.
echo ============================================
echo Build completed successfully!
echo ============================================
echo.
echo Executable: komik_system.exe
echo.

REM Ask if user wants to run
set /p RUN="Do you want to run the program now? (Y/N): "
if /i "%RUN%"=="Y" (
    echo.
    echo Running Comic Management System...
    echo ============================================
    komik_system.exe
) else (
    echo.
    echo You can run the program later with: komik_system.exe
)

echo.
pause