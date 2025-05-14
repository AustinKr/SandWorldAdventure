#include "HeaderFiles/Game/GameObjects/Player.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"
#include "HeaderFiles/Game/GameInstance.h"
#include "HeaderFiles/RenderLayerNames.h"

namespace SandboxEngine::Game::GameObject
{
	Player::Player() : AccX(0), AccY(0), m_Vel({}), m_Dampening(.98)
	{
		// GraphicsPipeline2D::Release() releases registered mesh data
		mp_Mesh = new GraphicsPipeline::Mesh(); // Create its mesh
		GameInstance::Pipeline.GetLayer(RENDERLAYERS_Characters).RegisterMesh(mp_Mesh); // Register the mesh

		mp_Mesh->Scale = float2(1.0f, 1.0f);
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
			0, 2, GraphicsPipeline::GraphicsPipeline2D::GP2D_BASE_SHADER
		};
	}

	Vector2 Player::GetPosition()
	{
		return Vector2(mp_Mesh->Origin.X, mp_Mesh->Origin.Y);
	}
	void Player::SetPosition(Vector2 newPosition)
	{
		mp_Mesh->Origin = float2(newPosition.X, newPosition.Y);
	}

	void Player::Update(Time time)
	{
		m_Vel += Vector2::Normalize(Vector2(AccX, AccY) * 2.0) * time.FrameDeltaTime * 3.0;

		SetPosition(GetPosition() + m_Vel * time.FrameDeltaTime);

		m_Vel.X *= m_Dampening;
		m_Vel.Y *= m_Dampening;
	}
	void Player::Release()
	{
		// Not realy necessary unless player is for some reason deleted prior to the application termination

		GameInstance::Pipeline.GetLayer(RENDERLAYERS_Characters).UnregisterMesh(mp_Mesh); // Unregister the mesh
	}
}