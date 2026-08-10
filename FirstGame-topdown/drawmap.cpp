#include "drawmap.h"
#include <cmath>

void drawMap(
    float left,
    float right,
    float bottom,
    float top
)
{
    glColor3f(0.3f, 0.3f, 0.3f);
    glLineWidth(1.0f);

    glBegin(GL_LINES);

    float startX = std::floor(left / GRID_SIZE) * GRID_SIZE;
    float startY = std::floor(bottom / GRID_SIZE) * GRID_SIZE;

    // Vertical lines
    for (float x = startX; x <= right; x += GRID_SIZE)
    {
        glVertex2f(x, bottom);
        glVertex2f(x, top);
    }

    // Horizontal lines
    for (float y = startY; y <= top; y += GRID_SIZE)
    {
        glVertex2f(left, y);
        glVertex2f(right, y);
    }

    glEnd();
}