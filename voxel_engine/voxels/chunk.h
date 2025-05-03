#ifndef CHUNK_H
#define CHUNK_H

#include <cstdint>

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 16
#define CHUNK_DEPTH 16
#define CHUNK_VOLUME (CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH)

#define NOISE_FREQUENCY 0.3f
#define NOISE_AMPLITUDE 0.5f
#define SURFACE_SCALE 10
#define STONE_LAYER_HEIGHT 2

struct Voxel;

struct chunk_attributes
{
	int pos;
	int uv;
	int lightning;
};
class Chunk
{
public:
	Voxel *voxels;
	Chunk();
	~Chunk();
	int calculateHeight(int x, int z) const;
	uint8_t determineVoxelId(int x, int y, int z) const;
	int getVoxelIndex(int x, int y, int z);
};

#endif 