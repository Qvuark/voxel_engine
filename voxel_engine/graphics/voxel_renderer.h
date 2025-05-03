#ifndef VOXEL_RENDERER_H
#define VOXEL_RENDERER_H

#include <vector>
#include <cstdlib>

class Mesh;
class Chunk;

class VoxelRenderer
{
public:
	std::vector<float> buffer;
	size_t capacity;
	VoxelRenderer(size_t capacity);
	~VoxelRenderer();
	bool isBlocked(Chunk* chunk, int x, int y, int z) const;
	void addFace(std::vector<float>& buffer,
		float x, float y, float z,
		float u, float v, float light,
		int dx, int dy, int dz) const;
	Mesh* render(Chunk* chunk);
};

#endif
