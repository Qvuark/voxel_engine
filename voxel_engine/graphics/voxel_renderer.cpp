#include "voxel_renderer.h"
#include "Mesh.h"
#include "../voxels/chunk.h"
#include "../voxels/voxels.h"

#define VERTEX_SIZE (3 + 2 + 1)

#define CDIV(X,A) ((X) < 0 ? (X) / (A) - 1 : (X) / (A))
#define LOCAL(X, SIZE) ((X % SIZE + SIZE) % SIZE) 
#define IS_CHUNK(X,Y,Z) (GET_CHUNK(X,Y,Z) != nullptr)
#define GET_CHUNK(X,Y,Z) (chunks[((CDIV(Y, CHUNK_HEIGHT)+1) * 3 + CDIV(Z, CHUNK_DEPTH) + 1) * 3 + CDIV(X, CHUNK_WIDTH) + 1])

#define VOXEL(X,Y,Z) (GET_CHUNK(X,Y,Z)->voxels[(LOCAL(Y, CHUNK_HEIGHT) * CHUNK_DEPTH + LOCAL(Z, CHUNK_DEPTH)) * CHUNK_WIDTH + LOCAL(X, CHUNK_WIDTH)])
#define IS_BLOCKED(X,Y,Z) ((!IS_CHUNK(X, Y, Z)) || VOXEL(X, Y, Z).id)

#define VERTEX(INDEX,X,Y,Z,U,V,L) buffer[INDEX+0] = (X);\
								  buffer[INDEX+1] = (Y);\
								  buffer[INDEX+2] = (Z);\
								  buffer[INDEX+3] = (U);\
								  buffer[INDEX+4] = (V);\
								  buffer[INDEX+5] = (L);\
								  INDEX += VERTEX_SIZE;

static std::vector<int> attributes = { 3,2,1,0 };

VoxelRenderer::VoxelRenderer(size_t capacity) : capacity(capacity) 
{
	buffer.resize(capacity * VERTEX_SIZE * 6);
}

VoxelRenderer::~VoxelRenderer() {}	

