#include "World.h"

#include <cmath>


// ==================================================
// CONSTRUCTOR
// ==================================================

World::World()
{
    // 2 = 5x5 chunks
    renderDistance = 2;
}


// ==================================================
// DESTRUCTOR
// ==================================================

World::~World()
{
    for (auto& pair : chunks)
    {
        delete pair.second;
    }

    chunks.clear();
}


// ==================================================
// UPDATE WORLD
// ==================================================

void World::update(
    float playerX,
    float playerY
)
{
    const float CHUNK_WORLD_SIZE =
        CHUNK_SIZE * 40.0f;


    // Find player's chunk

    int playerChunkX =
        (int)std::floor(
            playerX / CHUNK_WORLD_SIZE
        );

    int playerChunkY =
        (int)std::floor(
            playerY / CHUNK_WORLD_SIZE
        );


    // Generate nearby chunks

    for (
        int x = -renderDistance;
        x <= renderDistance;
        x++
    )
    {
        for (
            int y = -renderDistance;
            y <= renderDistance;
            y++
        )
        {
            int chunkX =
                playerChunkX + x;

            int chunkY =
                playerChunkY + y;


            std::pair<int, int> key(
                chunkX,
                chunkY
            );


            // Does chunk already exist?

            if (chunks.find(key) == chunks.end())
            {
                chunks[key] =
                    new Chunk(
                        chunkX,
                        chunkY
                    );
            }
        }
    }
}


// ==================================================
// DRAW WORLD
// ==================================================

void World::draw()
{
    for (auto& pair : chunks)
    {
        pair.second->draw();
    }
}