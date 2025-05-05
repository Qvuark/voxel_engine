#include "chunk.h"
#include "voxels.h"
#include <math.h>

float Chunk::NOISE_FREQUENCY = 0.4f;
float Chunk::NOISE_AMPLITUDE = 0.8f;
int Chunk::SURFACE_SCALE = 10;
int Chunk::STONE_LAYER_HEIGHT = 1;

Chunk::Chunk(int xpos, int ypos, int zpos) : x(xpos), y(ypos), z(zpos) 
{
    voxels = new Voxel[CHUNK_VOLUME];
    for (int ly = 0; ly < CHUNK_HEIGHT; ly++) 
    {  
        for (int lz = 0; lz < CHUNK_DEPTH; lz++) 
        {
            for (int lx = 0; lx < CHUNK_WIDTH; lx++) 
            {
                int globalX = lx + x * CHUNK_WIDTH;
                int globalZ = lz + z * CHUNK_DEPTH;
                int index = getVoxelIndex(lx, ly, lz);
                if (index != -1) 
                {
                    voxels[index].id = determineVoxelId(globalX, ly, globalZ);
                }
            }
        }
    }
}

Chunk::~Chunk()
{
    delete[] voxels;
}
int Chunk::calculateHeight(int x, int z) const
{
    float noise = (sin(x * NOISE_FREQUENCY) * NOISE_AMPLITUDE + NOISE_AMPLITUDE);
    return static_cast<int>(noise * SURFACE_SCALE);
}
uint8_t Chunk::determineVoxelId(int x, int y, int z) const
{
    const int surfaceHeight = calculateHeight(x, z);
    uint8_t id = 0;
    if (y <= surfaceHeight)
    {
        id = (y <= STONE_LAYER_HEIGHT) ? 2 : 1;
    }
    return id;
}
int Chunk::getVoxelIndex(int x, int y, int z)
{
    if (x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_DEPTH)
        return -1; 
    return x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
}