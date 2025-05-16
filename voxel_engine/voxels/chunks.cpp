#include "chunks.h"
#include "chunk.h"
#include "voxels.h"
#include "../blocks/BlockRegister.h"

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

IBlock* Chunks::getVoxel(int x, int y, int z)
{
	auto calculateChunkIndex = [](int coord, int chunkSize) {
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

	size_t idx = (ly * CHUNK_DEPTH + lz) * CHUNK_WIDTH + lx;

	return chunk->voxels[idx].get();
}
Chunk* Chunks::getChunk(int x, int y, int z) 
{
	if (x < 0 || y < 0 || z < 0 || x >= width || y >= height || z >= depth)
		return nullptr;
	return chunks[(y * depth + z) * width + x];
}
void Chunks::setVoxel(int x, int y, int z, int id)
{
	auto calculateChunkIndex = [](int coord, int chunkSize)	{
			return (coord < 0) ? (coord - (chunkSize - 1)) / chunkSize : coord / chunkSize;
	};
	int cx = calculateChunkIndex(x, CHUNK_WIDTH);
	int cy = calculateChunkIndex(y, CHUNK_HEIGHT);
	int cz = calculateChunkIndex(z, CHUNK_DEPTH);;

	if (cx < 0 || cy < 0 || cz < 0 || cx >= width || cy >= height || cz >= depth)
		return;
	Chunk* chunk = chunks[(cy * depth + cz) * width + cx];

	int lx = (x % CHUNK_WIDTH + CHUNK_WIDTH) % CHUNK_WIDTH;
	int ly = (y % CHUNK_HEIGHT + CHUNK_HEIGHT) % CHUNK_HEIGHT;
	int lz = (z % CHUNK_DEPTH + CHUNK_DEPTH) % CHUNK_DEPTH;

	chunk->voxels[id] = createBlockById(id);
	chunk->isModified = true;

	if (lx == 0)
	{
		if (Chunk* neighbor = getChunk(cx - 1, cy, cz))
		{
			neighbor->isModified = true;
		}
	}
	if (ly == 0)
	{
		if (Chunk* neighbor = getChunk(cx, cy - 1, cz))
		{
			neighbor->isModified = true;
		}
	}
	if (lz == 0)
	{
		if (Chunk* neighbor = getChunk(cx, cy, cz - 1))
		{
			neighbor->isModified = true;
		}
	}
	if (lx == CHUNK_WIDTH - 1)
	{
		if (Chunk* neighbor = getChunk(cx + 1, cy, cz))
		{
			neighbor->isModified = true;
		}
	}
	if (ly == CHUNK_HEIGHT - 1)
	{
		if (Chunk* neighbor = getChunk(cx, cy + 1, cz))
		{
			neighbor->isModified = true;
		}
	}
	if (lz == CHUNK_DEPTH - 1)
	{
		if (Chunk* neighbor = getChunk(cx, cy, cz + 1))
		{
			neighbor->isModified = true;
		}
	}
}
IBlock* Chunks::pointerRay(glm::vec3 a, glm::vec3 dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend)
{
	if (glm::length(dir) != 0.0f)
	{
		dir = glm::normalize(dir);
	}

	float px = a.x;
	float py = a.y;
	float pz = a.z;

	float dx = dir.x;
	float dy = dir.y;
	float dz = dir.z;

	float t = 0.0f;

	const float epsilon = 0.0001f;

	int ix = static_cast<int>(floor(px + epsilon * (dx > 0 ? 1 : -1)));
	int iy = static_cast<int>(floor(py + epsilon * (dy > 0 ? 1 : -1)));
	int iz = static_cast<int>(floor(pz + epsilon * (dz > 0 ? 1 : -1)));

	float stepx = (dx > 0.0f) ? 1.0f : -1.0f;
	float stepy = (dy > 0.0f) ? 1.0f : -1.0f;
	float stepz = (dz > 0.0f) ? 1.0f : -1.0f;

	float infinity = std::numeric_limits<float>::infinity();

	float txDelta = (dx == 0.0f) ? infinity : abs(1.0f / dx);
	float tyDelta = (dy == 0.0f) ? infinity : abs(1.0f / dy);
	float tzDelta = (dz == 0.0f) ? infinity : abs(1.0f / dz);

	float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
	float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
	float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);

	float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
	float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
	float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;

	int steppedIndex = -1;

	while (t <= maxDist)
	{
		IBlock* voxel = getVoxel(ix, iy, iz);

		if (voxel == nullptr || voxel->getBlockId())
		{
			end.x = px + t * dx;
			end.y = py + t * dy;
			end.z = pz + t * dz;

			iend.x = ix;
			iend.y = iy;
			iend.z = iz;

			norm.x = norm.y = norm.z = 0.0f;

			if (steppedIndex == 0) norm.x = -stepx;
			if (steppedIndex == 1) norm.y = -stepy;
			if (steppedIndex == 2) norm.z = -stepz;

			return voxel;
		}

		if (txMax < tyMax && txMax < tzMax)
		{
			ix += stepx;
			t = txMax;
			txMax += txDelta;
			steppedIndex = 0;
		}
		else if (tyMax < tzMax)
		{
			iy += stepy;
			t = tyMax;
			tyMax += tyDelta;
			steppedIndex = 1;
		}
		else
		{
			iz += stepz;
			t = tzMax;
			tzMax += tzDelta;
			steppedIndex = 2;
		}
	}

	iend.x = ix;
	iend.y = iy;
	iend.z = iz;

	end.x = px + t * dx;
	end.y = py + t * dy;
	end.z = pz + t * dz;

	norm.x = norm.y = norm.z = 0.0f;

	return nullptr;


}