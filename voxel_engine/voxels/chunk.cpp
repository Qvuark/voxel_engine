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

				int id = realY <= (sin(realX * 0.1f) * 0.5f + 0.5f) * 10;
				if (realY <= 2)
					id = 2;
				voxels[(y * CHUNK_DEPTH + z) * CHUNK_WIDTH + x].id = id;
			}
		}
	}
}

Chunk::~Chunk() {
	delete[] voxels;
}