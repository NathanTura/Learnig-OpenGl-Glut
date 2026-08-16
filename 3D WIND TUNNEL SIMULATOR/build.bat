@echo off

echo Compiling Wind Tunnel Simulator Phase 1...

if not exist "libfreeglut.dll" (
    echo Copying libfreeglut.dll...
    copy "..\freeglut\build\bin\libfreeglut.dll" "." > nul 2>&1
    if not exist "libfreeglut.dll" (
        copy "..\libfreeglut.dll" "." > nul 2>&1
    )
)

g++ *.cpp ^
-I"C:\Users\yerni\Desktop\freeglut\include" ^
-L"C:\Users\yerni\Desktop\freeglut\build\lib" ^
-lfreeglut -lopengl32 -lglu32 -lgdi32 -lwinmm ^
-mconsole -o WindTunnel.exe

if %errorlevel% neq 0 (
    echo.
    echo Compilation failed!
    pause
    exit /b
)

echo.
echo Compilation successful!
echo Starting WindTunnel...

WindTunnel.exe

echo.
echo Simulator closed.
pause