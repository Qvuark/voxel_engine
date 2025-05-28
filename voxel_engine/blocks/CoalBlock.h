#ifndef	COALBLOCK_H
#define COALBLOCK_H

#include "IBlock.h"

class CoalBlock : public IBlock
{
public:
    CoalBlock() { baseTextureId = 9; }
    int getBlockId() const { return 7; }
    virtual int getTextureId(bool isDay) const { return baseTextureId; }
    bool isDayNightSensitive() const override { return false; }
    virtual bool isBlockCarvable() const override { return true; }
};

#endif