﻿#include "voxel_renderer.h"
#include "Mesh.h"
#include "../voxels/chunk.h"
#include "../voxels/voxels.h"
#include <memory>

#define VERTEX_SIZE (3 + 2 + 1 + 3)

inline int cdiv(int X, int A) 
{
	return (X < 0) ? ((X) / (A)-1) : (X / A);
}

inline int local_neg(int X, int SIZE)
{
	return (X < 0) ? (SIZE + X) : X;
}

inline int local(int X, int SIZE) 
{
	return (X >= SIZE) ? (X - SIZE) : local_neg(X, SIZE);
}

inline const Chunk* get_chunk(int X, int Y, int Z, const Chunk** chunks) 
{
	int cy = cdiv(Y, CHUNK_HEIGHT) + 1;
	int cz = cdiv(Z, CHUNK_DEPTH) + 1;
	int cx = cdiv(X, CHUNK_WIDTH) + 1;
	int index = (cy * 3 + cz) * 3 + cx;
	return chunks[index];
}

inline bool is_chunk(int X, int Y, int Z, const Chunk** chunks) 
{
	return get_chunk(X, Y, Z, chunks) != nullptr;
}

inline IBlock* voxel(int X, int Y, int Z, const Chunk** chunks)
{
	const Chunk* chunk = get_chunk(X, Y, Z, chunks);
	int lx = local(X, CHUNK_WIDTH);
	int ly = local(Y, CHUNK_HEIGHT);
	int lz = local(Z, CHUNK_DEPTH);
	return chunk->getBlock(lx, ly, lz);
}
inline bool is_blocked(int X, int Y, int Z, const Chunk** chunks, bool isDay)
{
    if (!is_chunk(X, Y, Z, chunks))
        return true;

    IBlock* block = voxel(X, Y, Z, chunks);
    if (!block)
        return false; 

    return block->getBlockId() != 0;
}

inline size_t add_vertex(float* buffer, size_t index, float x, float y, float z, float u, float v, float l, float r, float g, float b)
{
	buffer[index] = x;
	buffer[index + 1] = y;
	buffer[index + 2] = z;
	buffer[index + 3] = u;
	buffer[index + 4] = v;
	buffer[index + 5] = l;
    buffer[index + 6] = r;
    buffer[index + 7] = g;
    buffer[index + 8] = b;
	return index + VERTEX_SIZE;
}

static std::vector<int> attributes = { 3,2,1,3 };

VoxelRenderer::VoxelRenderer(size_t capacity) : capacity(capacity)
{
	buffer.resize(capacity * VERTEX_SIZE * 6);
}

VoxelRenderer::~VoxelRenderer() {}

