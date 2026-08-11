#ifndef DRAWMAP_H
#define DRAWMAP_H

#include <GL/freeglut.h>

// =========================
// TILE / WORLD SETTINGS
// =========================

const float GRID_SIZE = 40.0f;

// Tiles inside one chunk
const int CHUNK_SIZE = 15;

// Number of chunks in the world
const int WORLD_CHUNKS_X = 20;
const int WORLD_CHUNKS_Y = 20;

// Total tiles
const int WORLD_WIDTH =
    CHUNK_SIZE * WORLD_CHUNKS_X;

const int WORLD_HEIGHT =
    CHUNK_SIZE * WORLD_CHUNKS_Y;


// =========================
// TILE TYPES
// =========================

enum TileType
{
    TILE_GRASS,
    TILE_DESERT
};


// =========================
// FUNCTIONS
// =========================

void generateWorld();

void drawMap(
    float left,
    float right,
    float bottom,
    float top
);

void drawBiome(
    float left,
    float right,
    float bottom,
    float top
);

#endif