#ifndef	DIRTBLOCK_H
#define DIRTBLOCK_H

#include "IBlock.h"

class DirtBlock : public IBlock
{
public:
	DirtBlock()
	{
		baseTextureId = 3; 
		dayTextureId = 4; 
		nightTextureId = 3;
	}
	int getBlockId() const { return 3; }
	int getTextureId(bool isDay) const override
	{
		return isDay ? dayTextureId : nightTextureId;
	}
	bool isDayNightSensitive() const override { return true; }
	bool isBlockCarvable() const override { return true; }
private:
	int dayTextureId;
	int nightTextureId;
};
#endif