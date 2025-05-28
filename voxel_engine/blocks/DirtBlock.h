#ifndef	DIRTBLOCK_H
#define DIRTBLOCK_H

#include "IBlock.h"

class DirtBlock : public IBlock
{
public:
	DirtBlock()
	{
		baseTextureId = 3; 
		dayTextureId = 3; 
		nightTextureId = 4;
	}
	int getTextureId(bool isDay) const override
	{
		return isDay ? dayTextureId : nightTextureId;
	}
	glm::vec3 getColorMultiplier(bool isDay) const override;
	bool isDayNightSensitive() const override { return true; }
	virtual bool isBlockCarvable() const override { return true; }
private:
	int dayTextureId;
	int nightTextureId;
};
#endif