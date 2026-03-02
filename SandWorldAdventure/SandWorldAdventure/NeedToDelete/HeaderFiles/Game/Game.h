#pragma once

//TODO: Go through all files and clean up includes

// Forward declarations
namespace SandboxEngine::Game::GameObject::Tilemap
{
	class TileBehavior;

	class Tilemap;
	class TilemapContainer;
	struct Chunk;

}

// Headers
#include "Time.h"

#include <vector>
#include <Windows.h>
#include "HeaderFiles/Math.h"

#include "HeaderFiles/Game/GameObjects/IGameObject.h"
#include "HeaderFiles/Game/GameInstance.h"

#include "HeaderFiles/Game/GameObjects/Tilemap/Tile.h"

#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehavior.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/TileBehaviorNames.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/SolidTileBehavior.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TileBehaviors/SandTileBehavior.h"

#include "HeaderFiles/Game/GameObjects/Tilemap/Chunk.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/TilemapContainer.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"

#include "HeaderFiles/Game/GameObjects/Player.h"
#include "HeaderFiles/Game/GameObjects/DebugObject.h"