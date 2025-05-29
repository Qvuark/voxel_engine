#ifndef ANDESITEBLOCK_H
#define ANDESITECLOCK_H

#include "IBlock.h"

class AndesiteBlock : public IBlock
{
public:
    AndesiteBlock() { baseTextureId = 5; }
    int getBlockId() const { return 4; }
    int getTextureId(bool isDay) const { return baseTextureId; }
    bool isDayNightSensitive() const override { return false; }
    bool isBlockCarvable() const override { return true; }
};
#endif