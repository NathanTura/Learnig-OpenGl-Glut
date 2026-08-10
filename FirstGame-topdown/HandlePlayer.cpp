#include "HandlePlayer.h"


Player::Player()
{
    x = 400.0f;
    y = 300.0f;

    size = 30.0f;
    speed = 1.0f;

    up = false;
    down = false;
    left = false;
    right = false;
}


void Player::draw()
{
    glColor3f(1.0f, 0.0f, 0.0f);

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


void Player::KeyPressed(int key)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            up = true;
            break;

        case GLUT_KEY_DOWN:
            down = true;
            break;

        case GLUT_KEY_LEFT:
            left = true;
            break;

        case GLUT_KEY_RIGHT:
            right = true;
            break;
    }
}


void Player::KeyReleased(int key)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            up = false;
            break;

        case GLUT_KEY_DOWN:
            down = false;
            break;

        case GLUT_KEY_LEFT:
            left = false;
            break;

        case GLUT_KEY_RIGHT:
            right = false;
            break;
    }
}


void Player::update()
{
    float dx = 0.0f;
    float dy = 0.0f;


    // Calculate direction

    if (up)
        dy += 1.0f;

    if (down)
        dy -= 1.0f;

    if (left)
        dx -= 1.0f;

    if (right)
        dx += 1.0f;


    // Normalize diagonal movement

    if (dx != 0.0f && dy != 0.0f)
    {
        dx *= 0.7071f;
        dy *= 0.7071f;
    }


    // Move player

    x += dx * speed;
    y += dy * speed;
}


// --------------------------------------------------
// Getters
// --------------------------------------------------

float Player::getX()
{
    return x;
}


float Player::getY()
{
    return y;
}


// --------------------------------------------------
// Create player
// --------------------------------------------------

Player player;


// --------------------------------------------------
// GLUT callbacks
// --------------------------------------------------

void handleMovement(int key, int x, int y)
{
    player.KeyPressed(key);
}


void handleMovementRelease(int key, int x, int y)
{
    player.KeyReleased(key);
}