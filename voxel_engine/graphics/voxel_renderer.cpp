#include "voxel_renderer.h"
#include "mesh.h"
#include "../voxels/chunk.h"
#include "../voxels/voxels.h"

#define VERTEX_SIZE (3 + 2 + 1)

#define IS_IN_CHUNK(X,Y,Z) ((X) >= 0 && (X) < CHUNK_WIDTH && (Y) >= 0 && (Y) < CHUNK_HEIGHT && (Z) >= 0 && (Z) < CHUNK_DEPTH)
#define VOXEL(X,Y,Z) (chunk->voxels[((Y) * CHUNK_DEPTH + (Z)) * CHUNK_WIDTH + (X)])
#define IS_BLOCKED(X,Y,Z) ((IS_IN_CHUNK(X, Y, Z)) && VOXEL(X, Y, Z).id)

#define VERTEX(INDEX, X,Y,Z, U,V, L) buffer[INDEX+0] = (X);\
								  buffer[INDEX+1] = (Y);\
								  buffer[INDEX+2] = (Z);\
								  buffer[INDEX+3] = (U);\
								  buffer[INDEX+4] = (V);\
								  buffer[INDEX+5] = (L);\
								  INDEX += VERTEX_SIZE;

VoxelRenderer::VoxelRenderer(size_t capacity) : capacity(capacity)
{
    buffer.resize(capacity * VERTEX_SIZE * 6);
}

VoxelRenderer::~VoxelRenderer() {}

const std::vector<std::tuple<int, int, int, float>> directions = 
{
    {0, 1, 0, 1.0f},   
    {0, -1, 0, 0.75f}, 
    {1, 0, 0, 0.95f},  
    {-1, 0, 0, 0.85f}, 
    {0, 0, 1, 0.9f},   
    {0, 0, -1, 0.8f}, 
};
void VoxelRenderer::addVoxelFace(std::vector<float>& buffer, size_t& index, float x, float y, float z, float u, float v, float light, int dx, int dy, int dz) const
{
    const float half = 0.5f;
    const float uvsize = 1.0f / 16.0f;

    float cx = x + dx * half;
    float cy = y + dy * half;
    float cz = z + dz * half;

    if (dy == 1) 
    {
        VERTEX(index, cx - half, cy, cz - half, u + uvsize, v, light);
        VERTEX(index, cx - half, cy, cz + half, u + uvsize, v + uvsize, light);
        VERTEX(index, cx + half, cy, cz + half, u, v + uvsize, light);

        VERTEX(index, cx - half, cy, cz - half, u + uvsize, v, light);
        VERTEX(index, cx + half, cy, cz + half, u, v + uvsize, light);
        VERTEX(index, cx + half, cy, cz - half, u, v, light);
    }
    else if (dy == -1) 
    {
        VERTEX(index, cx - half, cy, cz - half, u, v, light);
        VERTEX(index, cx + half, cy, cz + half, u + uvsize, v + uvsize, light);
        VERTEX(index, cx - half, cy, cz + half, u, v + uvsize, light);

        VERTEX(index, cx - half, cy, cz - half, u, v, light);
        VERTEX(index, cx + half, cy, cz - half, u + uvsize, v, light);
        VERTEX(index, cx + half, cy, cz + half, u + uvsize, v + uvsize, light);
    }
    else if (dx == 1) 
    {
        VERTEX(index, cx, cy - half, cz - half, u + uvsize, v, light);
        VERTEX(index, cx, cy + half, cz - half, u + uvsize, v + uvsize, light);
        VERTEX(index, cx, cy + half, cz + half, u, v + uvsize, light);

        VERTEX(index, cx, cy - half, cz - half, u + uvsize, v, light);
        VERTEX(index, cx, cy + half, cz + half, u, v + uvsize, light);
        VERTEX(index, cx, cy - half, cz + half, u, v, light);
    }
    else if (dx == -1) 
    {
        VERTEX(index, cx, cy - half, cz - half, u, v, light);
        VERTEX(index, cx, cy + half, cz + half, u + uvsize, v + uvsize, light);
        VERTEX(index, cx, cy + half, cz - half, u, v + uvsize, light);

        VERTEX(index, cx, cy - half, cz - half, u, v, light);
        VERTEX(index, cx, cy - half, cz + half, u + uvsize, v, light);
        VERTEX(index, cx, cy + half, cz + half, u + uvsize, v + uvsize, light);
    }
    else if (dz == 1) 
    {
        VERTEX(index, cx - half, cy - half, cz, u, v, light);
        VERTEX(index, cx + half, cy + half, cz, u + uvsize, v + uvsize, light);
        VERTEX(index, cx - half, cy + half, cz, u, v + uvsize, light);

        VERTEX(index, cx - half, cy - half, cz, u, v, light);
        VERTEX(index, cx + half, cy - half, cz, u + uvsize, v, light);
        VERTEX(index, cx + half, cy + half, cz, u + uvsize, v + uvsize, light);
    }
    else if (dz == -1) 
    {
        VERTEX(index, cx - half, cy - half, cz, u + uvsize, v, light);
        VERTEX(index, cx - half, cy + half, cz, u + uvsize, v + uvsize, light);
        VERTEX(index, cx + half, cy + half, cz, u, v + uvsize, light);

        VERTEX(index, cx - half, cy - half, cz, u + uvsize, v, light);
        VERTEX(index, cx + half, cy + half, cz, u, v + uvsize, light);
        VERTEX(index, cx + half, cy - half, cz, u, v, light);
    }
}
Mesh* VoxelRenderer::render(Chunk* chunk)
{
    size_t index = 0; 
    
    for (int y = 0; y < CHUNK_HEIGHT; y++) 
    {
        for (int z = 0; z < CHUNK_DEPTH; z++) 
        {
            for (int x = 0; x < CHUNK_WIDTH; x++) 
            {
                Voxel& vox = VOXEL(x, y, z);
                if (!vox.id) continue;

                float uvsize = 1.0f / 16.0f;
                float u = (vox.id % 16) * uvsize;
                float v = 1 - ((1 + vox.id / 16) * uvsize);

                for (const auto& [dx, dy, dz, light] : directions) 
                {
                    if (!isVoxelBlocked(chunk, x + dx, y + dy, z + dz)) 
                    {
                        addVoxelFace(buffer, index, float(x), float(y), float(z), u, v, light, dx, dy, dz);
                    }
                }
            }
        }
    }
    return new Mesh(buffer, buffer.size() / VERTEX_SIZE, attributes);
}

bool VoxelRenderer::isVoxelBlocked(Chunk* chunk, int x, int y, int z) const
{
    if (x < 0 || x >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_DEPTH)
        return false;
    return VOXEL(x, y, z).id != 0;
}