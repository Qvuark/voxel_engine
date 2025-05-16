#ifndef	DIRTBLOCK_H
#define DIRTBLOCK_H

#include "IBlock.h"

class DirtBlock : public IBlock
{
public:
	int getBlockId() const override;
	virtual bool isBlockCarvable() const override;
};
#endif