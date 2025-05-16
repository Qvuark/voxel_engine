#include "chunk.h"
#include "voxels.h"
#include "../blocks/blockRegister.h"
#include "../blocks/IBlock.h"
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
    const int   WATER_LEVEL = 50;
    const int   COAL_SPAWN_CHANCE = 104;
    const int   TREE_SPACING = 10;

    const float CAVE_NOISE_SCALE = 0.03f;
    const int   CAVE_MIN_Y = 5;
    const int   CAVE_MAX_Y = 55;
    const float CAVE_THRESHOLD = 0.15f;
    const float VERTICAL_SQUASH = 0.08f;
    const float HORIZONTAL_STRETCH = 1.9f;
    const float CAVE_WARP_INTENSITY = 2.5f;
    const float SURFACE_ENTRY_CHANCE = 0.002f;
    const int   ENTRY_RADIUS = 6;
    const int   SHAFT_RADIUS = 3;
    const int   MIN_ENTRY_DEPTH = 8;

    voxels = new std::unique_ptr<IBlock>[CHUNK_VOLUME];


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
                    else if (realY <= terrainHeight - 5 && realY >= terrainHeight - 9)
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

                if (voxels[getVoxelIndex(lx, ly, lz)] == 0)
                {
                    voxels[getVoxelIndex(lx, ly, lz)] = createBlockById(id);
                }
            }
        }
    }
    for (int ly = 0; ly < CHUNK_HEIGHT; ly++)
    {
        int realY = ly + y * CHUNK_HEIGHT;
        if (realY < CAVE_MIN_Y || realY + 5 > CAVE_MAX_Y)
            continue;

        float t = float(realY - CAVE_MIN_Y) / float(CAVE_MAX_Y - CAVE_MIN_Y);
        float mask = 1.0f - 4.0f * (t - 0.5f) * (t - 0.5f);

        for (int lz = 0; lz < CHUNK_DEPTH; lz++)
        {
            for (int lx = 0; lx < CHUNK_WIDTH; lx++)
            {
                int idx = getVoxelIndex(lx, ly, lz);

                if (!voxels[idx]-> isBlockCarvable())
                    continue;

                int realX = lx + x * CHUNK_WIDTH;
                int realZ = lz + z * CHUNK_DEPTH;

                glm::vec3 warpPos = glm::vec3(realX * CAVE_NOISE_SCALE * 0.3f, realZ * CAVE_NOISE_SCALE * 0.3f, realY * VERTICAL_SQUASH * 0.1f);

                glm::vec3 q = glm::vec3(glm::perlin(warpPos + glm::vec3(0.0f, 5.2f, 1.7f)), glm::perlin(warpPos + glm::vec3(3.5f, 2.8f, 8.1f)), glm::perlin(warpPos + glm::vec3(9.7f, 4.3f, 6.6f)));

                glm::vec3 warped = warpPos + q * CAVE_WARP_INTENSITY;

                float n1 = glm::perlin(glm::vec3(realX * CAVE_NOISE_SCALE * HORIZONTAL_STRETCH, realZ * CAVE_NOISE_SCALE * HORIZONTAL_STRETCH, realY * VERTICAL_SQUASH));

                float n2 = glm::perlin(warped * 3.0f) * 0.4f;

                float n3 = glm::perlin(glm::vec3(realX * 0.15f, realZ * 0.15f, realY * 0.05f)) * 0.2f;

                float noise = (n1 * 0.7f + n2 * 0.2f + n3 * 0.1f) * mask;

                if (noise > CAVE_THRESHOLD)
                {
                    voxels[idx] = createBlockById(0);

                    int surfaceHeight = WATER_LEVEL + 5;
                    int depth = surfaceHeight - realY;

                    if (depth >= MIN_ENTRY_DEPTH && realY >= CAVE_MAX_Y - 15 && mask > 0.7f && noise > CAVE_THRESHOLD * 1.5f && rand() % 1000 < SURFACE_ENTRY_CHANCE * 1000)
                    {
                        glm::vec3 entryWarp = glm::vec3(realX * 0.1f, realZ * 0.1f, realY * 0.05f);

                        float warpX = glm::perlin(entryWarp + glm::vec3(10.0f));
                        float warpZ = glm::perlin(entryWarp + glm::vec3(20.0f));

                        for (int dy = 0; dy <= depth; dy++)
                        {
                            int currentY = realY + dy;
                            if (currentY > CAVE_MAX_Y)
                                break;

                            float t = float(dy) / depth;
                            int offsetX = lx + warpX * t * 4.0f;
                            int offsetZ = lz + warpZ * t * 4.0f;

                            for (int ox = -SHAFT_RADIUS; ox <= SHAFT_RADIUS; ox++)
                            {
                                for (int oz = -SHAFT_RADIUS; oz <= SHAFT_RADIUS; oz++)
                                {
                                    if (ox * ox + oz * oz > SHAFT_RADIUS * SHAFT_RADIUS)
                                        continue;

                                    int wx = offsetX + ox;
                                    int wz = offsetZ + oz;

                                    if (wx < 0 || wx >= CHUNK_WIDTH || wz < 0 || wz >= CHUNK_DEPTH)
                                        continue;

                                    int entryIdx = getVoxelIndex(wx, ly + dy, wz);

                                    if (!voxels[idx]->isBlockCarvable())
                                        continue;

                                    if (dy > depth - 3)
                                    {
                                        if (ox * ox + oz * oz <= (ENTRY_RADIUS * ENTRY_RADIUS) * (1.0f - t))
                                        {
                                            if (voxels[entryIdx] == createBlockById(3))
                                                voxels[entryIdx] = createBlockById(4);
                                            else
                                                voxels[entryIdx] = createBlockById(0);
                                        }
                                    }
                                    else
                                    {
                                        voxels[entryIdx] = createBlockById(0);
                                    }
                                }
                            }
                        }
                    }
                }
                if ((lx == 8 || lz == 8) && mask > 0.7f && rand() % 200 == 0)
                {
                    float dx = lx - 8, dz = lz - 8, dy = ly - (CAVE_MAX_Y - CAVE_MIN_Y) / 2;
                    float dist = dx * dx + dz * dz + dy * dy;

                    if (dist < 9.0f)
                    {
                        voxels[idx] = createBlockById(0);

                    }
                }
            }
        }
    }

}
void Chunk::generateTree(int x, int y, int z)
{
    const int TRUNK_HEIGHT = 6;
    const int MAX_LEAF_LAYERS = 3;

    for (int dy = 0; dy <= TRUNK_HEIGHT; dy++)
    {
        if (y + dy >= CHUNK_HEIGHT)
            break;
        int idx = getVoxelIndex(x, y + dy, z);
        if (voxels[idx] == createBlockById(0) || voxels[idx] == createBlockById(3))
        { 
            voxels[idx] = createBlockById(10);
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
                if (voxels[idx] == createBlockById(0)) 
                {
                    voxels[idx] = createBlockById(9);
                }
            }
        }
    }
}
Chunk::~Chunk() 
{
    delete[] voxels;
}