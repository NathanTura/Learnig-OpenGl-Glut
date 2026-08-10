
@echo off

g++ glut_test.cpp -IC:\freeglut\include -LC:\freeglut\lib -lfreeglut -lopengl32 -lglu32 -lgdi32 -lwinmm -o glut_test.exe

if %errorlevel% equ 0 (
    glut_test.exe
) else (
    echo Compilation failed!
    pause
)
