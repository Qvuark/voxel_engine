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
    IS_IN(X, Y, Z) && (VOXEL(X, Y, Z).id != 0) \
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

}
