#ifndef AIRBLOCK_H
#define AIRBLOCK_H

#include "IBlock.h"

class AirBlock : public IBlock
{
public:
	int getBlockId() const override;
	virtual bool isBlockCarvable() const override;
};
#endif
