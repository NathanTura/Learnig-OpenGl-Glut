@echo off

echo Compiling...

g++ class1.cpp  ^
-I"C:\Users\yerni\Desktop\freeglut\include" ^
-L"C:\Users\yerni\Desktop\freeglut\build\lib" ^
-lfreeglut -lopengl32 -lglu32 -lgdi32 -lwinmm ^
-o class1.exe

if %errorlevel% neq 0 (
    echo.
    echo Compilation failed!
    pause
    exit /b
)

echo.
echo Compilation successful!
echo Starting game...

class1.exe

echo.
echo Game closed.
pause