#include "HeaderFiles/Render/Renderer.h"

namespace SandboxEngine::Render
{
	const double Renderer::GAME_UNIT_SIZE = 100;

	Vector2 Renderer::FromWorldToScreen(ScreenState* pScreen, Vector2 world, bool center)
	{
		world.X /= GAME_UNIT_SIZE;
		world.Y /= GAME_UNIT_SIZE;

		world.X *= (double)pScreen->BitmapInfo.bmiHeader.biHeight;
		world.Y *= (double)pScreen->BitmapInfo.bmiHeader.biHeight;
		if (center)
		{
			world.X += (double)pScreen->BitmapInfo.bmiHeader.biWidth / 2.0;
			world.Y += (double)pScreen->BitmapInfo.bmiHeader.biHeight / 2.0;
		}
		return world;
	}
	Vector2 Renderer::FromScreenToWorld(ScreenState* pScreen, Vector2 screen, bool center)
	{
		if (center)
		{
			screen.X -= (double)pScreen->BitmapInfo.bmiHeader.biWidth / 2.0;
			screen.Y -= (double)pScreen->BitmapInfo.bmiHeader.biHeight / 2.0;
		}
		screen.X /= (double)pScreen->BitmapInfo.bmiHeader.biHeight;
		screen.Y /= (double)pScreen->BitmapInfo.bmiHeader.biHeight;
		screen.X *= GAME_UNIT_SIZE;
		screen.Y *= GAME_UNIT_SIZE;

		return screen;
	}

	UINT* Renderer::GetPixel(UINT** ppData, int i)
	{
		return (*ppData + i);
	}
	UINT* Renderer::GetPixel(ScreenState* pScreen, int i)
	{
		return GetPixel((UINT**)&pScreen->Data, i);
	}
	void Renderer::Background(ScreenState* pScreen, UINT color)
	{
		for (int i = 0; i < pScreen->BitmapInfo.bmiHeader.biWidth * pScreen->BitmapInfo.bmiHeader.biHeight; i++)
		{
			*GetPixel(pScreen, i) = color;
		}
	}

