#ifndef AIRBLOCK_H
#define AIRBLOCK_H

#include "IBlock.h"

class AirBlock : public IBlock
{
public:
    AirBlock() { baseTextureId = 0; }
    virtual int getTextureId(bool isDay) const { return baseTextureId; }
    glm::vec3 getColorMultiplier(bool isDay) const override;
    bool isDayNightSensitive() const override { return false; }
    virtual bool isBlockCarvable() const override { return true; }
};
#endif
