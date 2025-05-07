#include "chunk.h"
#include "voxels.h"
#include <math.h>

inline int getVoxelIndex(int lx, int ly, int lz)
{
    return (ly * CHUNK_DEPTH + lz) * CHUNK_WIDTH + lx;
}


Chunk::Chunk(int xpos, int ypos, int zpos) : x(xpos), y(ypos), z(zpos) 
{
	voxels = new Voxel[CHUNK_VOLUME];
    for (int ly = 0; ly < CHUNK_HEIGHT; ly++) 
    {
        for (int lz = 0; lz < CHUNK_DEPTH; lz++) 
        {
            for (int lx = 0; lx < CHUNK_WIDTH; lx++) 
            {
                int realX = lx + x * CHUNK_WIDTH;
                int realY = ly + y * CHUNK_HEIGHT;
                int realZ = lz + z * CHUNK_DEPTH;

                int id = realY <= noiseGenerator(realX, realY, realZ);
                if (realY <= 2) id = 2;

                voxels[getVoxelIndex(lx, ly, lz)].id = id;
            }
        }
    }
}
int Chunk::noiseGenerator(int realX, int realY, int realZ)
{
	return (sin(realX * 0.4f) * 0.9f + 0.6f) * 10;
}
Chunk::~Chunk() 
{
	delete[] voxels;
}