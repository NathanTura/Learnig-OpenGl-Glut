#include <GL/freeglut.h>

#include "drawmap.h"
#include "HandlePlayer.h"


// =========================
// CAMERA
// =========================

float viewWidth;
float viewHeight;

float baseViewHeight = 600.0f;


// =========================
// RESHAPE
// =========================

void reshape(
    int w,
    int h
)
{
    if (h == 0)
        h = 1;


    glViewport(
        0,
        0,
        w,
        h
    );


    float aspect =
        (float)w / (float)h;


    float zoom =
        player.getZoom();


    viewHeight =
        baseViewHeight * zoom;


    viewWidth =
        viewHeight * aspect;


    float cameraX =
        player.getX();


    float cameraY =
        player.getY();


    glMatrixMode(
        GL_PROJECTION
    );


    glLoadIdentity();


    gluOrtho2D(

        cameraX - viewWidth / 2.0f,

        cameraX + viewWidth / 2.0f,

        cameraY - viewHeight / 2.0f,

        cameraY + viewHeight / 2.0f
    );


    glMatrixMode(
        GL_MODELVIEW
    );


    glLoadIdentity();
}


// =========================
// UPDATE CAMERA
// =========================

void updateCamera()
{
    float cameraX =
        player.getX();


    float cameraY =
        player.getY();


    float zoom =
        player.getZoom();


    viewHeight =
        baseViewHeight * zoom;


    int w =
        glutGet(
            GLUT_WINDOW_WIDTH
        );


    int h =
        glutGet(
            GLUT_WINDOW_HEIGHT
        );


    if (h == 0)
        h = 1;


    float aspect =
        (float)w / (float)h;


    viewWidth =
        viewHeight * aspect;


    glMatrixMode(
        GL_PROJECTION
    );


    glLoadIdentity();


    gluOrtho2D(

        cameraX - viewWidth / 2.0f,

        cameraX + viewWidth / 2.0f,

        cameraY - viewHeight / 2.0f,

        cameraY + viewHeight / 2.0f
    );


    glMatrixMode(
        GL_MODELVIEW
    );


    glLoadIdentity();
}


// =========================
// DISPLAY
// =========================

void display()
{
    // Background

    glClearColor(
        0.05f,
        0.05f,
        0.05f,
        1.0f
    );


    glClear(
        GL_COLOR_BUFFER_BIT
    );


    // Update camera

    updateCamera();


    float cameraX =
        player.getX();


    float cameraY =
        player.getY();


    float left =
        cameraX -
        viewWidth / 2.0f;


    float right =
        cameraX +
        viewWidth / 2.0f;


    float bottom =
        cameraY -
        viewHeight / 2.0f;


    float top =
        cameraY +
        viewHeight / 2.0f;


    // =========================
    // DRAW TERRAIN
    // =========================

    drawBiome(
        left,
        right,
        bottom,
        top
    );


    // =========================
    // DRAW GRID
    // =========================

    drawMap(
        left,
        right,
        bottom,
        top
    );


    // =========================
    // DRAW PLAYER
    // =========================

    player.draw();


    glutSwapBuffers();
}


// =========================
// UPDATE
// =========================

void update()
{
    player.update();

    glutPostRedisplay();
}


// =========================
// MAIN
// =========================

int main(
    int argc,
    char** argv
)
{
    glutInit(
        &argc,
        argv
    );


    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB
    );


    glutInitWindowSize(
        800,
        600
    );


    glutCreateWindow(
        "Top Down Test"
    );


    // =========================
    // GENERATE WORLD ONCE
    // =========================

    generateWorld();


    // =========================
    // GLUT CALLBACKS
    // =========================

    glutReshapeFunc(
        reshape
    );


    glutDisplayFunc(
        display
    );


    glutKeyboardFunc(
        handleMovement
    );


    glutKeyboardUpFunc(
        handleMovementRelease
    );


    glutMouseWheelFunc(
        mouseWheel
    );


    glutIdleFunc(
        update
    );


    // =========================
    // START GAME
    // =========================

    glutMainLoop();


    return 0;
}