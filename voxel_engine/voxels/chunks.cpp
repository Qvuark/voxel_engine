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
Chunks::~Chunks()
{
	for (int i = 0; i < volume; i++)
	{
		delete chunks[i];
	}
	delete[] chunks;
}

Voxel* Chunks::getVoxel(int x, int y, int z)
{
	auto calculateChunkIndex = [](int coord, int chunkSize) 
	{
			return (coord < 0) ? (coord - (chunkSize - 1)) / chunkSize : coord / chunkSize;
	};
	int cx = calculateChunkIndex(x, CHUNK_WIDTH);
	int cy = calculateChunkIndex(y, CHUNK_HEIGHT);
	int cz = calculateChunkIndex(z, CHUNK_DEPTH);;
	
	if (cx < 0 || cy < 0 || cz < 0 || cx >= width || cy >= height || cz >= depth)
		return nullptr;
	Chunk* chunk = chunks[(cy * depth + cz) * width + cx];
	if (!chunk)
		return nullptr;

	int lx = (x % CHUNK_WIDTH + CHUNK_WIDTH) % CHUNK_WIDTH;
	int ly = (y % CHUNK_HEIGHT + CHUNK_HEIGHT) % CHUNK_HEIGHT;
	int lz = (z % CHUNK_DEPTH + CHUNK_DEPTH) % CHUNK_DEPTH;

	return &chunk->voxels[(ly * CHUNK_DEPTH + lz) * CHUNK_WIDTH + lx];
}
Chunk* Chunks::getChunk(int x, int y, int z) 
{
	if (x < 0 || y < 0 || z < 0 || x >= width || y >= height || z >= depth)
		return nullptr;
	return chunks[(y * depth + z) * width + x];
}