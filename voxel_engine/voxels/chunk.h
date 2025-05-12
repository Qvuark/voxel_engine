#ifndef CHUNK_H
#define CHUNK_H

#include <cstdint>

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 128
#define CHUNK_DEPTH 16
#define CHUNK_VOLUME (CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH)



class Voxel;

class Chunk
{
public:

	int x, y, z;
	bool isModified = true;
	Voxel* voxels;

	Chunk(int xpos, int ypos, int zpos);
	void generateTree(int x, int y, int z);
	int noiseGenerator(int realX, int realY, int realZ);
	uint8_t determineVoxelId(int realX, int realY, int realZ);
	~Chunk();
};

#endif 