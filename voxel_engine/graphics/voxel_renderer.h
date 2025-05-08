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

	Mesh* render(Chunk* chunk, const Chunk** chunks);
};

#endif