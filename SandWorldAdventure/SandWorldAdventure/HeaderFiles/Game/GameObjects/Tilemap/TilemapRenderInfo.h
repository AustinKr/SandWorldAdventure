#pragma once
#include "HeaderFiles/MasterWindow.h"
#include "HeaderFiles/Vector2.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	struct TilemapRenderInfo
	{
	public:
		Vector2 ExactTilePixelSize;
		Vector2 ExactChunkPixelSize;

		std::vector<Vector2> ActiveChunks;
		bool ShouldResize; // Has the window main screen been resized? If so, resize active chunks

		inline TilemapRenderInfo()
		{
			ExactTilePixelSize = Vector2(0, 0);
			ExactChunkPixelSize = Vector2(0, 0);
			ActiveChunks = {};
			ShouldResize = true;
		}
	};
}