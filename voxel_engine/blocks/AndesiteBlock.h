#ifndef ANDESITEBLOCK_H
#define ANDESITECLOCK_H

#include "IBlock.h"

class AndesiteBlock : public IBlock
{
public:
    AndesiteBlock() { baseTextureId = 5; }
    virtual int getTextureId(bool isDay) const { return baseTextureId; }
    glm::vec3 getColorMultiplier(bool isDay) const override;
    bool isDayNightSensitive() const override { return false; }
    virtual bool isBlockCarvable() const override { return true; }
};
#endif