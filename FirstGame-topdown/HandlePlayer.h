#ifndef HANDLEPLAYER_H
#define HANDLEPLAYER_H

#include <GL/freeglut.h>

class Player
{
private:
    float x;
    float y;

    float size;
    float speed;

    bool up;
    bool down;
    bool left;
    bool right;

public:
    Player();

    void draw();

    void KeyPressed(int key);
    void KeyReleased(int key);

    void update();

    float getX();
    float getY();
};


// The actual player object
extern Player player;


// GLUT callback functions
void handleMovement(int key, int x, int y);
void handleMovementRelease(int key, int x, int y);

#endif