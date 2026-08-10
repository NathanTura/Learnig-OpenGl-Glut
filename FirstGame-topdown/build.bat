@echo off

echo Compiling...

g++ Main.cpp HandlePlayer.cpp World.cpp Chunk.cpp drawmap.cpp ^
-I"C:\freeglut\include" ^
-L"C:\freeglut\lib" ^
-lfreeglut -lopengl32 -lglu32 -lgdi32 -lwinmm ^
-o Main.exe

if %errorlevel% neq 0 (
    echo.
    echo Compilation failed!
    pause
    exit /b
)

echo Compilation successful!
echo Starting game...

Main.exe

echo.
echo Game closed.
pause