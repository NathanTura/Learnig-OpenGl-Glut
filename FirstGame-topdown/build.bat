@echo off

echo Compiling...

g++ Main.cpp HandlePlayer.cpp drawmap.cpp ^
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