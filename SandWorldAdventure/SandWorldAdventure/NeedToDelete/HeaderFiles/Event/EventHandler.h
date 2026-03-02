#pragma once
#include <unordered_map>
#include "DelegateTypes/BaseEventDelegate.h"

namespace SandboxEngine::Event
{
	template<typename EventArguments, typename EventData = void*, typename DelegateType = BaseEventDelegate<EventArguments, EventData>>
	struct EventHandler
	{
	public:
		typedef unsigned long int UID;

	private:
		UID m_NextID;
		std::unordered_map<UID, DelegateType> m_Events;

	public:
		// event should be created by EVENT(pFunc);
		inline UID SubscribeEvent(DelegateType event)
		{
			m_Events.insert(std::make_pair(m_NextID, event));
			return m_NextID++;
		}
		inline void TryUnsubscribeEvent(UID id)
		{
			if (!m_Events.contains(id))
				return;
			m_Events.erase(id);
		}
		inline void InvokeEvents(EventArguments arguments)
		{
			for (auto& rIter : m_Events)
			{
				// Invoke
				rIter.second.p_Function(arguments, rIter.second.ExtraData);
			}
		}

		inline std::unordered_map<UID, DelegateType>::iterator GetBegin()
		{
			return m_Events.begin();
		}
		inline std::unordered_map<UID, DelegateType>::iterator GetEnd()
		{
			return m_Events.end();
		}
		inline int GetCount()
		{
			return m_Events.size();
		}

		inline EventHandler() : m_NextID{}, m_Events{} {/*nothing*/ }
	};
}