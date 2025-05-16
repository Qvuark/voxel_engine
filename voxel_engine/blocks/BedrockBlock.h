#ifndef BEDROCKBLOCK_H
#define BEDROCKBLOCK_H

#include "IBlock.h"

class BedrockBlock : public IBlock
{
public:
	int getBlockId() const override;
	virtual bool isBlockCarvable() const override;
};
#endif
