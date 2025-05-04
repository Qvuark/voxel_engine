#include "chunk.h"
#include "voxels.h"
#include <math.h>

Chunk::Chunk(int xpos, int ypos, int zpos) : x(xpos), y(ypos), z(zpos)
{
    voxels = new Voxel[CHUNK_VOLUME];
    for (int y = 0; y < CHUNK_HEIGHT; y++) 
    {
        for (int z = 0; z < CHUNK_DEPTH; z++) 
        {
            for (int x = 0; x < CHUNK_WIDTH; x++)
            {
                int index = getVoxelIndex(x, y, z);
                if (index != -1) 
                {
                    voxels[index].id = determineVoxelId(x, y, z);
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
    return x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH;
}