Mesh* VoxelRenderer::render(Chunk* chunk, const Chunk** chunks, bool ambientOcclusion, bool isDay)
{
	size_t index = 0;
	float aoIntensity = 0.25f;

	for (int y = 0; y < CHUNK_HEIGHT; y++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				IBlock* block = chunk->getBlock(x,y,z);
				unsigned int id = block->getTextureId(isDay);
                glm::vec3 colorMult = block->getColorMultiplier(isDay);
                float rr = colorMult.r;
                float gg = colorMult.g;
                float bb = colorMult.b;

				if (!id)
					continue;

				float l;
				float uvsize = 1.0f / 16.0f;
				float u1 = (id % 16) * uvsize;
				float v1 = 1 - ((1 + id / 16) * uvsize);
				float u2 = u1 + uvsize;
				float v2 = v1 + uvsize;
				float a, b, c, d, e, f, g, h;
				a = b = c = d = e = f = g = h = 0.0f;

				if (!is_blocked(x, y + 1, z, chunks, isDay)) 
				{
					l = 1.0f;
					if (ambientOcclusion) 
					{
						a = is_blocked(x + 1, y + 1, z, chunks, isDay) * aoIntensity;
						b = is_blocked(x, y + 1, z + 1, chunks, isDay) * aoIntensity;
						c = is_blocked(x - 1, y + 1, z, chunks, isDay) * aoIntensity;
						d = is_blocked(x, y + 1, z - 1, chunks, isDay) * aoIntensity;

						e = is_blocked(x - 1, y + 1, z - 1, chunks, isDay) * aoIntensity;
						f = is_blocked(x - 1, y + 1, z + 1, chunks, isDay) * aoIntensity;
						g = is_blocked(x + 1, y + 1, z + 1, chunks, isDay) * aoIntensity;
						h = is_blocked(x + 1, y + 1, z - 1, chunks, isDay) * aoIntensity;
					}

					index = add_vertex(buffer.data(), index, x - 0.5f, y + 0.5f, z - 0.5f, u2, v1, l * (1.0f - c - d - e), rr,gg,bb);
					index = add_vertex(buffer.data(), index, x - 0.5f, y + 0.5f, z + 0.5f, u2, v2, l * (1.0f - c - b - f), rr, gg, bb);
					index = add_vertex(buffer.data(), index, x + 0.5f, y + 0.5f, z + 0.5f, u1, v2, l * (1.0f - a - b - g), rr, gg, bb);
					index = add_vertex(buffer.data(), index, x - 0.5f, y + 0.5f, z - 0.5f, u2, v1, l * (1.0f - c - d - e), rr, gg, bb);
					index = add_vertex(buffer.data(), index, x + 0.5f, y + 0.5f, z + 0.5f, u1, v2, l * (1.0f - a - b - g), rr, gg, bb);
					index = add_vertex(buffer.data(), index, x + 0.5f, y + 0.5f, z - 0.5f, u1, v1, l * (1.0f - a - d - h), rr, gg, bb);
				}
                if (!is_blocked(x, y - 1, z, chunks, isDay))
                {
                    l = 0.75f;
                    if (ambientOcclusion)
                    {
                        a = is_blocked(x + 1, y - 1, z, chunks, isDay) * aoIntensity;
                        b = is_blocked(x, y - 1, z + 1, chunks, isDay) * aoIntensity;
                        c = is_blocked(x - 1, y - 1, z, chunks, isDay) * aoIntensity;
                        d = is_blocked(x, y - 1, z - 1, chunks, isDay) * aoIntensity;
                        e = is_blocked(x - 1, y - 1, z - 1, chunks, isDay) * aoIntensity;
                        f = is_blocked(x - 1, y - 1, z + 1, chunks, isDay) * aoIntensity;
                        g = is_blocked(x + 1, y - 1, z + 1, chunks, isDay) * aoIntensity;
                        h = is_blocked(x + 1, y - 1, z - 1, chunks, isDay) * aoIntensity;
                    }
                    index = add_vertex(buffer.data(), index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, l * (1.0f - c - d - e), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x + 0.5f, y - 0.5f, z + 0.5f, u2, v2, l * (1.0f - a - b - g), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x - 0.5f, y - 0.5f, z + 0.5f, u1, v2, l * (1.0f - c - b - f), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, l * (1.0f - c - d - e), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x + 0.5f, y - 0.5f, z - 0.5f, u2, v1, l * (1.0f - a - d - h), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x + 0.5f, y - 0.5f, z + 0.5f, u2, v2, l * (1.0f - a - b - g), rr, gg, bb);
                }
                if (!is_blocked(x + 1, y, z, chunks, isDay))
                {
                    l = 0.95f;

                    if (ambientOcclusion) 
                    {
                        a = is_blocked(x + 1, y + 1, z, chunks, isDay) * aoIntensity;
                        b = is_blocked(x + 1, y, z + 1, chunks, isDay) * aoIntensity;
                        c = is_blocked(x + 1, y - 1, z, chunks, isDay) * aoIntensity;
                        d = is_blocked(x + 1, y, z - 1, chunks, isDay) * aoIntensity;
                        e = is_blocked(x + 1, y - 1, z - 1, chunks, isDay) * aoIntensity;
                        f = is_blocked(x + 1, y - 1, z + 1, chunks, isDay) * aoIntensity;
                        g = is_blocked(x + 1, y + 1, z + 1, chunks, isDay) * aoIntensity;
                        h = is_blocked(x + 1, y + 1, z - 1, chunks, isDay) * aoIntensity;
                    }
                    index = add_vertex(buffer.data(), index, x + 0.5f, y - 0.5f, z - 0.5f, u2, v1, l * (1.0f - c - d - e), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x + 0.5f, y + 0.5f, z - 0.5f, u2, v2, l * (1.0f - d - a - h), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x + 0.5f, y + 0.5f, z + 0.5f, u1, v2, l * (1.0f - a - b - g), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x + 0.5f, y - 0.5f, z - 0.5f, u2, v1, l * (1.0f - c - d - e), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x + 0.5f, y + 0.5f, z + 0.5f, u1, v2, l * (1.0f - a - b - g), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x + 0.5f, y - 0.5f, z + 0.5f, u1, v1, l * (1.0f - b - c - f), rr, gg, bb);
                }
                if (!is_blocked(x - 1, y, z, chunks, isDay))
                {
                    l = 0.85f;
                    a = b = c = d = e = f = g = h = 0.0f;

                    if (ambientOcclusion)
                    {
                        a = is_blocked(x - 1, y + 1, z, chunks, isDay) * aoIntensity;
                        b = is_blocked(x - 1, y, z + 1, chunks, isDay) * aoIntensity;
                        c = is_blocked(x - 1, y - 1, z, chunks, isDay) * aoIntensity;
                        d = is_blocked(x - 1, y, z - 1, chunks, isDay) * aoIntensity;
                        e = is_blocked(x - 1, y - 1, z - 1, chunks, isDay) * aoIntensity;
                        f = is_blocked(x - 1, y - 1, z + 1, chunks, isDay) * aoIntensity;
                        g = is_blocked(x - 1, y + 1, z + 1, chunks, isDay) * aoIntensity;
                        h = is_blocked(x - 1, y + 1, z - 1, chunks, isDay) * aoIntensity;
                    }

                    index = add_vertex(buffer.data(), index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, l * (1.0f - c - d - e), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x - 0.5f, y + 0.5f, z + 0.5f, u2, v2, l * (1.0f - a - b - g), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x - 0.5f, y + 0.5f, z - 0.5f, u1, v2, l * (1.0f - d - a - h), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, l * (1.0f - c - d - e), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x - 0.5f, y - 0.5f, z + 0.5f, u2, v1, l * (1.0f - b - c - f), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x - 0.5f, y + 0.5f, z + 0.5f, u2, v2, l * (1.0f - a - b - g), rr, gg, bb);
                }

                if (!is_blocked(x, y, z + 1, chunks, isDay))
                {
                    l = 0.9f;
                    a = b = c = d = e = f = g = h = 0.0f;

                    if (ambientOcclusion) 
                    {
                        a = is_blocked(x, y + 1, z + 1, chunks, isDay) * aoIntensity;
                        b = is_blocked(x + 1, y, z + 1, chunks, isDay) * aoIntensity;
                        c = is_blocked(x, y - 1, z + 1, chunks, isDay) * aoIntensity;
                        d = is_blocked(x - 1, y, z + 1, chunks, isDay) * aoIntensity;
                        e = is_blocked(x - 1, y - 1, z + 1, chunks, isDay) * aoIntensity;
                        f = is_blocked(x + 1, y - 1, z + 1, chunks, isDay) * aoIntensity;
                        g = is_blocked(x + 1, y + 1, z + 1, chunks, isDay) * aoIntensity;
                        h = is_blocked(x - 1, y + 1, z + 1, chunks, isDay) * aoIntensity;
                    }

                    index = add_vertex(buffer.data(), index, x - 0.5f, y - 0.5f, z + 0.5f, u1, v1, l * (1.0f - c - d - e), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x + 0.5f, y + 0.5f, z + 0.5f, u2, v2, l * (1.0f - a - b - g), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x - 0.5f, y + 0.5f, z + 0.5f, u1, v2, l * (1.0f - a - d - h), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x - 0.5f, y - 0.5f, z + 0.5f, u1, v1, l * (1.0f - c - d - e), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x + 0.5f, y - 0.5f, z + 0.5f, u2, v1, l * (1.0f - b - c - f), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x + 0.5f, y + 0.5f, z + 0.5f, u2, v2, l * (1.0f - a - b - g), rr, gg, bb);
                }
                if (!is_blocked(x, y, z - 1, chunks, isDay))
                {
                    l = 0.8f;
                    a = b = c = d = e = f = g = h = 0.0f;

                    if (ambientOcclusion)
                    {
                        a = is_blocked(x, y + 1, z - 1, chunks, isDay) * aoIntensity;
                        b = is_blocked(x + 1, y, z - 1, chunks, isDay) * aoIntensity;
                        c = is_blocked(x, y - 1, z - 1, chunks, isDay) * aoIntensity;
                        d = is_blocked(x - 1, y, z - 1, chunks, isDay) * aoIntensity;
                        e = is_blocked(x - 1, y - 1, z - 1, chunks, isDay) * aoIntensity;
                        f = is_blocked(x + 1, y - 1, z - 1, chunks, isDay) * aoIntensity;
                        g = is_blocked(x + 1, y + 1, z - 1, chunks, isDay) * aoIntensity;
                        h = is_blocked(x - 1, y + 1, z - 1, chunks, isDay) * aoIntensity;
                    }

                    index = add_vertex(buffer.data(), index, x - 0.5f, y - 0.5f, z - 0.5f, u2, v1, l * (1.0f - c - d - e), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x - 0.5f, y + 0.5f, z - 0.5f, u2, v2, l * (1.0f - a - d - h), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x + 0.5f, y + 0.5f, z - 0.5f, u1, v2, l * (1.0f - a - b - g), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x - 0.5f, y - 0.5f, z - 0.5f, u2, v1, l * (1.0f - c - d - e), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x + 0.5f, y + 0.5f, z - 0.5f, u1, v2, l * (1.0f - a - b - g), rr, gg, bb);
                    index = add_vertex(buffer.data(), index, x + 0.5f, y - 0.5f, z - 0.5f, u1, v1, l * (1.0f - b - c - f), rr, gg, bb);
                }
            }
        }
    }
    return new Mesh(buffer, index / VERTEX_SIZE, attributes);
}