	void Renderer::DrawRectPixels(ScreenState* pScreen, int x, int y, int width, int height, UINT color)
	{
		for (int i = std::clamp(x, 0, (int)pScreen->BitmapInfo.bmiHeader.biWidth); i < std::clamp(x + width, 0, (int)pScreen->BitmapInfo.bmiHeader.biWidth); i++)
		{
			for (int j = std::clamp(y, 0, (int)pScreen->BitmapInfo.bmiHeader.biHeight); j < std::clamp(y + height, 0, (int)pScreen->BitmapInfo.bmiHeader.biHeight); j++)
			{
				*GetPixel(pScreen, i + j * pScreen->BitmapInfo.bmiHeader.biWidth) = color;
			}
		}
	}
	void Renderer::DrawRect(ScreenState* pScreen, double x, double y, double width, double height, UINT color)
	{
		Vector2 screenPos = FromWorldToScreen(pScreen, Vector2(x, y));
		Vector2 screenSize = FromWorldToScreen(pScreen, Vector2(width, height), false);

		DrawRectPixels(pScreen, floor(screenPos.X), floor(screenPos.Y), floor(screenSize.X + screenPos.X - floor(screenPos.X)), floor(screenSize.Y + screenPos.Y - floor(screenPos.Y)), color);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="pScreenSource"></param>
	/// <param name="sourceX"></param>
	/// <param name="sourceY"></param>
	/// <param name="pScreenDestination"></param>
	/// <param name="destinationX"></param>
	/// <param name="destinationY"></param>
	/// <param name="nullColor">set to -1 for no null color( should be more efficient)</param>
	void Renderer::CopyState(ScreenState* pScreenSource, int sourceX, int sourceY, ScreenState* pScreenDestination, int destinationX, int destinationY, int nullColor)
	{
		//TODO: Maybe try to simplify this function to improve performance

		int sourceWidth = pScreenSource->BitmapInfo.bmiHeader.biWidth;
		int sourceHeight = pScreenSource->BitmapInfo.bmiHeader.biHeight;
		int destWidth = pScreenDestination->BitmapInfo.bmiHeader.biWidth;
		int destHeight = pScreenDestination->BitmapInfo.bmiHeader.biHeight;
		if (sourceWidth < 1 || sourceHeight < 1 || destWidth < 1 || destHeight < 1)
			return;

		Vector2 dstOrigin = Vector2(destinationX, destinationY);
		Vector2 dstEnd = dstOrigin + Vector2(destWidth, destHeight);
		// Source area to copy
		Vector2 srcOrigin = Vector2(std::clamp(sourceX, (int)dstOrigin.X, (int)dstEnd.X), std::clamp(sourceY, (int)dstOrigin.Y, (int)dstEnd.Y));
		Vector2 srcEnd = Vector2(std::clamp((int)sourceX + sourceWidth, (int)dstOrigin.X, (int)dstEnd.X), std::clamp((int)sourceY + sourceHeight, (int)dstOrigin.Y, (int)dstEnd.Y));

		int size = srcEnd.X - srcOrigin.X; // Columns to copy
		int sourceOffset = srcOrigin.X - sourceX; // change in src origin
		int sourceRow = 0, destinationRow = 0;
		unsigned int* pSource, * pDestination;
		for (int y = srcOrigin.Y; y < srcEnd.Y; y++)
		{
			sourceRow = y - srcOrigin.Y;
			destinationRow = y - dstOrigin.Y;
			pSource = Renderer::GetPixel(pScreenSource, sourceRow * pScreenSource->BitmapInfo.bmiHeader.biWidth + sourceOffset);
			pDestination = Renderer::GetPixel(pScreenDestination, destinationRow * pScreenDestination->BitmapInfo.bmiHeader.biWidth + std::clamp(srcOrigin.X - dstOrigin.X, 0.0, dstEnd.X));
			if (nullColor == -1)
			{
				// Copy whole row at a time
				memcpy_s(pDestination, sizeof(unsigned int) * size, pSource, sizeof(unsigned int) * size);
				continue;
			}
			// Go through each pixel one by one
			for (int i = 0; i < size; i++)
			{
				if (*(pSource + i) != nullColor)
					*(pDestination + i) = *(pSource + i);
			}
		}


		/*RECT srcBounds = RECT(sourceX, sourceY + sourceHeight, sourceX + sourceWidth, sourceY);
		RECT dstBounds = RECT(destinationX, destinationY + destHeight, destinationX + destWidth, destinationY);*/

		//// crop source to fit destination
		//if (srcBounds.right > dstBounds.right)
		//	srcBounds.right -= srcBounds.right - dstBounds.right;
		//if (srcBounds.top > dstBounds.top)
		//	srcBounds.top -= srcBounds.top - dstBounds.top;

		//int offset = std::clamp((int)dstBounds.left - (int)srcBounds.left, 0, (int)dstBounds.right);
		//int posX = srcBounds.left + offset; // screen position of the beginning of the columns to copy
		//int size = std::clamp((int)dstBounds.right - posX, 0, (int)srcBounds.right - (int)srcBounds.left);
		//// go through each row and copy the amount of columns
		//for (int y = std::clamp((int)srcBounds.bottom, (int)dstBounds.bottom, (int)dstBounds.top); y < srcBounds.top-1; y++)
		//{
		//	int destRow = y - dstBounds.bottom;// So its not the screen position
		//	int sourceRow = y - srcBounds.bottom; // So its not the screen position
		//	unsigned int* pDestination = Renderer::GetPixel(pScreenDestination, destWidth * destRow + offset);
		//	unsigned int* pSource = Renderer::GetPixel(pScreenSource, (srcBounds.right-srcBounds.left)*sourceRow + offset);

		//	if (nullColor == -1)
		//	{
		//		// Copy whole row at a time
		//		memcpy_s(pDestination, size, pSource, size);
		//		continue;
		//	}
		//	// Go through each pixel one by one
		//	for (int i = 0; i < size; i++)
		//	{
		//		if (*(pSource + i) != nullColor)
		//			*(pDestination + i) = *(pSource + i);
		//	}
		//}
	}
}