#pragma once
#include "SWAEngine/Math/vector2.h"
#include "GP2D/Pipeline/GraphicsTypes.h"

namespace SWA::Player
{
	// A player object that is associated with its mesh
	struct Player
	{
	private:
		GP2D::Pipeline::GP2D_UID m_MeshUID;

		SWAEngine::Math::Vector2 m_Vel;
		double m_Dampening;
	
		bool m_ShouldBreakTile;
		bool m_ShouldAddTile;
	public:
		/*
		The ExtraFlags field stores the base rgba color used for that tile.
		The p_Data field stores the behavior type
		*/
		Inventory::Inventory<Inventory::BasicItem> CurrentInventory;
		Inventory::PlayerInventoryGUI InventoryGUI;

		double AccX;
		double AccY;
		double Speed;
		double CameraFollowSpeed;

		// Initializes the player. This should only be called once when the application begins and is the only instance used. Assigns GameInstance::p_Player automatically
		Player(IGameObject* pTilemap);
		
		// Retrieves the position of the player object. Note: The player shares its position with its mesh origin
		Vector2 GetPosition();
		void SetPosition(Vector2 newPosition);

		// Inherited via IGameObject
		void Update(Time time) override;
		void Release() override;
	};
}