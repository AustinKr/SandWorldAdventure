#pragma once
#include "IGameObject.h"

namespace SandboxEngine::Game::GameObject
{
	class Player : public IGameObject
	{
		// Inherited via IGameObject
		void Update(Time time) override;
		void Release() override;
	};
}