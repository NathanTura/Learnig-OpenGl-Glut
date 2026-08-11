#include "HandlePlayer.h"


Player::Player()
{
    // Start at world center

    x = 0.0f;
    y = 0.0f;


    size = 30.0f;

    speed = 1.0f;


    up = false;
    down = false;
    left = false;
    right = false;


    // Normal zoom

    zoom = 1.0f;
}


// =========================
// DRAW PLAYER
// =========================

void Player::draw()
{
    glColor3f(
        1.0f,
        0.0f,
        0.0f
    );


    glBegin(GL_QUADS);


    glVertex2f(
        x - size / 2,
        y - size / 2
    );


    glVertex2f(
        x + size / 2,
        y - size / 2
    );


    glVertex2f(
        x + size / 2,
        y + size / 2
    );


    glVertex2f(
        x - size / 2,
        y + size / 2
    );


    glEnd();
}


// =========================
// KEY PRESSED
// =========================

void Player::KeyPressed(int key)
{
    switch (key)
    {
        case 'w':
        case 'W':
            up = true;
            break;


        case 's':
        case 'S':
            down = true;
            break;


        case 'a':
        case 'A':
            left = true;
            break;


        case 'd':
        case 'D':
            right = true;
            break;
    }
}


// =========================
// KEY RELEASED
// =========================

void Player::KeyReleased(int key)
{
    switch (key)
    {
        case 'w':
        case 'W':
            up = false;
            break;


        case 's':
        case 'S':
            down = false;
            break;


        case 'a':
        case 'A':
            left = false;
            break;


        case 'd':
        case 'D':
            right = false;
            break;
    }
}


// =========================
// PLAYER UPDATE
// =========================

void Player::update()
{
    float dx = 0.0f;
    float dy = 0.0f;


    if (up)
        dy += 1.0f;


    if (down)
        dy -= 1.0f;


    if (left)
        dx -= 1.0f;


    if (right)
        dx += 1.0f;


    // Normalize diagonal movement

    if (
        dx != 0.0f &&
        dy != 0.0f
    )
    {
        dx *= 0.7071f;
        dy *= 0.7071f;
    }


    x += dx * speed;
    y += dy * speed;
}


// =========================
// POSITION
// =========================

float Player::getX()
{
    return x;
}


float Player::getY()
{
    return y;
}


// =========================
// ZOOM
// =========================

void Player::zoomIn()
{
    zoom -= ZOOM_SPEED;


    if (zoom < MIN_ZOOM)
        zoom = MIN_ZOOM;
}


void Player::zoomOut()
{
    zoom += ZOOM_SPEED;


    if (zoom > MAX_ZOOM)
        zoom = MAX_ZOOM;
}


float Player::getZoom()
{
    return zoom;
}


// =========================
// GLOBAL PLAYER
// =========================

Player player;


// =========================
// GLUT INPUT
// =========================

void handleMovement(
    unsigned char key,
    int x,
    int y
)
{
    player.KeyPressed(key);

    glutPostRedisplay();
}


void handleMovementRelease(
    unsigned char key,
    int x,
    int y
)
{
    player.KeyReleased(key);
}


void mouseWheel(
    int wheel,
    int direction,
    int x,
    int y
)
{
    if (direction > 0)
    {
        player.zoomIn();
    }
    else
    {
        player.zoomOut();
    }


    glutPostRedisplay();
}