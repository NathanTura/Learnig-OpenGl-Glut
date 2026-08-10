#include <GL/freeglut.h>

#include "drawmap.h"
#include "HandlePlayer.h"
#include "World.h"

float viewWidth;
float viewHeight;

float baseViewHeight = 600.0f;

World world;

void reshape(int w, int h)
{
    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    float aspect = (float)w / (float)h;

    // Get zoom from player
    float zoom = player.getZoom();

    viewHeight = baseViewHeight * zoom;
    viewWidth = viewHeight * aspect;


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

void updateCamera()
{
    float cameraX = player.getX();
    float cameraY = player.getY();

    float zoom = player.getZoom();

    viewHeight = baseViewHeight * zoom;

    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);

    if (h == 0)
        h = 1;

    float aspect = (float)w / (float)h;

    viewWidth = viewHeight * aspect;


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


void display()
{
    glClearColor(
        0.05f,
        0.05f,
        0.05f,
        1.0f
    );

    glClear(GL_COLOR_BUFFER_BIT);


    updateCamera();


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


    drawMap(
        left,
        right,
        bottom,
        top
    );

    world.draw();

    player.draw();
    glutSwapBuffers();
}


void update()
{
    player.update();

        world.update(
        player.getX(),
        player.getY()
    );


    glutPostRedisplay();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB
    );

    glutInitWindowSize(800,600);

    glutCreateWindow(
        "Top Down Test"
    );

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(handleMovement);
    glutKeyboardUpFunc(handleMovementRelease);
    glutMouseWheelFunc(mouseWheel);
    glutIdleFunc(update);

    glutMainLoop();

    return 0;
}