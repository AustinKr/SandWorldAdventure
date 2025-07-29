#pragma once
#include <vector>
#include "HeaderFiles/Math.h"
#include "HeaderFiles/Event/EventHandler.h"

namespace SandboxEngine::Game::Inventory
{
	template<typename ItemType>
	struct Inventory
	{
		typedef std::vector<ItemType> COLLECTION;

	private:
		COLLECTION m_Items;
		Vector2Int m_Size;

	public:
		// Has no arguments. Invoked whenever the size of the collection changes
		Event::EventHandler<> AssignmentEventHandler;

		Inventory() : m_Items{}, m_Size{} {/*nothing*/ }
		Inventory(Vector2Int size) : m_Items{}, m_Size(size) 
		{
			Assign(size);
		}

		void Assign(Vector2Int newSize)
		{
			m_Items.assign(newSize.X * newSize.Y, {});
			m_Size = newSize;

			AssignmentEventHandler.InvokeEvents(nullptr);
		}
		void Append(Vector2Int addedSize)
		{
			Vector2Int newSize = addedSize + m_Size;

			COLLECTION itemsCopy = std::move(m_Items);
			m_Items = { newSize.X* newSize.Y};

			for (int w = 0; w < m_Size.X; w++)
			{
				for (int h = 0; h < m_Size.Y; h++)
				{
					m_Items[h * newSize.X + w] = itemsCopy[h * m_Size.X + w];
				}
			}
			m_Size = newSize;

			AssignmentEventHandler.InvokeEvents(nullptr);
		}
		void Clear()
		{
			m_Items.clear();
			AssignmentEventHandler.InvokeEvents(nullptr);
		}
		ItemType& GetItemAt(Vector2Int position)
		{
			return m_Items.at(position.X + position.Y * m_Size.X);
		}
		COLLECTION::iterator GetBegin()
		{
			return m_Items.begin();
		}
		COLLECTION::iterator GetEnd()
		{
			return m_Items.end();
		}

		Vector2Int GetSize()
		{
			return m_Size;
		}

		void Release()
		{
			AssignmentEventHandler.Release();
		}
	};
}