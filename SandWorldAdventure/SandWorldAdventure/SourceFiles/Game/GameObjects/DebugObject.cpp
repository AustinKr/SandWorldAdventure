#include "HeaderFiles/Game/GameObjects/DebugObject.h"
#include "HeaderFiles/Game/GameInstance.h"
#include "HeaderFiles/RenderLayerNames.h"

namespace SandboxEngine::Game::GameObject
{
	DebugObject::DebugObject()
	{
		mp_Mesh = new GraphicsPipeline::Mesh(); // Create its mesh
		GameInstance::Pipeline.GetLayer(RENDERLAYERS_Debug).RegisterMesh(mp_Mesh); // Register the mesh

		// TODO: Add verts
	}

	void DebugObject::Update(Time time)
	{
		// Timer
		if (_i > 0)
		{
			_i -= time.FrameDeltaTime;
			return;
		}
		_i = 10;
	}

	void DebugObject::Release()
	{
		// Not realy necessary unless object is for some reason deleted prior to the application termination

		GameInstance::Pipeline.GetLayer(RENDERLAYERS_Debug).UnregisterMesh(mp_Mesh); // Unregister the mesh
	}
}