#ifndef VOXEL_RENDERER_H
#define VOXEL_RENDERER_H


#include <vector>
#include <cstdlib>

class Mesh;
class Chunk;

static std::vector<int> attributes = { 3,2,1 };

class VoxelRenderer
{
public:
	std::vector<float> buffer;
	size_t capacity;
	VoxelRenderer(size_t capacity);
	~VoxelRenderer();
	bool isVoxelBlocked(Chunk* chunk, int x, int y, int z) const;
	void addVoxelFace(std::vector<float>& buffer, size_t &index,
		float x, float y, float z,
		float u, float v, float light,
		int dx, int dy, int dz) const;
	Mesh* render(Chunk* chunk);
};

#endif
