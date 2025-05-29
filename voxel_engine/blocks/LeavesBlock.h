#ifndef	LEAVESBLOCK_H
#define LEAVESBLOCK_H

#include "IBlock.h"

class LeavesBlock : public IBlock
{
public:

    LeavesBlock() { baseTextureId = 12; }
    int getBlockId() const { return 10; }
    int getTextureId(bool isDay) const { return baseTextureId; }
    glm::vec3 getColorMultiplier(bool isDay) const override
    {
        return isDay ? glm::vec3(1.0f) : glm::vec3(0.6f, 0.6f, 0.6f);
    }
    bool isDayNightSensitive() const override { return true; }
    bool isBlockCarvable() const override { return false; }
};
#endif