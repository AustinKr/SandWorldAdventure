#pragma once
#include "IGameObject.h"

namespace SandboxEngine::Game::GameObject
{
	class DebugObject : public IGameObject
	{
	private:
		int _i = 0;
	public:
		static const UINT NULL_DEBUG_COLOR;
		Render::ScreenState DebugScreen;

		DebugObject();

		void Update(Time time) override;
		void CopyScreen(Render::ScreenState* pMainScreen) override;
		void Release() override;
	};
}