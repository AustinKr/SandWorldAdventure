#pragma once
#include "SWAEngine/BaseGameObject.h"
#include "SWAEngine/Math/vector2.h"
#include "SWAEngine/Inventory/Inventory.h"
#include "SWA/Player/Item.h"

#include "GP2D/Pipeline/Mesh/Mesh.h"

namespace SWA::Player
{
	// TODO: Could create IPhysicsObject
	// A player object that is associated with its mesh
	struct Player : SWAEngine::BaseGameObject
	{
	private:
		static const int MAX_COLLISION_STEPS;

		GP2D::Pipeline::Mesh::Mesh *mp_Mesh;

		SWAEngine::Math::Vector2 m_LastVelocity;
		SWAEngine::Math::Vector2 m_Velocity;
		SWAEngine::Math::Vector2 m_Acceleration;

		SWAEngine::Time m_Time;

		double m_Dampening;
	
		bool m_ShouldBreakTile;
		bool m_ShouldAddTile;

		bool m_IsTouchingGround;

		void SetInputs();

		void TryUseItem(Item item);
		void UseCurrentTileItem(Item item, SWAEngine::Math::Vector2Int mouseTilePosition);

		// Step move algorithm (only accurate for small movements)
		bool StepMove(SWAEngine::Math::Vector2 movement);
		void TryApplyVelocity();

		static GP2D::GP2D_HEX_COLOR MixColor(GP2D::GP2D_HEX_COLOR colA, GP2D::GP2D_HEX_COLOR colB);
	public:
		/*
		The ExtraFlags field stores the base rgba color used for that tile.
		*/
		SWAEngine::Inventory::Inventory<Item> CurrentInventory;

		double Gravity;
		double Speed;
		double CameraFollowSpeed;
		double JumpHeight;

		// This should only be called once when the application begins and is the only instance used
		// Creates the mesh with default size
		// Creates the inventory and gui
		Player();
		
		// Inherited via BaseGameObject
		virtual void Update(SWAEngine::Time time) override;
		virtual void Release() override;

		// Retrieves the position of the player object. Note: The player shares its position with its mesh origin
		SWAEngine::Math::Vector2 GetPosition();
		void SetPosition(SWAEngine::Math::Vector2 newPosition);
		// Retrieves the scale of the player object. Note: The player shares its scale with its mesh origin
		SWAEngine::Math::Vector2 GetScale();
		void SetScale(SWAEngine::Math::Vector2 newScale);

		// Returns the currently applied velocity
		SWAEngine::Math::Vector2 GetVelocity();
		// Moves by an impluse
		// Note that this is only applied at the end of every Player::Update()
		void AddVelocity(SWAEngine::Math::Vector2 vel);
		// Continually accelerates by the given amount
		void Accelerate(SWAEngine::Math::Vector2 acc);
		// Returns the actual change in velocity over change in time
		SWAEngine::Math::Vector2 GetAcceleration();

		void Jump(double height);
		bool IsTouchingGround();

		// Checks collision against the tilemap, bounds of tilemap, and other entities
		bool IsColliding();
	};
}