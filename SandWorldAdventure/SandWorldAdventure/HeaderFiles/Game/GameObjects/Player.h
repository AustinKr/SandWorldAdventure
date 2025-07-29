#pragma once
#include "IGameObject.h"
#include "HeaderFiles/GraphicsPipeline/Meshes/Mesh.h"
#include "HeaderFiles/Game/Inventory/Inventory.h"
#include "HeaderFiles/Game/Inventory/BasicItem.h"
#include "HeaderFiles/Game/Inventory/PlayerInventoryGUI.h"

#define PLAYER_H
namespace SandboxEngine::Game::GameObject
{
	class Player : public IGameObject
	{
	private:
		GraphicsPipeline::Mesh* mp_Mesh;
		Vector2 m_Vel;
		double m_Dampening;
		bool m_ShouldBreakTile;
		bool m_ShouldAddTile;
		IGameObject* const mp_Tilemap;
	public:
		Inventory::Inventory<Inventory::BasicItem> CurrentInventory;
		Inventory::PlayerInventoryGUI InventoryGUI;

		double AccX;
		double AccY;
		double Speed;
		double CameraFollowSpeed;

		Player(IGameObject* pTilemap);
		
		// Retrieves the position of the player object. Note: The player shares its position with its mesh origin
		Vector2 GetPosition();
		void SetPosition(Vector2 newPosition);

		// Inherited via IGameObject
		void Update(Time time) override;
		void Release() override;
	};
}