#ifndef	WATERBLOCK_H
#define WATERBLOCK_H

#include "IBlock.h"

class WaterBlock : public IBlock 
{
public:
    WaterBlock() 
    {
        baseTextureId = 6;
        dayTextureId = 6;
        nightTextureId = 7;
    }
    int getBlockId() const { return 5; }
    int getTextureId(bool isDay) const override 
    {
        return isDay ? dayTextureId : nightTextureId;
    }
    bool isDayNightSensitive() const override { return true; }
    virtual bool isBlockCarvable() const override { return false; }
private:
    int dayTextureId;
    int nightTextureId;
};
#endif