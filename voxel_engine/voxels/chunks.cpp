#include "chunks.h"
#include "chunk.h"
#include "voxels.h"

Chunks::Chunks(int w, int h, int d) : width(w), height(h), depth(d)
{
	volume = width * height * depth;
	chunks = new Chunk*[volume];

	int index = 0;
	for (int y = 0; y < height; y++)
	{
		for (int z = 0; z < depth; z++)
		{
			for (int x = 0; x < width; x++, index++)
			{
				Chunk* chunk = new Chunk(x,y,z);
		    	chunks[index] = chunk;
			}
		}
	}
}
Voxel* Chunks::getVoxel(int x, int y, int z)
{
	int cx = x / CHUNK_WIDTH;
	int cy = y / CHUNK_HEIGHT;
	int cz = z / CHUNK_DEPTH;
	if(x < 0) cx--;
	if (y < 0) cy--;
	if (z < 0) cz--;
	if (cx < 0 || cy < 0 || cz < 0 || cx >= width || cy >= height || cz >= depth)
		return nullptr;
	Chunk* chunk = chunks[(cy * depth + cz) * width + cx];
	int lx = x - cx * CHUNK_WIDTH;
	int ly = y - cy * CHUNK_HEIGHT;
	int lz = z - cz * CHUNK_DEPTH;
	return &chunk->voxels[(ly * CHUNK_DEPTH + lz) * CHUNK_WIDTH + lx];
}
Chunks::~Chunks()
{
	for (int i = 0; i < volume; i++)
	{
		delete chunks[i];
	}
	delete[] chunks;
}
