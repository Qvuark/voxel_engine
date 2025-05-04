#include "chunks.h"
#include "chunk.h"

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

