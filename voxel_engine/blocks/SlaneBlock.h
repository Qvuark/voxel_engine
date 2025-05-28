#ifndef	SLANEBLOCK_H
#define SLANEBLOCK_H

#include "IBlock.h"

class SlaneBlock : public IBlock
{
public:
    SlaneBlock() { baseTextureId = 1; }
    int getBlockId() const { return 1; }
    virtual int getTextureId(bool isDay) const { return baseTextureId; }
    bool isDayNightSensitive() const override { return false; }
    virtual bool isBlockCarvable() const override { return true; }
};
#endif