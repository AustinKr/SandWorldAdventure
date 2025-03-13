#include "HeaderFiles/Render/Camera.h"
#include "HeaderFiles/Render/Renderer.h"

namespace SandboxEngine::Render
{
	void Camera::DrawRect(ScreenState* pScreen, double x, double y, double width, double height, UINT color)
	{
		Vector2 pos = FromViewportToScreen(FromWorldToViewport(Vector2(x, y)));
		Vector2 size = FromViewportToScreen(FromWorldToViewport(Vector2(width, height), false), false);

		Renderer::DrawRectPixels(pScreen, floor(pos.X), floor(pos.Y), floor(size.X + pos.X - floor(pos.X)), floor(size.Y + pos.Y - floor(pos.Y)), color);
	}

	Vector2 Camera::FromWorldToViewport(Vector2 world, bool center)
	{
		if(center)
			// 0,0 is bottom left of camera, 1,1 is top right, this->Position is center
			world -= this->Position - this->Scale / 2.0;
		// get coordinate on a scale of 0 to 1
		return world / this->Scale; 
	}
	Vector2 Camera::FromViewportToWorld(Vector2 viewport, bool center)
	{
		viewport *= this->Scale;
		if (center)
			viewport += this->Position - this->Scale / 2.0;
		return viewport;
	}

	Vector2 Camera::FromViewportToScreen(Vector2 viewport, bool center)
	{
		viewport.X *= this->MainScreen.BitmapInfo.bmiHeader.biHeight;
		viewport.Y *= this->MainScreen.BitmapInfo.bmiHeader.biHeight;
		if (center)
		{
			viewport.X += (this->MainScreen.BitmapInfo.bmiHeader.biWidth - this->MainScreen.BitmapInfo.bmiHeader.biHeight) / 2.0;
		}

		return viewport;
	}
	Vector2 Camera::FromScreenToViewport(Vector2 screen, bool center)
	{
		if (center)
		{
			screen.X -= (this->MainScreen.BitmapInfo.bmiHeader.biWidth - this->MainScreen.BitmapInfo.bmiHeader.biHeight) / 2.0;
		}
		screen.X /= this->MainScreen.BitmapInfo.bmiHeader.biHeight;
		screen.Y /= this->MainScreen.BitmapInfo.bmiHeader.biHeight;
		return screen;
	}
}