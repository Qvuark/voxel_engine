#ifndef CHUNKS_H
#define CHUNKS_H

#include <glm/glm.hpp>
#include "../blocks/BlockRegister.h"

class Chunk;
class Voxel;

class Chunks
{
private:
	Chunk** chunks;
	size_t volume;
	unsigned int width, height, depth;
public:
	Chunks(int w, int h, int d);
	IBlock* getVoxel(int x, int y, int z);
	Chunk* getChunk(int x, int y, int z);
	size_t getVolume() const;
	Chunk* getChunkById(int id) const;
	void setVoxel(int x, int y, int z, IBlock* block);
	IBlock* pointerRay(glm::vec3 start, glm::vec3 dir, float maxLength, glm::vec3 & end, glm::vec3& norm, glm::vec3& iend);
	~Chunks();
};

#endif