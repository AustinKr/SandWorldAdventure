#include "HeaderFiles/Game/GameObjects/DebugObject.h"
#include "HeaderFiles/Render/Renderer.h"

namespace SandboxEngine::Game::GameObject
{
	const UINT DebugObject::NULL_DEBUG_COLOR = 0xff00ff;

	DebugObject::DebugObject()
	{
		DebugScreen = { 1,1 };
		_i = 0;
	}

	void DebugObject::Update(Time time)
	{
		if (_i > 0)
		{
			_i -= time.FrameDeltaTime;
			return;
		}
		_i = 10; // TODO: Probably an issue with delta time

		Render::Renderer::Background(&DebugScreen, GameObject::DebugObject::NULL_DEBUG_COLOR);
	}

	void DebugObject::CopyScreen(Render::ScreenState* pMainScreen)
	{
		Render::Renderer::CopyState(&this->DebugScreen, 0, 0, pMainScreen, 0, 0, NULL_DEBUG_COLOR);
	}

	void DebugObject::Release()
	{
		DebugScreen.Release();
	}
}