#include "HeaderFiles/Game/GameObjects/DebugObject.h"

namespace SandboxEngine::Game::GameObject
{
	void DebugObject::Update(Time time)
	{
		if (_i > 0)
		{
			_i -= time.FrameDeltaTime;
			return;
		}
		_i = 10;
	}

	void DebugObject::Release()
	{
	}
}