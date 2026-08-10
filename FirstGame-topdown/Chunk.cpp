#include "Chunk.h"

#include <GL/freeglut.h>
#include <cmath>
#include <random>

// ==================================================
// WORLD GENERATION SETTINGS
// ==================================================

const int START_CHUNK_RADIUS = 1;

// How many ore patches to attempt per chunk
const int ORE_PATCH_COUNT = 1;


// ==================================================
// SIMPLE DETERMINISTIC NOISE
// ==================================================

static float noise2D(int x, int y)
{
    unsigned int n =
        (unsigned int)(
            x * 374761393 +
            y * 668265263
        );

    n =
        (n ^ (n >> 13))
        * 1274126177;

    n ^= n >> 16;

    return
        (n % 10000) / 10000.0f;
}


// ==================================================
// DETERMINISTIC RANDOM 0-1
// ==================================================

static float random01(int x, int y)
{
    unsigned int n =
        (unsigned int)(
            x * 374761393 +
            y * 668265263
        );

    n =
        (n ^ (n >> 13))
        * 1274126177;

    n ^= n >> 16;

    return
        (n % 10000) / 10000.0f;
}


// ==================================================
// SMOOTH INTERPOLATION
// ==================================================

static float smoothNoise(
    float x,
    float y
)
{
    int x0 = (int)std::floor(x);
    int y0 = (int)std::floor(y);

    int x1 = x0 + 1;
    int y1 = y0 + 1;


    float sx =
        x - x0;

    float sy =
        y - y0;


    // Smooth curve

    sx =
        sx * sx *
        (3.0f - 2.0f * sx);

    sy =
        sy * sy *
        (3.0f - 2.0f * sy);


    float n00 =
        noise2D(x0, y0);

    float n10 =
        noise2D(x1, y0);

    float n01 =
        noise2D(x0, y1);

    float n11 =
        noise2D(x1, y1);


    float nx0 =
        n00 +
        (n10 - n00) * sx;

    float nx1 =
        n01 +
        (n11 - n01) * sx;


    return
        nx0 +
        (nx1 - nx0) * sy;
}


// ==================================================
// BIOME NOISE
// ==================================================

static float biomeNoise(
    float worldX,
    float worldY
)
{
    float scale = 0.035f;

    return smoothNoise(
        worldX * scale,
        worldY * scale
    );
}


// ==================================================
// CONSTRUCTOR
// ==================================================

Chunk::Chunk(
    int x,
    int y
)
{
    chunkX = x;
    chunkY = y;

    generate();
}


// ==================================================
// GENERATE CHUNK
// ==================================================

void Chunk::generate()
{
    resources.clear();


    unsigned int seed =
        12345
        + chunkX * 73856093
        + chunkY * 19349663;


    std::mt19937 rng(seed);


    // =================================================
    // BIOME
    // =================================================

    float centerX =
        chunkX * CHUNK_SIZE
        + CHUNK_SIZE / 2.0f;

    float centerY =
        chunkY * CHUNK_SIZE
        + CHUNK_SIZE / 2.0f;


    float biomeValue =
        biomeNoise(
            centerX,
            centerY
        );


    if (biomeValue < 0.42f)
    {
        biome = SAND_BIOME;
    }
    else
    {
        biome = GRASS_BIOME;
    }


    // =================================================
    // TREES
    // =================================================

    generateTrees(rng);


    // =================================================
    // STARTER RESOURCES
    // =================================================

    generateStarterResources();


    // =================================================
    // NORMAL ORE PATCHES
    // =================================================

    for (int i = 0;
         i < ORE_PATCH_COUNT;
         i++)
    {
        generatePatch(
            IRON,
            0.25f,
            50,
            rng
        );


        generatePatch(
            COPPER,
            0.22f,
            50,
            rng
        );


        generatePatch(
            COAL,
            0.20f,
            50,
            rng
        );


        generatePatch(
            STONE,
            0.30f,
            40,
            rng
        );
    }
}


