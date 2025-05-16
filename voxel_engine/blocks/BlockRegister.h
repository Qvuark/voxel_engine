#ifndef BLOCKREGISTER_H 
#define BLOCKREGISTER_H

#include <memory>
#include "IBlock.h"
#include "AirBlock.h"
#include "AndesiteBlock.h"
#include "BedrockBlock.h"
#include "CoalBlock.h"
#include "CobblestoneBlock.h"
#include "DirtBlock.h"
#include "LeavesBlock.h"
#include "SlaneBlock.h"
#include "WaterBlock.h"
#include "WoodBlock.h"


inline std::make_unique<IBlock> createBlockById(int id)
{
	switch (id)
	{
	case 0: return std::make_unique<AirBlock>();
	case 1: return std::make_unique<>
	}
}
#endif 
