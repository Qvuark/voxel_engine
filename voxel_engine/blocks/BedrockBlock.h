#ifndef BEDROCKBLOCK_H
#define BEDROCKBLOCK_H

#include "IBlock.h"

class BedrockBlock : public IBlock
{
public:
    BedrockBlock() { baseTextureId = 8; }
    virtual int getTextureId(bool isDay) const { return baseTextureId; }
    glm::vec3 getColorMultiplier(bool isDay) const override;
    bool isDayNightSensitive() const override { return false; }
    virtual bool isBlockCarvable() const override { return false; }
};

#endif
