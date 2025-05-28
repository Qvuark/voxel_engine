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
#include "PlayersBlock.h"

inline std::unique_ptr<IBlock> createBlockById(int id)
{
	switch (id)
	{
	case 0: return std::make_unique<AirBlock>();
	case 1: return std::make_unique<SlaneBlock>();
	case 2: return std::make_unique<CobblestoneBlock>();
	case 3: return std::make_unique<DirtBlock>();
	case 4: return std::make_unique<AndesiteBlock>();
	case 5: return std::make_unique<WaterBlock>();
	case 6: return std::make_unique<BedrockBlock>();
	case 7: return std::make_unique<CoalBlock>();
	case 8: return std::make_unique<PlayersBlock>();
	case 9: return std::make_unique<WoodBlock>();
	case 10: return std::make_unique<LeavesBlock>();
	default: return std::make_unique<AirBlock>();
	}
}
#endif 