Mesh* VoxelRenderer::render(Chunk* chunk, const Chunk** chunks, bool ambientOcclusion) {
    size_t index = 0;
    float aoIntensity = 0.20f;

    const int chunkX = chunk->x * CHUNK_WIDTH;
    const int chunkY = chunk->y * CHUNK_HEIGHT;
    const int chunkZ = chunk->z * CHUNK_DEPTH;

    auto is_blocked = [&](int gx, int gy, int gz) {
        const Chunk* c = GET_CHUNK(gx, gy, gz);
        if (!c) return false;

        int lx = (gx - c->x * CHUNK_WIDTH + CHUNK_WIDTH) % CHUNK_WIDTH;
        int ly = (gy - c->y * CHUNK_HEIGHT + CHUNK_HEIGHT) % CHUNK_HEIGHT;
        int lz = (gz - c->z * CHUNK_DEPTH + CHUNK_DEPTH) % CHUNK_DEPTH;

        return c->voxels[(ly * CHUNK_DEPTH + lz) * CHUNK_WIDTH + lx].id != 0;
        };

    for (int y = 0; y < CHUNK_HEIGHT; y++) {
        for (int z = 0; z < CHUNK_DEPTH; z++) {
            for (int x = 0; x < CHUNK_WIDTH; x++) {
                Voxel vox = chunk->voxels[(y * CHUNK_DEPTH + z) * CHUNK_WIDTH + x];
                unsigned int id = vox.id;

                if (!id) continue;

                const int gx = chunkX + x;
                const int gy = chunkY + y;
                const int gz = chunkZ + z;

                float l;
                float uvsize = 1.0f / 16.0f;
                float u1 = (id % 16) * uvsize;
                float v1 = 1 - ((1 + id / 16) * uvsize;
                float u2 = u1 + uvsize;
                float v2 = v1 + uvsize;
                float a, b, c, d, e, f, g, h;

                // Top face
                if (!is_blocked(gx, gy + 1, gz)) {
                    l = 1.0f;
                    if (ambientOcclusion) {
                        a = is_blocked(gx + 1, gy + 1, gz) * aoIntensity;
                        b = is_blocked(gx, gy + 1, gz + 1) * aoIntensity;
                        c = is_blocked(gx - 1, gy + 1, gz) * aoIntensity;
                        d = is_blocked(gx, gy + 1, gz - 1) * aoIntensity;
                        e = is_blocked(gx - 1, gy + 1, gz - 1) * aoIntensity;
                        f = is_blocked(gx - 1, gy + 1, gz + 1) * aoIntensity;
                        g = is_blocked(gx + 1, gy + 1, gz + 1) * aoIntensity;
                        h = is_blocked(gx + 1, gy + 1, gz - 1) * aoIntensity;
                    }
                    VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u2, v1, l * (1.0f - c - d - e));
                    VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u2, v2, l * (1.0f - c - d - f));
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u1, v2, l * (1.0f - a - b - g));

                    VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u2, v1, l * (1.0f - c - d - e));
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u1, v2, l * (1.0f - a - b - g));
                    VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u1, v1, l * (1.0f - a - d - h));
                }

                // Bottom face
                if (!is_blocked(gx, gy - 1, gz)) {
                    l = 0.75f;
                    if (ambientOcclusion) {
                        a = is_blocked(gx + 1, gy - 1, gz) * aoIntensity;
                        b = is_blocked(gx, gy - 1, gz + 1) * aoIntensity;
                        c = is_blocked(gx - 1, gy - 1, gz) * aoIntensity;
                        d = is_blocked(gx, gy - 1, gz - 1) * aoIntensity;
                        e = is_blocked(gx - 1, gy - 1, gz - 1) * aoIntensity;
                        f = is_blocked(gx - 1, gy - 1, gz + 1) * aoIntensity;
                        g = is_blocked(gx + 1, gy - 1, gz + 1) * aoIntensity;
                        h = is_blocked(gx + 1, gy - 1, gz - 1) * aoIntensity;
                    }
                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, l * (1.0f - c - d - e));
                    VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u2, v2, l * (1.0f - a - b - g));
                    VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u1, v2, l * (1.0f - c - b - f));

                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, l * (1.0f - c - d - e));
                    VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u2, v1, l * (1.0f - a - d - h));
                    VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u2, v2, l * (1.0f - a - b - g));
                }

                // Right face (x+1)
                if (!is_blocked(gx + 1, gy, gz)) {
                    l = 0.95f;
                    if (ambientOcclusion) {
                        a = is_blocked(gx + 1, gy + 1, gz) * aoIntensity;
                        b = is_blocked(gx + 1, gy, gz + 1) * aoIntensity;
                        c = is_blocked(gx + 1, gy - 1, gz) * aoIntensity;
                        d = is_blocked(gx + 1, gy, gz - 1) * aoIntensity;
                        e = is_blocked(gx + 1, gy - 1, gz - 1) * aoIntensity;
                        f = is_blocked(gx + 1, gy - 1, gz + 1) * aoIntensity;
                        g = is_blocked(gx + 1, gy + 1, gz + 1) * aoIntensity;
                        h = is_blocked(gx + 1, gy + 1, gz - 1) * aoIntensity;
                    }
                    VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u2, v1, l * (1.0f - c - d - e));
                    VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u2, v2, l * (1.0f - d - a - h));
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u1, v2, l * (1.0f - a - b - g));

                    VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u2, v1, l * (1.0f - c - d - e));
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u1, v2, l * (1.0f - a - b - g));
                    VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u1, v1, l * (1.0f - b - c - f));
                }

                // Left face (x-1)
                if (!is_blocked(gx - 1, gy, gz)) {
                    l = 0.85f;
                    if (ambientOcclusion) {
                        a = is_blocked(gx - 1, gy + 1, gz) * aoIntensity;
                        b = is_blocked(gx - 1, gy, gz + 1) * aoIntensity;
                        c = is_blocked(gx - 1, gy - 1, gz) * aoIntensity;
                        d = is_blocked(gx - 1, gy, gz - 1) * aoIntensity;
                        e = is_blocked(gx - 1, gy - 1, gz - 1) * aoIntensity;
                        f = is_blocked(gx - 1, gy - 1, gz + 1) * aoIntensity;
                        g = is_blocked(gx - 1, gy + 1, gz + 1) * aoIntensity;
                        h = is_blocked(gx - 1, gy + 1, gz - 1) * aoIntensity;
                    }
                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, l * (1.0f - c - d - e));
                    VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u2, v2, l * (1.0f - a - b - g));
                    VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u1, v2, l * (1.0f - d - a - h));

                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, l * (1.0f - c - d - e));
                    VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u2, v1, l * (1.0f - b - c - f));
                    VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u2, v2, l * (1.0f - a - b - g));
                }

                // Front face (z+1)
                if (!is_blocked(gx, gy, gz + 1)) {
                    l = 0.9f;
                    if (ambientOcclusion) {
                        a = is_blocked(gx, gy + 1, gz + 1) * aoIntensity;
                        b = is_blocked(gx + 1, gy, gz + 1) * aoIntensity;
                        c = is_blocked(gx, gy - 1, gz + 1) * aoIntensity;
                        d = is_blocked(gx - 1, gy, gz + 1) * aoIntensity;
                        e = is_blocked(gx - 1, gy - 1, gz + 1) * aoIntensity;
                        f = is_blocked(gx + 1, gy - 1, gz + 1) * aoIntensity;
                        g = is_blocked(gx + 1, gy + 1, gz + 1) * aoIntensity;
                        h = is_blocked(gx - 1, gy + 1, gz + 1) * aoIntensity;
                    }
                    VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u1, v1, l * (1.0f - c - d - e));
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u2, v2, l * (1.0f - a - b - g));
                    VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u1, v2, l * (1.0f - a - d - h));

                    VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u1, v1, l * (1.0f - c - d - e));
                    VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u2, v1, l * (1.0f - b - c - f));
                    VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u2, v2, l * (1.0f - a - b - g));
                }

                // Back face (z-1)
                if (!is_blocked(gx, gy, gz - 1)) {
                    l = 0.8f;
                    if (ambientOcclusion) {
                        a = is_blocked(gx, gy + 1, gz - 1) * aoIntensity;
                        b = is_blocked(gx + 1, gy, gz - 1) * aoIntensity;
                        c = is_blocked(gx, gy - 1, gz - 1) * aoIntensity;
                        d = is_blocked(gx - 1, gy, gz - 1) * aoIntensity;
                        e = is_blocked(gx - 1, gy - 1, gz - 1) * aoIntensity;
                        f = is_blocked(gx + 1, gy - 1, gz - 1) * aoIntensity;
                        g = is_blocked(gx + 1, gy + 1, gz - 1) * aoIntensity;
                        h = is_blocked(gx - 1, gy + 1, gz - 1) * aoIntensity;
                    }
                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u2, v1, l * (1.0f - c - d - e));
                    VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u2, v2, l * (1.0f - a - d - h));
                    VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u1, v2, l * (1.0f - a - b - g));

                    VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u2, v1, l * (1.0f - c - d - e));
                    VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u1, v2, l * (1.0f - a - b - g));
                    VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u1, v1, l * (1.0f - b - c - f));
                }
            }
        }
    }
    return new Mesh(buffer, index / VERTEX_SIZE, attributes);
}