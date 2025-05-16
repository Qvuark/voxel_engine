#ifndef IBLOCK_H
#define IBLOCK_H

class IBlock
{
public:
	virtual ~IBlock() = default;
	virtual int getBlockId() const = 0;
	virtual bool isBlockCarvable() const { return true; };
};

#endif