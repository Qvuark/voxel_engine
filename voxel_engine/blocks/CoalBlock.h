#ifndef	COALBLOCK_H
#define COALBLOCK_H

#include "IBlock.h"

class CoalBlock : public IBlock
{
public:
	int getBlockId() const override;
	virtual bool isBlockCarvable() const override;
};
#endif