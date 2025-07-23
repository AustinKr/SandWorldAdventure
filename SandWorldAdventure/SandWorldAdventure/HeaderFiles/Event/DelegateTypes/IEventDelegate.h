#pragma once
namespace SandboxEngine::Event
{
	struct IEventDelegate
	{
	public:
		virtual void Invoke(void*) = 0;
	};
}