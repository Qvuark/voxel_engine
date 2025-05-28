#ifndef	COALBLOCK_H
#define COALBLOCK_H

#include "IBlock.h"

class CoalBlock : public IBlock
{
public:
    CoalBlock() { baseTextureId = 9; }
    virtual int getTextureId(bool isDay) const { return baseTextureId; }
    glm::vec3 getColorMultiplier(bool isDay) const override;
    bool isDayNightSensitive() const override { return false; }
    virtual bool isBlockCarvable() const override { return true; }
};

#endif