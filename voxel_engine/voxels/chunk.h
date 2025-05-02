#ifndef CHUNK_H
#define CHUNK_H

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 16
#define CHUNK_DEPTH 16
#define CHUNK_VOLUME (CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH)

#define NOISE_FREQUENCY 0.3f
#define NOISE_AMPLITUDE 0.5f
#define SURFACE_SCALE 10
#define STONE_LAYER_HEIGHT 2

struct Voxel;

namespace VoxelType 
{
	constexpr uint8_t AIR = 0;
	constexpr uint8_t DIRT = 1;
	constexpr uint8_t STONE = 2;
}

class Chunk
{
public:
	Voxel *voxels;
	Chunk();
	~Chunk();
	int getVoxelIndex(int x, int y, int z);
};

#endif 