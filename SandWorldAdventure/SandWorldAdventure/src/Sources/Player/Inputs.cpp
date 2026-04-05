#include "SWA/Player/Inputs.h"
#include "SWAEngine/Math/vector2.h"
#include "GP2D/Pipeline/Window/Window.h"

using namespace GP2D::Pipeline;
using namespace GP2D::Math;
using namespace SWAEngine::Math;

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace SWA::Player
{
	void Inputs::SetInputs()
	{
		Movement = {};
		TryJump = false;

		// Keyboard
		if (Window::Window::GetKeyState(GLFW_KEY_SPACE) || Window::Window::GetKeyState(GLFW_KEY_UP) || Window::Window::GetKeyState(GLFW_KEY_W))
		{
			// Try jump
			TryJump = true;
		}
		else if (Window::Window::GetKeyState(GLFW_KEY_DOWN) || Window::Window::GetKeyState(GLFW_KEY_S))
			Movement.Y = -1;

		if (Window::Window::GetKeyState(GLFW_KEY_RIGHT) || Window::Window::GetKeyState(GLFW_KEY_D))
			Movement.X = 1;
		else if (Window::Window::GetKeyState(GLFW_KEY_LEFT) || Window::Window::GetKeyState(GLFW_KEY_A))
			Movement.X = -1;

		// Tiles
		BreakTile = Window::Window::GetKeyState(GLFW_MOUSE_BUTTON_1);
		AddTile = Window::Window::GetKeyState(GLFW_MOUSE_BUTTON_2);
	}
}