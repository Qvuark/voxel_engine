#include "chunk.h"
#include "voxels.h"
#include <math.h>
#include <glm/gtc/noise.hpp>

inline int getVoxelIndex(int lx, int ly, int lz)
{
    return (ly * CHUNK_DEPTH + lz) * CHUNK_WIDTH + lx;
}
Chunk::Chunk(int xpos, int ypos, int zpos) : x(xpos), y(ypos), z(zpos) {
    constexpr int WATER_LEVEL = 5;
    constexpr float NOISE_SCALE = 0.05f;
    constexpr int MAX_HEIGHT = 30;
    constexpr int BEDROCK_LAYER = 2;

    voxels = new Voxel[CHUNK_VOLUME];

    for (int ly = 0; ly < CHUNK_HEIGHT; ly++) {
        for (int lz = 0; lz < CHUNK_DEPTH; lz++) {
            for (int lx = 0; lx < CHUNK_WIDTH; lx++) {
                int realX = lx + x * CHUNK_WIDTH;
                int realY = ly + y * CHUNK_HEIGHT;
                int realZ = lz + z * CHUNK_DEPTH;

                int id = 0;
                float noise = glm::perlin(glm::vec3(realX * NOISE_SCALE, realZ * NOISE_SCALE, 0.0f));
                int terrainHeight = static_cast<int>((noise + 1.0f) * 0.5f * MAX_HEIGHT);

                if (realY <= BEDROCK_LAYER) 
                    id = 6; 
                else if (realY <= WATER_LEVEL && id == 0) 
                    id = 5;
                else if (realY <= terrainHeight && id == 0) 
                {
                 
                    if (realY > terrainHeight - 4) 
                    {
                        id = 2; 
                    }
                    else if (realY == terrainHeight) 
                    {
                        id = (realY > WATER_LEVEL) ? 3 : 4;
                    }
                    else
                    {
                        id = 1;
                    }
                }

                voxels[getVoxelIndex(lx, ly, lz)].id = id;
            }
        }
    }
}
int Chunk::noiseGenerator(int realX, int realY, int realZ)
{
	return -glm::perlin(glm::vec3(realX * 0.0125f, realY * 0.0125f, realZ * 0.0125f)) > 0.1f;
}
Chunk::~Chunk() 
{
	delete[] voxels;
}