#ifndef	PLANKSBLOCK_H
#define PLANKSBLOCK_H

#include "IBlock.h"

class PlayersBlock : public IBlock
{
public:
    PlayersBlock() { baseTextureId = 10; }

    virtual int getTextureId(bool isDay) const { return baseTextureId; }
    glm::vec3 getColorMultiplier(bool isDay) const override
    {
        return isDay ? glm::vec3(1.0f) : glm::vec3(1.2f, 1.2f, 0.8f);
    }
    bool isDayNightSensitive() const override { return true; }
    virtual bool isBlockCarvable() const override { return false; }
};
#endif