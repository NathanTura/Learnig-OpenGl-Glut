#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <random>


// ==================================================
// CHUNK SETTINGS
// ==================================================

const int CHUNK_SIZE = 32;
const float TILE_SIZE = 40.0f;


// ==================================================
// BIOMES
// ==================================================

enum BiomeType
{
    GRASS_BIOME,
    SAND_BIOME
};


// ==================================================
// RESOURCES
// ==================================================

enum ResourceType
{
    TREE,
    STONE,
    IRON,
    COPPER,
    COAL
};


// ==================================================
// RESOURCE DATA
// ==================================================

struct Resource
{
    int x;
    int y;

    ResourceType type;

    int amount;
};


// ==================================================
// CHUNK
// ==================================================

class Chunk
{
private:

    int chunkX;
    int chunkY;

    BiomeType biome;

    std::vector<Resource> resources;


public:

    // Constructor
    Chunk(int x, int y);


    // Generation
    void generate();

    void generateStarterResources();

    void generatePatch(
        ResourceType type,
        float probability,
        int amount,
        std::mt19937& rng
    );

    void generateTrees(
        std::mt19937& rng
    );


    // Drawing
    void draw();

    void drawTerrain();

    void drawResources();


    // Get position
    int getX();

    int getY();
};

#endif