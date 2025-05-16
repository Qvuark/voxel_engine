#ifndef	WOODBLOCK_H
#define WOODBLOCK_H

#include "IBlock.h"

class WoodBlock : public IBlock
{
public:
	int getBlockId() const override;
	virtual bool isBlockCarvable() const override;
};
#endif