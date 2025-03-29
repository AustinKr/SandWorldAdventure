#pragma once
#include "Tile.h"
#include <vector>
#include <optional>

namespace SandboxEngine::Game::GameObject::Tilemap
{
	struct Chunk
	{
	public:
		// Column in the tilemap
		int X;
		// Row in the tilemap
		int Y;
		// How many tiles arn't empty
		int NonEmptyTilesCount;
		std::optional<std::vector<Tile>> Tiles;

		// This is for rendering I think
		bool IsActive;

		inline Chunk()
		{
			X = 0;
			Y = 0;
			NonEmptyTilesCount = 0;
			IsActive = false;
			Tiles = {};
		}
	};
}