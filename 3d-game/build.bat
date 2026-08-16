@echo off

echo Compiling...

if not exist "libfreeglut.dll" (
    echo Copying libfreeglut.dll...
    copy "..\libfreeglut.dll" "." > nul
)

g++ *.cpp  ^
-I"C:\Users\yerni\Desktop\freeglut\include" ^
-L"C:\Users\yerni\Desktop\freeglut\build\lib" ^
-lfreeglut -lopengl32 -lglu32 -lgdi32 -lwinmm ^
-o Main.exe

if %errorlevel% neq 0 (
    echo.
    echo Compilation failed!
    pause
    exit /b
)

echo.
echo Compilation successful!
echo Starting game...

Main.exe

echo.
echo Game closed.
pause