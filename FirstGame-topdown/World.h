#ifndef WORLD_H
#define WORLD_H

#include <map>
#include "Chunk.h"

class World
{
private:

    std::map<
        std::pair<int, int>,
        Chunk*
    > chunks;

    int renderDistance;

public:

    World();

    ~World();

    void update(
        float playerX,
        float playerY
    );

    void draw();
};

#endif