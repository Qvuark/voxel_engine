#ifndef	LEAVESBLOCK_H
#define LEAVESBLOCK_H

#include "IBlock.h"

class LeavesBlock : public IBlock
{
public:
	int getBlockId() const override;
	virtual bool isBlockCarvable() const override;
};
#endif