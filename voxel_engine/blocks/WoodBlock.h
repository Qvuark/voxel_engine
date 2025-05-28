#ifndef	WOODBLOCK_H
#define WOODBLOCK_H

#include "IBlock.h"

class WoodBlock : public IBlock
{
public:
    WoodBlock() { baseTextureId = 11; }
    int getBlockId() const { return 9; }
    virtual int getTextureId(bool isDay) const { return baseTextureId; }
    glm::vec3 getColorMultiplier(bool isDay) const override 
    {
        return isDay ? glm::vec3(1.0f) : glm::vec3(0.6f, 0.6f, 0.6f);
    }
    bool isDayNightSensitive() const override { return true; }
    virtual bool isBlockCarvable() const override { return false; }
};
#endif