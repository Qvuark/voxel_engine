#ifndef	SLANEBLOCK_H
#define SLANEBLOCK_H

#include "IBlock.h"

class SlaneBlock : public IBlock
{
public:
	int getBlockId() const override;
	virtual bool isBlockCarvable() const override;
};
#endif