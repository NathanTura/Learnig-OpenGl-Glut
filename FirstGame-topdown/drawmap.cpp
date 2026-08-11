#include "drawmap.h"

#include <cmath>


TileType world[WORLD_WIDTH][WORLD_HEIGHT];


void generateWorld()
{

    for (int x = 0; x < WORLD_WIDTH; x++)
    {
        for (int y = 0; y < WORLD_HEIGHT; y++)
        {
            world[x][y] = TILE_GRASS;
        }
    }

    for (int x = 190; x < 285; x++)
    {
        for (int y = 70; y < 180; y++)
        {
            float dx =
                (x - 237) / 48.0f;

            float dy =
                (y - 125) / 55.0f;

            float distance =
                dx * dx + dy * dy;

            if (distance < 1.0f)
            {
                world[x][y] = TILE_DESERT;
            }
        }
    }


    for (int x = 25; x < 125; x++)
    {
        for (int y = 190; y < 285; y++)
        {
            float dx =
                (x - 75) / 50.0f;

            float dy =
                (y - 235) / 47.0f;

            float distance =
                dx * dx + dy * dy;

            if (distance < 1.0f)
            {
                world[x][y] = TILE_DESERT;
            }
        }
    }


    for (int x = 115; x < 210; x++)
    {
        for (int y = 15; y < 75; y++)
        {
            float dx =
                (x - 162) / 48.0f;

            float dy =
                (y - 45) / 30.0f;

            float distance =
                dx * dx + dy * dy;

            if (distance < 1.0f)
            {
                world[x][y] = TILE_DESERT;
            }
        }
    }
}

// =========================
// DRAW GRID
// =========================

void drawMap(
    float left,
    float right,
    float bottom,
    float top
)
{
    glColor3f(
        0.3f,
        0.3f,
        0.3f
    );

    glLineWidth(1.0f);


    glBegin(GL_LINES);


    // Align grid to world coordinates

    float startX =
        std::floor(
            left / GRID_SIZE
        ) * GRID_SIZE;

    float startY =
        std::floor(
            bottom / GRID_SIZE
        ) * GRID_SIZE;


    // =========================
    // VERTICAL LINES
    // =========================

    for (
        float x = startX;
        x <= right;
        x += GRID_SIZE
    )
    {
        glVertex2f(
            x,
            bottom
        );

        glVertex2f(
            x,
            top
        );
    }


    // =========================
    // HORIZONTAL LINES
    // =========================

    for (
        float y = startY;
        y <= top;
        y += GRID_SIZE
    )
    {
        glVertex2f(
            left,
            y
        );

        glVertex2f(
            right,
            y
        );
    }


    glEnd();
}


// =========================
// DRAW BIOME
// =========================

void drawBiome(
    float left,
    float right,
    float bottom,
    float top
)
{
    // Align terrain to the
    // exact same grid as drawMap()

    float startX =
        std::floor(
            left / GRID_SIZE
        ) * GRID_SIZE;

    float startY =
        std::floor(
            bottom / GRID_SIZE
        ) * GRID_SIZE;


    // =========================
    // DRAW VISIBLE TILES
    // =========================

    for (
        float x = startX;
        x < right;
        x += GRID_SIZE
    )
    {
        for (
            float y = startY;
            y < top;
            y += GRID_SIZE
        )
        {
            // Convert world coordinate
            // into tile coordinate.

            int tileX =
                static_cast<int>(
                    std::floor(
                        x / GRID_SIZE
                    )
                )
                + WORLD_WIDTH / 2;


            int tileY =
                static_cast<int>(
                    std::floor(
                        y / GRID_SIZE
                    )
                )
                + WORLD_HEIGHT / 2;


            // =========================
            // WORLD BOUNDARY
            // =========================

            if (
                tileX < 0 ||
                tileX >= WORLD_WIDTH ||
                tileY < 0 ||
                tileY >= WORLD_HEIGHT
            )
            {
                continue;
            }


            // Get stored tile

            TileType tile =
                world[tileX][tileY];


            // =========================
            // GRASS
            // =========================

            if (tile == TILE_GRASS)
            {
                glColor3f(
                    0.25f,
                    0.55f,
                    0.20f
                );
            }


            // =========================
            // DESERT
            // =========================

            else if (tile == TILE_DESERT)
            {
                glColor3f(
                    0.75f,
                    0.65f,
                    0.35f
                );
            }


            // =========================
            // DRAW TILE
            // =========================

            glBegin(GL_QUADS);


            glVertex2f(
                x,
                y
            );


            glVertex2f(
                x + GRID_SIZE,
                y
            );


            glVertex2f(
                x + GRID_SIZE,
                y + GRID_SIZE
            );


            glVertex2f(
                x,
                y + GRID_SIZE
            );


            glEnd();
        }
    }
}