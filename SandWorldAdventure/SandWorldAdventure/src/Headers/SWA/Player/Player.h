#pragma once
#include "SWAEngine/BaseGameObject.h"
#include "SWAEngine/Math/vector2.h"
#include "SWAEngine/Inventory/Inventory.h"
#include "SWA/Player/Item.h"

#include "GP2D/Pipeline/Mesh/Mesh.h"

namespace SWA::Player
{
	// A player object that is associated with its mesh
	struct Player : SWAEngine::BaseGameObject
	{
	private:
		GP2D::Pipeline::Mesh::Mesh *mp_Mesh;

		SWAEngine::Math::Vector2 m_Vel;
		double m_Dampening;
	
		bool m_ShouldBreakTile;
		bool m_ShouldAddTile;

		void SetInputs();

		void TryUseItem(Item item);
		void UseCurrentTileItem(Item item, SWAEngine::Math::Vector2Int mouseTilePosition);
	public:
		/*
		The ExtraFlags field stores the base rgba color used for that tile.
		*/
		SWAEngine::Inventory::Inventory<Item> CurrentInventory;

		double AccX;
		double AccY;
		double Speed;
		double CameraFollowSpeed;

		// This should only be called once when the application begins and is the only instance used
		// Creates the mesh
		// Creates the inventory and gui
		Player();
		
		// Inherited via BaseGameObject
		virtual void Update(SWAEngine::Time time) override;
		virtual void Release() override;

		// Retrieves the position of the player object. Note: The player shares its position with its mesh origin
		SWAEngine::Math::Vector2 GetPosition();
		void SetPosition(SWAEngine::Math::Vector2 newPosition);
	};
}