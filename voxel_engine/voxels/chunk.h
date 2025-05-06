#ifndef CHUNK_H
#define CHUNK_H

#include <cstdint>

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 16
#define CHUNK_DEPTH 16
#define CHUNK_VOLUME (CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH)



struct Voxel;

class Chunk
{
public:
	static float NOISE_FREQUENCY;
	static float NOISE_AMPLITUDE;
	static int SURFACE_SCALE;
	static int STONE_LAYER_HEIGHT;

	int x, y, z;
	Voxel* voxels;
	Chunk(int x, int y, int z);
	~Chunk();
	int calculateHeight(int x, int z) const;
	uint8_t determineVoxelId(int x, int y, int z) const;
	int getVoxelIndex(int x, int y, int z);
};

#endif 