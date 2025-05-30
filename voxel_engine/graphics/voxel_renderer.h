#ifndef VOXEL_RENDERER_H
#define VOXEL_RENDERER_H


#include <vector>
#include <cstdlib>

class Mesh;
class Chunk;

class VoxelRenderer
{
private:
	std::vector<float> buffer;
	size_t capacity;
public:
	VoxelRenderer(size_t capacity);
	~VoxelRenderer();

	Mesh* render(Chunk* chunk, const Chunk** chunks, bool ambientOcclusion, bool isDay);
};

#endif