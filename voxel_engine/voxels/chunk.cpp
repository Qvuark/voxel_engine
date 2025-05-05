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
