#include "chunk.h"
#include "voxels.h"
#include <math.h>
#include <glm/gtc/noise.hpp>
#include <iostream> 
#include <ctime>
#include <random>



inline int getVoxelIndex(int lx, int ly, int lz) 
{
    return (ly * CHUNK_DEPTH + lz) * CHUNK_WIDTH + lx;
}

Chunk::Chunk(int xpos, int ypos, int zpos) : x(xpos), y(ypos), z(zpos) 
{

    const float NOISE_SCALE = 0.05f;
    const int WATER_LEVEL = 50;
    const int COAL_SPAWN_CHANCE = 25;
    const int TREE_SPACING = 10;
    const float CAVE_NOISE_SCALE = 0.03f;
    const float CAVE_THRESHOLD = 0.05f;
    const int MAX_CAVE_HEIGHT = 55;

    voxels = new Voxel[CHUNK_VOLUME]{};

    for (int ly = 0; ly < CHUNK_HEIGHT; ly++)
    {
        for (int lz = 0; lz < CHUNK_DEPTH; lz++) 
        {
            for (int lx = 0; lx < CHUNK_WIDTH; lx++) 
            {
                int realX = lx + x * CHUNK_WIDTH;
                int realZ = lz + z * CHUNK_DEPTH;
                int realY = ly + y * CHUNK_HEIGHT;

                float noise = glm::perlin(glm::vec3(realX * NOISE_SCALE, realZ * NOISE_SCALE, 0.0f));
                int terrainHeight = (WATER_LEVEL - 20) + static_cast<int>((noise + 1.0f) * 25.0f);

                int id = 0;

                if (realY <= terrainHeight && realY < CHUNK_HEIGHT) 
                {
                    if (realY <= 3) 
                    {
                        id = 6;
                    }
                    else if (realY <= terrainHeight - 10) 
                    {
                        id = 1; 
                        if (rand() % COAL_SPAWN_CHANCE == 0) 
                            id = 7; 
                    }
                    else if (realY <= terrainHeight - 2 && realY >= terrainHeight - 4)
                    {
                        id = 4;
                    }
                    else if (realY <= terrainHeight - 5 && realY >= terrainHeight-9) 
                    {
                        id = 2;
                    }
                    else 
                    {
                        id = 3; 
                        if (realY == terrainHeight && lx % TREE_SPACING == 0 && lz % TREE_SPACING == 0)
                        {
                            if (lx > 2 && lx < 13 && lz > 2 && lz < 13) 
                            {
                                if (rand() % 100 < 60)
                                    generateTree(lx, ly, lz);
                            }
                        }
                    }
                }
                else if (realY <= WATER_LEVEL) 
                {
                    id = 5; 
                }

                if (voxels[getVoxelIndex(lx, ly, lz)].id == 0) 
                {
                    voxels[getVoxelIndex(lx, ly, lz)].id = id;
                }
            }
        }
    }
    for (int ly = 0; ly < CHUNK_HEIGHT; ly++) 
    {
        for (int lz = 0; lz < CHUNK_DEPTH; lz++) 
        {
            for (int lx = 0; lx < CHUNK_WIDTH; lx++) 
            {
                int realY = ly + y * CHUNK_HEIGHT;

                if (realY < MAX_CAVE_HEIGHT) 
                {
                    int realX = lx + x * CHUNK_WIDTH;
                    int realZ = lz + z * CHUNK_DEPTH;

                    float caveNoise = glm::perlin(glm::vec3(realX * CAVE_NOISE_SCALE,realY * CAVE_NOISE_SCALE * 0.005f, realZ * CAVE_NOISE_SCALE));

                    float caveNoise2 = glm::perlin(glm::vec3(realX * CAVE_NOISE_SCALE * 4.0f,realY * CAVE_NOISE_SCALE*0.05f,realZ * CAVE_NOISE_SCALE * 4.0f))*0.5f;

                    float combinedNoise = caveNoise * 0.7f + caveNoise2 * 0.3f;


                    if (combinedNoise > CAVE_THRESHOLD) 
                    {
                        int idx = getVoxelIndex(lx, ly, lz);
                        if (voxels[idx].id != 6 && voxels[idx].id != 5) 
                        {
                            voxels[idx].id = 0; 
                        }
                    }
                }
            }
        }
    }

}
void Chunk::generateTree(int x, int y, int z) 
{
    constexpr int TRUNK_HEIGHT = 6;
    constexpr int MAX_LEAF_LAYERS = 3;

    for (int dy = 0; dy <= TRUNK_HEIGHT; dy++)
    {
        if (y + dy >= CHUNK_HEIGHT)
            break;
        int idx = getVoxelIndex(x, y + dy, z);
        if (voxels[idx].id == 0 || voxels[idx].id == 3) 
        { 
            voxels[idx].id = 10; 
        }
    }

    int leaf_base = y + TRUNK_HEIGHT;
    for (int dy = 0; dy < MAX_LEAF_LAYERS; dy++) 
    {
        int radius = MAX_LEAF_LAYERS - dy;
        for (int dx = -radius; dx <= radius; dx++) 
        {
            for (int dz = -radius; dz <= radius; dz++) 
            {
                if (dx == 0 && dz == 0 && dy == MAX_LEAF_LAYERS - 1) 
                    continue;

                int px = x + dx;
                int py = leaf_base + dy;
                int pz = z + dz;

                if (px < 0 || px >= CHUNK_WIDTH || pz < 0 || pz >= CHUNK_DEPTH || py >= CHUNK_HEIGHT)
                    continue;

                int idx = getVoxelIndex(px, py, pz);
                if (voxels[idx].id == 0) 
                {
                    voxels[idx].id = 9; 
                }
            }
        }
    }
}
Chunk::~Chunk() 
{
    delete[] voxels;
}