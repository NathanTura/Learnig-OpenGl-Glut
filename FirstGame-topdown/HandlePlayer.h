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


    // Zoom

    float zoom;


    const float MIN_ZOOM = 0.35f;
    const float MAX_ZOOM = 2.2f;
    const float ZOOM_SPEED = 0.1f;


public:

    Player();


    void draw();


    void KeyPressed(
        int key
    );


    void KeyReleased(
        int key
    );


    void update();


    float getX();
    float getY();


    void zoomIn();
    void zoomOut();

    float getZoom();
};


extern Player player;


void handleMovement(
    unsigned char key,
    int x,
    int y
);


void handleMovementRelease(
    unsigned char key,
    int x,
    int y
);


void mouseWheel(
    int wheel,
    int direction,
    int x,
    int y
);


#endif