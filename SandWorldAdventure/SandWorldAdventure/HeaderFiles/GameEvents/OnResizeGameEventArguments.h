#pragma once

struct OnResizeGameEventArguments
{
public:
	int Width;
	int Height;

	inline OnResizeGameEventArguments(int width, int height)
	{
		Width = width;
		Height = height;
	}
};