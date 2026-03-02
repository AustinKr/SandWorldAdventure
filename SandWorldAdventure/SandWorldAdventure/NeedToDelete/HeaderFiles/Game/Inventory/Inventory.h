#pragma once
#include <vector>
#include "HeaderFiles/Math.h"
#include "HeaderFiles/Event/EventHandler.h"
#include "ItemAssignmentEventArguments.h"

namespace SandboxEngine::Game::Inventory
{
	template<typename ItemType>
	class Inventory
	{
		typedef std::vector<ItemType> COLLECTION;

	private:
		COLLECTION m_Items;
		Vector2Int m_Size;

	public:
		Vector2Int SelectedItemID;

		// Argument is the old size. Invoked whenever the size of the collection changes
		Event::EventHandler<Vector2Int> AssignmentEventHandler;
		// Arguments: struct{Inventory*, Vector2Int location; Item itemCopy}.  Invoked for every SetItemAt()
		Event::EventHandler<ItemAssignmentEventArguments<ItemType>> ItemAssignmentEventHandler;

		Inventory() : AssignmentEventHandler{}, ItemAssignmentEventHandler{}, m_Items{}, m_Size{} {/*nothing*/ }
		Inventory(Vector2Int size) : AssignmentEventHandler{}, ItemAssignmentEventHandler{}, m_Items{}, m_Size(size)
		{
			Assign(size);
		}

		void Assign(Vector2Int newSize)
		{
			Vector2Int oldSize = m_Size; // Copy for InvokeEvents()

			m_Items.assign(newSize.X * newSize.Y, {});
			m_Size = newSize;

			AssignmentEventHandler.InvokeEvents(oldSize);
		}
		void Append(Vector2Int addedSize)
		{
			Vector2Int oldSize = m_Size; // Copy for InvokeEvents()
			Vector2Int newSize = addedSize + m_Size;

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

			AssignmentEventHandler.InvokeEvents(oldSize);
		}
		void Clear()
		{
			Vector2Int oldSize = m_Size; // Copy for InvokeEvents()

			for (auto iter : m_Items)
			{
				iter.Release();
			}
			m_Items.clear();
			m_Size = { 0,0 };

			AssignmentEventHandler.InvokeEvents(oldSize);
		}

		// Sets the item
		void SetItemAt(Vector2Int position, ItemType&& rNewItem)
		{
			// Get reference
			ItemType* pItem = &m_Items.at(position.X + position.Y * m_Size.X);
			// Update
			pItem->Release();
			*pItem = rNewItem;

			// Invoke
			ItemAssignmentEventHandler.InvokeEvents(ItemAssignmentEventArguments<ItemType>(position, *pItem));
		}
		// Returns a copy of the item (do not call .Release())
		ItemType GetItemAt(Vector2Int position)
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
			Clear();
		}
	};
}