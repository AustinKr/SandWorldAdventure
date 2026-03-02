#pragma once
#include "HeaderFiles/Time.h"

namespace SandboxEngine::Game::GameObject
{
	class IGameObject
	{
	public:
		virtual void Update(Time time) = 0;
		virtual void Release() = 0;
	};
}
