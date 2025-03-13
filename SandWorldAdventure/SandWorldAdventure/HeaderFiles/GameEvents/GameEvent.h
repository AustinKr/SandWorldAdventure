#include <vector>
#include "GameDelegate.h"

template<typename EventArguments>
class GameEvent
{
private:
	std::vector<GameDelegate<EventArguments>> m_Delegates;

public:
	inline std::vector<GameDelegate<EventArguments>>::const_iterator Subscribe(GameDelegate<EventArguments> function)
	{
		m_Delegates.push_back(function);
		return m_Delegates.cend(); 
	}
	inline void Unsubscribe(std::vector<GameDelegate<EventArguments>>::const_iterator iter)
	{
		m_Delegates.erase(iter);
	}
	inline void Fire(EventArguments args)
	{
		if (m_Delegates.empty())
			return;
		
		for (int iter = 0; iter < m_Delegates.size(); iter++)
		{
			m_Delegates[iter].Function(args);
		}
	}
};