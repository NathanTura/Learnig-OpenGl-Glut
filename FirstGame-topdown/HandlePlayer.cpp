#include "HandlePlayer.h"

Player::Player()
{
    x = 400;
    y = 300;
    size = 30;
    speed = 5.0f;
}

void Player::draw()
{
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_QUADS);

        glVertex2f(x - size / 2, y - size / 2);
        glVertex2f(x + size / 2, y - size / 2);
        glVertex2f(x + size / 2, y + size / 2);
        glVertex2f(x - size / 2, y + size / 2);

    glEnd();
}

void Player::move(int key)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            y += speed;
            break;

        case GLUT_KEY_DOWN:
            y -= speed;
            break;

        case GLUT_KEY_LEFT:
            x -= speed;
            break;

        case GLUT_KEY_RIGHT:
            x += speed;
            break;
    }
}

Player player;

void handleMovement(int key, int x, int y)
{
    player.move(key);

    glutPostRedisplay();
}