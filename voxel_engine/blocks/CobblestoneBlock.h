#ifndef	COBBLESTONEBLOCK_H
#define COBBLESTONEBLOCK_H

#include "IBlock.h"

class CobblestoneBlock : public IBlock
{
public:
    CobblestoneBlock() { baseTextureId = 2; }
    virtual int getTextureId(bool isDay) const { return baseTextureId; }
    glm::vec3 getColorMultiplier(bool isDay) const override;
    bool isDayNightSensitive() const override { return false; }
    virtual bool isBlockCarvable() const override { return true; }
};

#endif