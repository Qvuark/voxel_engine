#ifndef	COBBLESTONEBLOCK_H
#define COBBLESTONEBLOCK_H

#include "IBlock.h"

class CobblestoneBlock : public IBlock
{
public:
	int getBlockId() const override;
	virtual bool isBlockCarvable() const override;
};
#endif