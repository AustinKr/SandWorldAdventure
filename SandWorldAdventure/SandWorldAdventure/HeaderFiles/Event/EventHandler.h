#pragma once
#include <unordered_map>
#include "DelegateTypes/IEventDelegate.h"

namespace SandboxEngine::Event
{
	template<class DelegateType = IEventDelegate>
	struct EventHandler
	{
	public:
		typedef unsigned long int UID;

	private:
		UID m_NextID;
		std::unordered_map<UID, DelegateType*> m_Events;

	public:
		// pEvent should be created by  new MyEventDelegate()
		inline UID SubscribeEvent(DelegateType* pEvent)
		{
			m_Events.insert(std::make_pair(m_NextID, pEvent));
			return m_NextID++;
		}
		inline void TryUnsubscribeEvent(UID id)
		{
			if (!m_Events.contains(id))
				return;
			delete(m_Events[id]);
			m_Events.erase(id);
		}
		inline void InvokeEvents(void* pArguments)
		{
			for (auto& rIter : m_Events)
			{
				rIter.second->Invoke(pArguments);
			}
		}
		inline void Release()
		{
			for (auto& rIter : m_Events)
			{
				delete(rIter.second);
			}
			m_Events.clear();
		}

		inline std::unordered_map<UID, DelegateType*>::iterator GetBegin()
		{
			return m_Events.begin();
		}
		inline std::unordered_map<UID, DelegateType*>::iterator GetEnd()
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