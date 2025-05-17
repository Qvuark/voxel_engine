#ifndef CHUNK_H
#define CHUNK_H

#include <cstdint>
#include <memory>
#include "../blocks/IBlock.h"


#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 128
#define CHUNK_DEPTH 16
#define CHUNK_VOLUME (CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH)



class Voxel;

class Chunk
{
private:
	int x, y, z;
	bool isModified = true;
	std::unique_ptr<IBlock>* voxels;
public:
	Chunk(int xpos, int ypos, int zpos);
	void generateTree(int x, int y, int z);
	IBlock* getBlock(int lx, int ly, int lz) const;
	int getX() const;
	int getY() const;
	int getZ() const;
	void modify(bool b);
	bool getModifiedState() const;
	IBlock* getBlock(int index);
	void setBlock(int index, int blockId);
	~Chunk();
};

#endif 