// ==================================================
// STARTER RESOURCES
// ==================================================

void Chunk::generateStarterResources()
{
    // Only chunks immediately around
    // the starting chunk.

    if (std::abs(chunkX) >
        START_CHUNK_RADIUS)
        return;


    if (std::abs(chunkY) >
        START_CHUNK_RADIUS)
        return;


    int centerX =
        chunkX * CHUNK_SIZE
        + CHUNK_SIZE / 2;


    int centerY =
        chunkY * CHUNK_SIZE
        + CHUNK_SIZE / 2;


    // =================================================
    // IRON
    // =================================================

    Resource iron;

    iron.x =
        centerX - 5;

    iron.y =
        centerY + 2;

    iron.type =
        IRON;

    iron.amount =
        200;

    resources.push_back(
        iron
    );


    // =================================================
    // COPPER
    // =================================================

    Resource copper;

    copper.x =
        centerX + 5;

    copper.y =
        centerY + 2;

    copper.type =
        COPPER;

    copper.amount =
        200;

    resources.push_back(
        copper
    );


    // =================================================
    // COAL
    // =================================================

    Resource coal;

    coal.x =
        centerX + 2;

    coal.y =
        centerY - 5;

    coal.type =
        COAL;

    coal.amount =
        200;

    resources.push_back(
        coal
    );


    // =================================================
    // STONE
    // =================================================

    Resource stone;

    stone.x =
        centerX - 3;

    stone.y =
        centerY - 5;

    stone.type =
        STONE;

    stone.amount =
        200;

    resources.push_back(
        stone
    );
}


// ==================================================
// TREES
// ==================================================

void Chunk::generateTrees(
    std::mt19937& rng
)
{
    std::uniform_real_distribution<float> chance(
        0.0f,
        1.0f
    );


    // ==================================================
    // FACTORIO-STYLE TREE SETTINGS
    // ==================================================

    // Smaller = forests closer together.
    // Larger = forests farther apart.

    const float FOREST_SCALE =
        0.025f;


    // How much of a forest gets covered.

    const float TREE_COVERAGE =
        0.62f;


    // Small noise used to create
    // clearings and irregular edges.

    const float DETAIL_SCALE =
        0.085f;


    // ==================================================
    // WORLD POSITION
    // ==================================================

    int startX =
        chunkX * CHUNK_SIZE;

    int startY =
        chunkY * CHUNK_SIZE;


    // ==================================================
    // GENERATE TILE BY TILE
    // ==================================================

    for (int x = 0;
         x < CHUNK_SIZE;
         x++)
    {
        for (int y = 0;
             y < CHUNK_SIZE;
             y++)
        {
            int worldX =
                startX + x;

            int worldY =
                startY + y;


            // =================================================
            // KEEP STARTING AREA CLEAR
            // =================================================

            float distanceFromSpawn =
                std::sqrt(
                    (float)(
                        worldX * worldX +
                        worldY * worldY
                    )
                );


            if (distanceFromSpawn < 45.0f)
                continue;


            // =================================================
            // SAND
            // =================================================

            // Desert has very few trees.

            if (biomeNoise(
                    (float)worldX,
                    (float)worldY
                ) < 0.42f)
            {
                if (
                    random01(
                        worldX,
                        worldY
                    ) > 0.025f
                )
                {
                    continue;
                }
            }


            // =================================================
            // LARGE FOREST FIELD
            // =================================================

            float forest =
                smoothNoise(
                    worldX * FOREST_SCALE,
                    worldY * FOREST_SCALE
                );


            // =================================================
            // FOREST THRESHOLD
            // =================================================

            // Below this = open land.
            //
            // Above this = forest.

            const float FOREST_THRESHOLD =
                0.55f;


            if (forest <
                FOREST_THRESHOLD)
            {
                continue;
            }


            // =================================================
            // FOREST DENSITY
            // =================================================

            float forestDensity =
                (
                    forest -
                    FOREST_THRESHOLD
                )
                /
                (
                    1.0f -
                    FOREST_THRESHOLD
                );


            // =================================================
            // DETAIL NOISE
            // =================================================

            float detail =
                smoothNoise(
                    worldX * DETAIL_SCALE,
                    worldY * DETAIL_SCALE
                );


            // =================================================
            // COMBINE
            // =================================================

            float finalDensity =
                forestDensity *
                TREE_COVERAGE;


            // Detail creates natural gaps.

            finalDensity =
                finalDensity * 0.78f
                +
                detail * 0.22f;


            // =================================================
            // RANDOM TREE PLACEMENT
            // =================================================

            float random =
                random01(
                    worldX * 31,
                    worldY * 47
                );


            if (random >
                finalDensity)
            {
                continue;
            }


            // =================================================
            // TREE
            // =================================================

            Resource tree;

            tree.x =
                worldX;

            tree.y =
                worldY;

            tree.type =
                TREE;

            tree.amount =
                100;


            resources.push_back(
                tree
            );
        }
    }
}


