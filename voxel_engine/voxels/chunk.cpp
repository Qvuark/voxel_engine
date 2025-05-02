#include "chunk.h"
#include "voxels.h"
#include <math.h>

Chunk::Chunk()
{
    for (int y = 0; y < CHUNK_HEIGHT; y++) 
    {
        for (int z = 0; z < CHUNK_DEPTH; z++) 
        {
            for (int x = 0; x < CHUNK_WIDTH; x++) 
            {
                float noise = sin(x * NOISE_FREQUENCY) * NOISE_AMPLITUDE + NOISE_AMPLITUDE;
                int surfaceHeight = static_cast<int>(noise * SURFACE_SCALE);

                uint8_t id = 0; 
                if (y <= surfaceHeight) {
                    id = (y <= STONE_LAYER_HEIGHT) ? 2 : 1;
                }

                voxels[getVoxelIndex(x, y, z)].id = id;
            }
        }
    }
}
Chunk::~Chunk()
{
	delete[] voxels;
}