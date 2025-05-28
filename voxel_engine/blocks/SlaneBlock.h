#ifndef	SLANEBLOCK_H
#define SLANEBLOCK_H

#include "IBlock.h"

class SlaneBlock : public IBlock
{
public:
    SlaneBlock() { baseTextureId = 1; }
    virtual int getTextureId(bool isDay) const { return baseTextureId; }
    glm::vec3 getColorMultiplier(bool isDay) const override;
    bool isDayNightSensitive() const override { return false; }
    virtual bool isBlockCarvable() const override { return true; }
};
#endif