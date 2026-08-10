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

public:
    Player();

    void draw();
    void move(int key);
};

extern Player player;

void handleMovement(int key, int x, int y);

#endif