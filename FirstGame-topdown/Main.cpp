#include <GL/freeglut.h>

#include "drawmap.h"
#include "HandlePlayer.h"


// --------------------------------------------------
// Camera
// --------------------------------------------------

float viewWidth;
float viewHeight;


// --------------------------------------------------
// Reshape
// --------------------------------------------------

void reshape(int w, int h)
{
    if (h == 0)
        h = 1;


    glViewport(0, 0, w, h);


    float aspect = (float)w / (float)h;


    // Vertical world size
    viewHeight = 600.0f;


    // Horizontal world size
    viewWidth = viewHeight * aspect;


    // Camera follows player

    float cameraX = player.getX();
    float cameraY = player.getY();


    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();


    gluOrtho2D(
        cameraX - viewWidth / 2.0f,
        cameraX + viewWidth / 2.0f,

        cameraY - viewHeight / 2.0f,
        cameraY + viewHeight / 2.0f
    );


    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}


// --------------------------------------------------
// Camera update
// --------------------------------------------------

void updateCamera()
{
    float cameraX = player.getX();
    float cameraY = player.getY();


    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();


    gluOrtho2D(
        cameraX - viewWidth / 2.0f,
        cameraX + viewWidth / 2.0f,

        cameraY - viewHeight / 2.0f,
        cameraY + viewHeight / 2.0f
    );


    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}


// --------------------------------------------------
// Display
// --------------------------------------------------

void display()
{
    glClearColor(
        0.05f,
        0.05f,
        0.05f,
        1.0f
    );

    glClear(GL_COLOR_BUFFER_BIT);


    // Camera follows player

    updateCamera();


    // Get visible area

    float cameraX = player.getX();
    float cameraY = player.getY();


    float left =
        cameraX - viewWidth / 2.0f;

    float right =
        cameraX + viewWidth / 2.0f;

    float bottom =
        cameraY - viewHeight / 2.0f;

    float top =
        cameraY + viewHeight / 2.0f;


    // Draw grid

    drawMap(
        left,
        right,
        bottom,
        top
    );


    // Draw player

    player.draw();


    glutSwapBuffers();
}


// --------------------------------------------------
// Update
// --------------------------------------------------

void update()
{
    player.update();

    glutPostRedisplay();
}


// --------------------------------------------------
// Main
// --------------------------------------------------

int main(int argc, char** argv)
{
    glutInit(&argc, argv);


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


    // Window resize

    glutReshapeFunc(
        reshape
    );


    // Drawing

    glutDisplayFunc(
        display
    );


    // Keyboard

    glutSpecialFunc(
        handleMovement
    );


    // Keyboard release

    glutSpecialUpFunc(
        handleMovementRelease
    );


    // Game update

    glutIdleFunc(
        update
    );


    glutMainLoop();


    return 0;
}