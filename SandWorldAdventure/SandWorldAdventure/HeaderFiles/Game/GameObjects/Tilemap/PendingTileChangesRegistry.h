#pragma once
#include "HeaderFiles/Game/GameObjects/IGameObject.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/PendingTileChange.h"
#include "HeaderFiles/Time.h"
#include <unordered_map>

namespace SandboxEngine::Game::GameObject::Tilemap
{
	struct PendingTileChangesRegistry
	{
	public:
		typedef std::unordered_map<Vector2Int, PendingTileChange, Vector2Hasher> COLLECTION;

		PendingTileChangesRegistry(IGameObject* pTilemap);

		void TryRefreshTile(Vector2Int tilePosition);
		bool SetTile(Vector2Int tilePosition, Tile newTile);
		bool WillContainTile(Vector2Int tilePosition);

		void UpdateRegistry(Time time);
	private:
		IGameObject* mp_Tilemap;
		// The tiles that will override any of the existing tilemap in their places
		COLLECTION m_PendingTileChanges;

	};
}