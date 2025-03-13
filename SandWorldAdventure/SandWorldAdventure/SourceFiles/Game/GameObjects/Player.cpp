#include "HeaderFiles/Game/GameObjects/Player.h"
#include "HeaderFiles/Game/GameInstance.h"

namespace SandboxEngine::Game::GameObject
{
	void Player::Update(Time time)
	{
		// Blank
	}
	void Player::CopyScreen(Render::ScreenState* pMainScreen)
	{
		Game::GameInstance::MainCamera.DrawRect(pMainScreen, 0, 0, 30, 30, 0xf0f0f0);
	}
	void Player::Release()
	{
		// Blank
	}
}