#ifndef	PLANKSBLOCK_H
#define PLANKSBLOCK_H

#include "IBlock.h"

class PlanksBlock : public IBlock
{
public:
	int getBlockId() const override;
	virtual bool isBlockCarvable() const override;
};
#endif