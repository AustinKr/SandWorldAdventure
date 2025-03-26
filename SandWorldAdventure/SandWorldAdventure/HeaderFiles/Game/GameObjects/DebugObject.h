#pragma once
#include "IGameObject.h"
#include "HeaderFiles/GraphicsPipeline/Meshes/Mesh.h"

namespace SandboxEngine::Game::GameObject
{
	class DebugObject : public IGameObject
	{
	private:
		int _i = 0;
		GraphicsPipeline::Mesh* mp_Mesh;
	public:

		DebugObject();

		void Update(Time time) override;
		void Release() override;
	};
}