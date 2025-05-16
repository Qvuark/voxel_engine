#ifndef	WATERBLOCK_H
#define WATERBLOCK_H

#include "IBlock.h"

class WaterBlock : public IBlock
{
public:
	int getBlockId() const override;
	virtual bool isBlockCarvable() const override;
};
#endif