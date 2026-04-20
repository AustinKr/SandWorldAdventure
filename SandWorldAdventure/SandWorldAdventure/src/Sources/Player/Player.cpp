#include "SWA/Player/Player.h"
#include "SWA/Player/Inventory/GUILayout.h"
#include "SWA/SpriteShaderProperties.h"

#include "SWA/Game.h"

#include "SWA/RenderLayerNames.h"
#include "GP2D/Pipeline/GenericPipeline.h"

#include <SWAEngine/GameObject/Component/Physics/BoxCollider.h>

using namespace GP2D::Pipeline;
using namespace GP2D::Math;
using namespace SWAEngine::Math;
using namespace SWAEngine::Tilemap;
using namespace SWA::Player::Inventory;

using namespace SWAEngine::GameObject;
using namespace Component;
using namespace Physics;

namespace SWA::Player
{
	Player::Player(
		SWAEngine::GameObject::Component::Physics::Rigidbody* const pRigidbody,
		SWAEngine::GameObject::Component::Physics::Collider* const pCollider) :
		p_LinkedCollider(pCollider), p_LinkedRigidbody(pRigidbody),
		m_Inputs{}, Gravity(0.9), m_Time{}, CameraFollowSpeed{5}, Speed(1), JumpHeight(1)
	{
		// Create the mesh
		mp_Mesh = new Mesh::Mesh(true, SpriteShaderProperties::CreateProperties("player"), true);
		GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Characters).RegisterMesh(mp_Mesh);
		mp_Mesh->Vertices =
		{
			{{ 0, 0 }, {0, 0, 0}},
			{{ 1, 0 }, {1, 0, 0}},
			{{ 1, 1 }, {1, 1, 0}},
			{{ 0, 1 }, {0, 1, 0}},
		};
		mp_Mesh->Triangles =
		{
			0, 1, 2,
			0, 2, 3
		};
		mp_Mesh->Shaders =
		{
			{0, 6, "DefaultSpriteShader"}
		};

		// Subscribe to update mesh
		pCollider->p_LinkedTransform->OnSetPosition += [&](SWAEngine::Math::Vector2 newPosition) { mp_Mesh->Origin = { (float)newPosition.X, (float)newPosition.Y }; };
		pCollider->p_LinkedTransform->OnSetScale += [&](SWAEngine::Math::Vector2 newScale) { mp_Mesh->Scale = { (float)newScale.X, (float)newScale.Y }; };

		// Set default coordinates
		pCollider->p_LinkedTransform->SetPosition({ 0.0, 0.0});
		pCollider->p_LinkedTransform->SetScale({ 0.1, 0.2 });

		// Create player inventory and gui
		Inventory = {};
		GUILayout::Initialize(Inventory);
	}
	Player* const Player::CreateInstance(GameObject& linkedObject)
	{
		// TODO: COuld make a function that creates a component if it doesnt exist

		// Get collider
		Collider* pCollider = linkedObject.TryGetComponent<BoxCollider>("box_collider"); // TODO: use any collider
		if (pCollider == nullptr)
			pCollider = linkedObject.RegisterComponent(BoxCollider::CreateCollider(linkedObject));
		
		// Get rigidboy
		Rigidbody* pRigidbody = linkedObject.TryGetComponent<Rigidbody>("rigidbody");
		if (pRigidbody == nullptr)
			pRigidbody = linkedObject.RegisterComponent(Rigidbody::CreateRigidbody(linkedObject));

		// Create and return
		return new Player(pRigidbody, pCollider);
	}

	void Player::Move()
	{
		m_Inputs.SetInputs();
		// Try jump
		if (m_Inputs.TryJump && p_LinkedRigidbody->IsTouchingGround())
			p_LinkedRigidbody->Jump(JumpHeight, Gravity);
		// Move
		p_LinkedRigidbody->AddVelocity(Vector2::Normalize(m_Inputs.Movement) * Speed * m_Time.RealDeltaTime); // Accerate but as an impulse
		// Add gravity
		p_LinkedRigidbody->AddVelocity({ 0, -Gravity * m_Time.RealDeltaTime }); // Accerate but as an impulse
	}

	std::string const Player::GetName()
	{
		return "player";
	}
	void Player::SetActive(bool state) {}
	bool Player::GetActive() { return true; }
	void Player::Update(std::string objectName, SWAEngine::Math::Time time)
	{
		m_Time = time;

		// Use selected item
		Inventory.TryUseSelectedItem(m_Inputs, time);
		
		Move();

		// Move camera
		GenericPipeline::s_ActiveCamera.Origin += (mp_Mesh->Origin - GenericPipeline::s_ActiveCamera.Origin) * float(m_Time.RealDeltaTime * CameraFollowSpeed);
	}
	void Player::Release()
	{
		Inventory.Release(); // Clears/releases everything associated with the player inventory
		GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Characters).UnregisterMesh(mp_Mesh); // Unregister the mesh
	}

	// TODO: Create tilemap collider in SWAEngine, possibly create PlayerCollider in SWA for world origin
	/*bool Player::IsColliding()
	{
		return
			Coordinates.GetPosition().X < Game::p_Tilemap->Origin.X || Coordinates.GetPosition().Y < Game::p_Tilemap->Origin.Y ||
			Game::p_Tilemap->DetectCollisionRect(Game::p_Tilemap->WorldToTile(Coordinates.GetPosition()), Game::p_Tilemap->WorldToTile(Coordinates.GetPosition() + Coordinates.GetScale()));
	}*/
}