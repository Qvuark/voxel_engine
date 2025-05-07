#ifndef CHUNKS_H
#define CHUNKS_H

class Chunk;
class Voxel;

class Chunks
{
public:
	Chunk** chunks;
	size_t volume;
	unsigned int width, height, depth;
	
	Chunks(int w, int h, int d);
	Voxel* getVoxel(int x, int y, int z);
	~Chunks();
};

#endif