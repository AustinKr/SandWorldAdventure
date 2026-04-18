#pragma once
#include <SWAEngine/IPhysicsObject.h>
#include "SWAEngine/BaseGameObject.h"
#include "SWAEngine/Math/vector2.h"
#include "SWA/Player/Inputs.h"
#include "SWA/Player/Inventory/Manager.h"

#include "GP2D/Pipeline/Mesh/Mesh.h"

namespace SWA::Player
{
	// A player object that is associated with its mesh
	struct Player : SWAEngine::BaseGameObject, SWAEngine::IPhysicsObject
	{
	private:
		GP2D::Pipeline::Mesh::Mesh *mp_Mesh;

		SWAEngine::Time m_Time; // Not sure this is needed

		Inputs m_Inputs;

		static GP2D::GP2D_HEX_COLOR MixColor(GP2D::GP2D_HEX_COLOR colA, GP2D::GP2D_HEX_COLOR colB);
	public:
		Inventory::Manager Inventory;

		double Gravity;
		double Speed;
		double CameraFollowSpeed;
		double JumpHeight;

		// This should only be called once when the application begins and is the only instance used
		// Creates the mesh with default size
		// Creates the inventory and gui
		Player();
		void Move();

		// Inherited via BaseGameObject
		virtual void Update(SWAEngine::Time time) override;
		virtual void Release() override;

		// Retrieves the position of the player object. Note: The player shares its position with its mesh origin
		SWAEngine::Math::Vector2 GetPosition() override;
		void SetPosition(SWAEngine::Math::Vector2 newPosition) override;
		// Retrieves the scale of the player object. Note: The player shares its scale with its mesh origin
		SWAEngine::Math::Vector2 GetScale() override;
		void SetScale(SWAEngine::Math::Vector2 newScale) override;

		// Checks collision against the tilemap, bounds of tilemap, and other entities
		virtual bool IsColliding() override;
	};
}