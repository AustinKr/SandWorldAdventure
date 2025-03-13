#pragma once
#include "HeaderFiles/Time.h"
#include "HeaderFiles/Render/ScreenState.h"

namespace SandboxEngine::Game::GameObject
{
	class IGameObject
	{
	public:
		virtual void Update(Time time) = 0;
		virtual void CopyScreen(Render::ScreenState* pMainScreen) = 0;
		virtual void Release() = 0;
	};
}
