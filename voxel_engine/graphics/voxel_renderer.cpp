#include "voxel_renderer.h"
#include "mesh.h"
#include "../voxels/chunk.h"
#include "../voxels/voxels.h"

#define IS_IN_CHUNK(X,Y,Z) ( \
    ((X) >= 0) && ((X) < CHUNK_WIDTH)  && \
    ((Y) >= 0) && ((Y) < CHUNK_HEIGHT) && \
    ((Z) >= 0) && ((Z) < CHUNK_DEPTH)  \
)
#define VOXEL(X, Y, Z) ( \
    chunk->voxels[(X) + (Z) * CHUNK_WIDTH + (Y) * CHUNK_WIDTH * CHUNK_DEPTH] \
)
#define IS_VOXEL_BLOCKED(X, Y, Z) ( \
    IS_IN_CHUNK(X, Y, Z) && (VOXEL(X, Y, Z).id != 0) \
)
#define VERTEX(INDEX, X,Y,Z, U,V, L) buffer[INDEX+0] = (X);\
								  buffer[INDEX+1] = (Y);\
								  buffer[INDEX+2] = (Z);\
								  buffer[INDEX+3] = (U);\
								  buffer[INDEX+4] = (V);\
								  buffer[INDEX+5] = (L);\
								  INDEX += VERTEX_SIZE;
#define VERTEX_SIZE (3 + 2 + 1)

VoxelRenderer::VoxelRenderer(size_t capacity) : capacity(capacity)
{
	buffer.resize(capacity * VERTEX_SIZE * 6);
}
VoxelRenderer::~VoxelRenderer() {}

Mesh* render(Chunk* chunk)
{
    for (int y = 0; y < CHUNK_HEIGHT; y++)
    {
        for (int z = 0; z < CHUNK_DEPTH; z++)
        {
            for (int x = 0; x < CHUNK_WIDTH; x++)
            {
                Voxel& vox = chunk->voxels[x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH];
                if (!vox.id) continue;

                float uvsize = 1.0f / 32.0f;
                float u = (vox.id % 32) * uvsize;
                float v = 1 - ((1 + vox.id / 32) * uvsize);

                auto addVoxelsFace = [&](int dx, int dy, int dz, float light) 
                    {
                    if (!IS_VOXEL_BLOCKED(x + dx, y + dy, z + dz)) 
                    {
                       
                    };
            }
        }
    }
    return new Mesh(buffer.data(), buffer.size() / VERTEX_SIZE, attributes);
}
bool VoxelRenderer::isBlocked(Chunk * chunk, int x, int y, int z) const
{
        if (x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_DEPTH)
            return false;
        return chunk->getData()[x + z * CHUNK_WIDTH + y * CHUNK_WIDTH * CHUNK_DEPTH].id != 0;
}