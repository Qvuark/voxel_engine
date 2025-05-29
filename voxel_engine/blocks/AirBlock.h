#ifndef AIRBLOCK_H
#define AIRBLOCK_H

#include "IBlock.h"

class AirBlock : public IBlock
{
public:
    AirBlock() { baseTextureId = 0; }
    int getBlockId() const { return 0; }
    int getTextureId(bool isDay) const { return baseTextureId; }
    bool isDayNightSensitive() const override { return false; }
    bool isBlockCarvable() const override { return true; }
};
#endif
