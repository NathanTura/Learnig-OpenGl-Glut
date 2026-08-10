#include "drawmap.h"

void drawMap()
{
    glColor3f(0.3f, 0.3f, 0.3f);
    glLineWidth(1.0f);

    glBegin(GL_LINES);

    // Vertical lines
    for (int x = 0; x <= WINDOW_WIDTH; x += GRID_SIZE)
    {
        glVertex2f(x, 0);
        glVertex2f(x, WINDOW_HEIGHT);
    }

    // Horizontal lines
    for (int y = 0; y <= WINDOW_HEIGHT; y += GRID_SIZE)
    {
        glVertex2f(0, y);
        glVertex2f(WINDOW_WIDTH, y);
    }

    glEnd();
}