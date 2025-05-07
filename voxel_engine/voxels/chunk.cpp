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
				int realX = x + this->x * CHUNK_WIDTH;
				int realY = y + this->y * CHUNK_HEIGHT;
				int realZ = z + this->z * CHUNK_DEPTH;

				int id = realY <= noiseGenerator(realX, realY, realZ);
				if (realY <= 2)
					id = 2;
				voxels[(y * CHUNK_DEPTH + z) * CHUNK_WIDTH + x].id = id;
			}
		}
	}
}

int Chunk::noiseGenerator(int realX, int realY, int realZ)
{
	return (sin(realX * 0.4f) * 0.9f + 0.6f) * 10;
}
Chunk::~Chunk() {
	delete[] voxels;
}