// ==================================================
// ORE PATCH
// ==================================================

void Chunk::generatePatch(
    ResourceType type,
    float probability,
    int amount,
    std::mt19937& rng
)
{
    std::uniform_real_distribution<float> chance(
        0.0f,
        1.0f
    );


    // ==================================================
    // DOES THIS CHUNK GET A DEPOSIT?
    // ==================================================

    if (chance(rng) >
        probability)
        return;


    // ==================================================
    // DEPOSIT CENTER
    // ==================================================

    int centerX =
        chunkX * CHUNK_SIZE +
        rng() % CHUNK_SIZE;

    int centerY =
        chunkY * CHUNK_SIZE +
        rng() % CHUNK_SIZE;


    // ==================================================
    // DEPOSIT SIZE
    // ==================================================

    int radius =
        4 + rng() % 5;


    // ==================================================
    // GENERATE DEPOSIT
    // ==================================================

    for (int x = -radius;
         x <= radius;
         x++)
    {
        for (int y = -radius;
             y <= radius;
             y++)
        {
            float distance =
                std::sqrt(
                    (float)(
                        x * x +
                        y * y
                    )
                );


            if (distance >
                radius)
                continue;


            // =================================================
            // DENSITY
            // =================================================

            float density =
                1.0f -
                (
                    distance /
                    radius
                );


            // =================================================
            // IRREGULAR EDGE
            // =================================================

            float variation =
                chance(rng);


            if (variation >
                0.35f +
                density * 0.65f)
            {
                continue;
            }


            // =================================================
            // WORLD POSITION
            // =================================================

            int worldX =
                centerX + x;

            int worldY =
                centerY + y;


            // =================================================
            // RESOURCE
            // =================================================

            Resource resource;

            resource.x =
                worldX;

            resource.y =
                worldY;

            resource.type =
                type;


            // =================================================
            // RICHNESS
            // =================================================

            resource.amount =
                amount / 2 +
                rng() % amount;


            resources.push_back(
                resource
            );
        }
    }
}


// ==================================================
// TERRAIN
// ==================================================

void Chunk::drawTerrain()
{
    for (
        int x = 0;
        x < CHUNK_SIZE;
        x++
    )
    {
        for (
            int y = 0;
            y < CHUNK_SIZE;
            y++
        )
        {
            float worldX =
                (
                    chunkX * CHUNK_SIZE
                    + x
                )
                * TILE_SIZE;


            float worldY =
                (
                    chunkY * CHUNK_SIZE
                    + y
                )
                * TILE_SIZE;


            // Use the CENTER of the tile
            // for biome calculation.

            float centerX =
                chunkX * CHUNK_SIZE
                + x
                + 0.5f;


            float centerY =
                chunkY * CHUNK_SIZE
                + y
                + 0.5f;


            float value =
                biomeNoise(
                    centerX,
                    centerY
                );


            // =================================================
            // GRASS
            // =================================================

            if (value > 0.42f)
            {
                glColor3f(
                    0.25f,
                    0.55f,
                    0.20f
                );
            }


            // =================================================
            // SAND
            // =================================================

            else
            {
                glColor3f(
                    0.76f,
                    0.68f,
                    0.42f
                );
            }


            glBegin(GL_QUADS);

                glVertex2f(
                    worldX,
                    worldY
                );

                glVertex2f(
                    worldX + TILE_SIZE,
                    worldY
                );

                glVertex2f(
                    worldX + TILE_SIZE,
                    worldY + TILE_SIZE
                );

                glVertex2f(
                    worldX,
                    worldY + TILE_SIZE
                );

            glEnd();
        }
    }
}


