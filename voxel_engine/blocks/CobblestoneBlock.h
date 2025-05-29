#ifndef	COBBLESTONEBLOCK_H
#define COBBLESTONEBLOCK_H

#include "IBlock.h"

class CobblestoneBlock : public IBlock
{
public:
    CobblestoneBlock() { baseTextureId = 2; }
    int getBlockId() const { return 2; }
    int getTextureId(bool isDay) const { return baseTextureId; }
    bool isDayNightSensitive() const override { return false; }
    bool isBlockCarvable() const override { return true; }
};

#endif