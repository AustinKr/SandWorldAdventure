#pragma once

template<typename EventArguments>
struct GameDelegate
{
public:
	void(*Function)(EventArguments);

	inline GameDelegate(void (*func)(EventArguments))
	{
		Function = func;
	}
};