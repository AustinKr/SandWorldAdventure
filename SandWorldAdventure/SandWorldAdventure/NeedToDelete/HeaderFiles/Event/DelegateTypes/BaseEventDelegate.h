#pragma once

namespace SandboxEngine::Event
{
	template<typename EventArguments, typename EventData>
	struct BaseEventDelegate
	{
	public:
		// The function pointer
		void(*p_Function)(EventArguments, EventData);

		// (optional) data that may be stored with this event instance.
		EventData ExtraData;

		inline BaseEventDelegate(void (*pFunc)(EventArguments, EventData), EventData data)
		{
			p_Function = pFunc;
			ExtraData = data;
		}
	};
}

//TODO: Design how Event system will work
/*
Inventory.h -> separate from GUI and player entirely

Player ->   Player game object. Stores Inventory and PlayerInventoryGUI.
			Since there is only ever one player for each device, make static player pointer
PLayerInventoryGUI -> Accesess inventory through player pointer


EventHandler ->
BaseEventDelegate -> (fuction<args + void* pextraData> pFunction, void* pExtraData?)
	Derived Delegates?
	...


*/