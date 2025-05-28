#ifndef BEDROCKBLOCK_H
#define BEDROCKBLOCK_H

#include "IBlock.h"

class BedrockBlock : public IBlock
{
public:
    BedrockBlock() { baseTextureId = 8; }
    int getBlockId() const { return 6; }
    virtual int getTextureId(bool isDay) const { return baseTextureId; }
    bool isDayNightSensitive() const override { return false; }
    virtual bool isBlockCarvable() const override { return false; }
};

#endif
