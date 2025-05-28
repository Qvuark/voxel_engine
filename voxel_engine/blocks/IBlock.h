#ifndef IBLOCK_H
#define IBLOCK_H
#include <glm/ext/vector_float3.hpp>

class IBlock
{
public:
	virtual ~IBlock() = default;
	virtual int getTextureId(bool isDay) const = 0;
	virtual bool isBlockCarvable() const { return true; };
	virtual glm::vec3 getColorMultiplier(bool isDay) const { return glm::vec3(1.0f); }
	virtual bool isDayNightSensitive() const { return false; }
protected:
	int baseTextureId;
};

#endif