#pragma once
#include "SWAEngine/Math/vector2.h"
#include "SWAEngine/EventHandler.h"
#include <vector>

namespace SWAEngine::Inventory
{
	template<typename ItemType>
	class Inventory
	{
		typedef std::vector<ItemType> COLLECTION;

	private:
		COLLECTION m_Items;
		SWAEngine::Math::Vector2Int m_Size;

	public:
		SWAEngine::Math::Vector2Int SelectedItemID;

		typedef SWAEngine::Math::Vector2Int RESIZE_EVENT_ARGS;
		typedef std::pair<SWAEngine::Math::Vector2Int, ItemType> ASSIGNMENT_EVENT_ARGS;

		// Argument is the old size. Invoked whenever the size of the collection changes
		EventHandler<RESIZE_EVENT_ARGS> ResizeEventHandler;
		// Arguments: struct{Inventory*, Vector2Int location; Item itemCopy}.  Invoked for every SetItemAt()
		EventHandler<ASSIGNMENT_EVENT_ARGS> AssignmentEventHandler;

		Inventory(SWAEngine::Math::Vector2Int size = {0,0}, ItemType&& value = {}) : ResizeEventHandler{}, AssignmentEventHandler{}, m_Items{}, m_Size(size)
		{
			if (size.X > 0 && size.Y > 0)
				Assign(size, std::move(value));
		}

		void Assign(SWAEngine::Math::Vector2Int newSize, ItemType&& value)
		{
			SWAEngine::Math::Vector2Int oldSize = m_Size; // Copy for InvokeEvents()

			m_Items.assign(newSize.X * newSize.Y, value);
			m_Size = newSize;

			ResizeEventHandler.InvokeEvents(oldSize);
		}
		void Append(SWAEngine::Math::Vector2Int addedSize)
		{
			SWAEngine::Math::Vector2Int oldSize = m_Size; // Copy for InvokeEvents()
			SWAEngine::Math::Vector2Int newSize = addedSize + m_Size;

			COLLECTION itemsCopy = std::move(m_Items);
			m_Items = { newSize.X * newSize.Y };

			for (int w = 0; w < m_Size.X; w++)
			{
				for (int h = 0; h < m_Size.Y; h++)
				{
					m_Items[h * newSize.X + w] = itemsCopy[h * m_Size.X + w];
				}
			}
			m_Size = newSize;

			ResizeEventHandler.InvokeEvents(oldSize);
		}
		void Clear()
		{
			SWAEngine::Math::Vector2Int oldSize = m_Size; // Copy for InvokeEvents()

			for (auto iter : m_Items)
			{
				iter.Release();
			}
			m_Items.clear();
			m_Size = { 0,0 };

			ResizeEventHandler.InvokeEvents(oldSize);
		}

		// Sets the item
		void SetItemAt(SWAEngine::Math::Vector2Int position, ItemType&& rNewItem)
		{
			// Get reference
			ItemType& rItem = m_Items[position.X + position.Y * m_Size.X];
			// Update
			rItem.Release();
			rItem = std::move(rNewItem);

			// Invoke
			AssignmentEventHandler.InvokeEvents(std::make_pair(position, rItem));
		}
		// Returns a copy of the item (do not call .Release())
		ItemType GetItemAt(SWAEngine::Math::Vector2Int position)
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

		SWAEngine::Math::Vector2Int GetSize()
		{
			return m_Size;
		}

		void Release()
		{
			Clear();
		}
	};
}