#pragma once
#include <SWAEngine/GameObject.h>
#include <SWAEngine/Component/IComponent.h>
#include <SWAEngine/Component/Physics/Rigidbody.h>
#include <SWAEngine/Component/Physics/Collider.h>
#include "SWA/Player/Inputs.h"
#include "SWA/Player/Inventory/Manager.h"

#include "GP2D/Pipeline/Mesh/Mesh.h"

#include <string>

namespace SWA::Player
{
	// TODO: Checks collision against the tilemap, bounds of tilemap, and other entities
	// Retrieves the scale of the player object. Note: The player shares its scale with its mesh origin
	// A player object that is associated with its mesh
	struct Player : SWAEngine::Component::IComponent
	{
	private:
		GP2D::Pipeline::Mesh::Mesh *mp_Mesh;

		SWAEngine::Math::Time m_Time; // Not sure this is needed

		Inputs m_Inputs;

	public:
		SWAEngine::Component::Physics::Rigidbody* p_LinkedRigidbody;
		SWAEngine::Component::Physics::Collider* p_LinkedCollider;
		SWAEngine::Component::Transform* p_LinkedTransform;

		// This should only be called once when the application begins and is the only instance used
		Player();

		Inventory::Manager Inventory;

		double Gravity;
		double Speed;
		double CameraFollowSpeed;
		double JumpHeight;

		void Move();

		static std::string const GetName();
		// Inherited via IComponent
		virtual void Initialize(std::string objName) override; // Creates the mesh with default size, links components, and creates the inventory and its gui
		virtual void Release() override; 
		virtual void Update(std::string objectName, SWAEngine::Math::Time time) override;
		virtual void SetActive(bool state) override;
		virtual bool GetActive() override;
	};
}