// ==================================================
// RESOURCES
// ==================================================

void Chunk::drawResources()
{
    for (
        const Resource& resource :
        resources
    )
    {
        float x =
            resource.x * TILE_SIZE
            + TILE_SIZE / 2.0f;


        float y =
            resource.y * TILE_SIZE
            + TILE_SIZE / 2.0f;


        // =========================================
        // TREE
        // =========================================

        if (resource.type == TREE)
        {
            glColor3f(
                0.08f,
                0.35f,
                0.05f
            );


            glBegin(GL_TRIANGLES);

                glVertex2f(
                    x,
                    y + 17
                );

                glVertex2f(
                    x - 16,
                    y - 12
                );

                glVertex2f(
                    x + 16,
                    y - 12
                );

            glEnd();
        }


        // =========================================
        // STONE
        // =========================================

        else if (
            resource.type == STONE
        )
        {
            glColor3f(
                0.45f,
                0.45f,
                0.45f
            );


            glBegin(GL_POLYGON);

                glVertex2f(
                    x - 15,
                    y - 10
                );

                glVertex2f(
                    x - 8,
                    y + 12
                );

                glVertex2f(
                    x + 10,
                    y + 15
                );

                glVertex2f(
                    x + 16,
                    y - 5
                );

                glVertex2f(
                    x + 5,
                    y - 15
                );

            glEnd();
        }


        // =========================================
        // IRON
        // =========================================

        else if (
            resource.type == IRON
        )
        {
            glColor3f(
                0.30f,
                0.35f,
                0.40f
            );


            glBegin(GL_POLYGON);

                glVertex2f(
                    x - 15,
                    y
                );

                glVertex2f(
                    x - 7,
                    y + 15
                );

                glVertex2f(
                    x + 12,
                    y + 10
                );

                glVertex2f(
                    x + 15,
                    y - 8
                );

                glVertex2f(
                    x,
                    y - 15
                );

            glEnd();
        }


        // =========================================
        // COPPER
        // =========================================

        else if (
            resource.type == COPPER
        )
        {
            glColor3f(
                0.75f,
                0.35f,
                0.08f
            );


            glBegin(GL_POLYGON);

                glVertex2f(
                    x - 12,
                    y - 12
                );

                glVertex2f(
                    x + 10,
                    y - 14
                );

                glVertex2f(
                    x + 15,
                    y
                );

                glVertex2f(
                    x + 8,
                    y + 14
                );

                glVertex2f(
                    x - 12,
                    y + 10
                );

            glEnd();
        }


        // =========================================
        // COAL
        // =========================================

        else if (
            resource.type == COAL
        )
        {
            glColor3f(
                0.03f,
                0.03f,
                0.03f
            );


            glBegin(GL_POLYGON);

                glVertex2f(
                    x - 12,
                    y - 10
                );

                glVertex2f(
                    x - 15,
                    y + 5
                );

                glVertex2f(
                    x - 5,
                    y + 15
                );

                glVertex2f(
                    x + 12,
                    y + 10
                );

                glVertex2f(
                    x + 15,
                    y - 8
                );

            glEnd();
        }
    }
}


// ==================================================
// DRAW
// ==================================================

void Chunk::draw()
{
    drawTerrain();

    drawResources();
}


// ==================================================
// GETTERS
// ==================================================

int Chunk::getX()
{
    return chunkX;
}


int Chunk::getY()
{
    return chunkY;
}