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

const std::vector<std::tuple<int, int, int, float>> directions = 
{
    {0, 1, 0, 1.0f},   
    {0, -1, 0, 0.75f}, 
    {1, 0, 0, 0.95f},  
    {-1, 0, 0, 0.85f}, 
    {0, 0, 1, 0.9f},   
    {0, 0, -1, 0.8f}, 
};

void VoxelRenderer::addVoxelFace(std::vector<float>& buffer, float x, float y, float z, float u, float v, float light, int dx, int dy, int dz) const
{
    static size_t index = 0;
    if (dx != 0) 
    {
        VERTEX(index, x, y - 0.5f, z - 0.5f, u, v, light)
        VERTEX(index, x, y + 0.5f, z - 0.5f, u, v + 1.0f / 32, light)
        VERTEX(index, x, y + 0.5f, z + 0.5f, u + 1.0f / 32, v + 1.0f / 32, light)
        
        VERTEX(index, x, y - 0.5f, z - 0.5f, u, v, light)
        VERTEX(index, x, y + 0.5f, z + 0.5f, u + 1.0f / 32, v + 1.0f / 32, light)
        VERTEX(index, x, y - 0.5f, z + 0.5f, u + 1.0f / 32, v, light)
    }
    else if (dy != 0) 
    {
        VERTEX(index, x - 0.5f, y, z - 0.5f, u, v, light)
        VERTEX(index, x - 0.5f, y, z + 0.5f, u, v + 1.0f / 32, light)
        VERTEX(index, x + 0.5f, y, z + 0.5f, u + 1.0f / 32, v + 1.0f / 32, light)

        VERTEX(index, x - 0.5f, y, z - 0.5f, u, v, light)
        VERTEX(index, x + 0.5f, y, z + 0.5f, u + 1.0f / 32, v + 1.0f / 32, light)
        VERTEX(index, x + 0.5f, y, z - 0.5f, u + 1.0f / 32, v, light)
    }
    else if (dz != 0) 
    {
        VERTEX(index, x - 0.5f, y - 0.5f, z, u, v, light)
        VERTEX(index, x - 0.5f, y + 0.5f, z, u, v + 1.0f / 32, light)
        VERTEX(index, x + 0.5f, y + 0.5f, z, u + 1.0f / 32, v + 1.0f / 32, light)

        VERTEX(index, x - 0.5f, y - 0.5f, z, u, v, light)
        VERTEX(index, x + 0.5f, y + 0.5f, z, u + 1.0f / 32, v + 1.0f / 32, light)
        VERTEX(index, x + 0.5f, y - 0.5f, z, u + 1.0f / 32, v, light)
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
                        addVoxelFace(buffer, x + dx * 0.5f, y + dy * 0.5f, z + dz * 0.5f, u, v, light, dx, dy, dz);
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