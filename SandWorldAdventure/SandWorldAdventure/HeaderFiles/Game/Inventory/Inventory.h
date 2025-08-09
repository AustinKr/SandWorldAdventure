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
		Vector2Int SelectedItemID;
		
		// Has no arguments. Invoked whenever the size of the collection changes
		Event::EventHandler<> AssignmentEventHandler;
		// Arguments: pointer to struct{Vector2Int position; Item itemCopy}.  Invoked for every SetItemAt()
		Event::EventHandler<> ItemAssignmentEventHandler;

		Inventory() : AssignmentEventHandler{}, ItemAssignmentEventHandler{}, m_Items {}, m_Size{} {/*nothing*/ }
		Inventory(Vector2Int size) : AssignmentEventHandler{}, ItemAssignmentEventHandler{}, m_Items{}, m_Size(size)
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
			for (auto iter : m_Items)
			{
				iter.Release();
			}
			m_Items.clear();
			m_Size = { 0,0 };

			AssignmentEventHandler.InvokeEvents(nullptr);
		}

		// Sets the item
		void SetItemAt(Vector2Int position, ItemType&& rNewItem)
		{
			// Get reference
			ItemType* pItem = &m_Items.at(position.X + position.Y * m_Size.X);
			// Update
			pItem->Release();
			*pItem = rNewItem;

			// Create arguments
			void* pArguments = malloc(sizeof(Vector2Int) + sizeof(ItemType));
			*reinterpret_cast<Vector2Int*>(pArguments) = position;
			*reinterpret_cast<ItemType*>((char*)pArguments + sizeof(Vector2Int)) = *pItem;
			// Invoke
			ItemAssignmentEventHandler.InvokeEvents(pArguments);
			// Free memory
			free(pArguments);
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
			ItemAssignmentEventHandler.Release();
			AssignmentEventHandler.Release();
		}
	};
}