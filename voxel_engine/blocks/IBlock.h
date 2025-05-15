#ifndef IBLOCK
#define IBLOCK

class IBlock
{
	virtual ~IBlock() = default;
	virtual int getBlockId() = 0;
	virtual void render(int x, int y, int z) = 0;
	virtual void interactable() = 0;
	virtual bool isBlockCarvable() { return true };
};

#endif