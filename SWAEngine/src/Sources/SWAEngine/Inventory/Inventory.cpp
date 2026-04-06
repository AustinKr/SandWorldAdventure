#include "SWAEngine/Inventory/Inventory.h"

namespace SWAEngine::Inventory
{
	Inventory::Inventory(SWAEngine::Math::Vector2Int size) : ResizeEventHandler{}, AssignmentEventHandler{}, m_Items{}, m_Size(size)
	{
		Assign(size);
	}

	void Inventory::Assign(SWAEngine::Math::Vector2Int newSize)
	{
		if (newSize.X < 0 || newSize.Y < 0)
			return;

		SWAEngine::Math::Vector2Int oldSize = m_Size; // Copy for InvokeEvents()

		m_Items.assign(newSize.X * newSize.Y, nullptr);
		m_Size = newSize;

		ResizeEventHandler.InvokeEvents(oldSize);
	}
	void Inventory::Append(SWAEngine::Math::Vector2Int addedSize)
	{
		SWAEngine::Math::Vector2Int oldSize = m_Size; // Copy for InvokeEvents()
		SWAEngine::Math::Vector2Int newSize = addedSize + m_Size;

		COLLECTION oldItems = std::move(m_Items);
		m_Items = {};
		m_Items.assign(newSize.X * newSize.Y, nullptr);

		for (int w = 0; w < m_Size.X; w++)
		{
			for (int h = 0; h < m_Size.Y; h++)
			{
				m_Items[h * newSize.X + w] = oldItems[h * m_Size.X + w];
			}
		}
		m_Size = newSize;

		ResizeEventHandler.InvokeEvents(oldSize);
	}
	void Inventory::Clear()
	{
		SWAEngine::Math::Vector2Int oldSize = m_Size; // Copy for InvokeEvents()

		for (auto iter : m_Items)
		{
			if (iter != nullptr)
				iter->Release();
		}
		m_Items.clear();
		m_Size = { 0,0 };

		ResizeEventHandler.InvokeEvents(oldSize);
	}

	void Inventory::SetItemAt(SWAEngine::Math::Vector2Int position, ITEM* pNewItem)
	{
		ITEM* oldItem = m_Items[position.X + position.Y * m_Size.X];
		if (oldItem != nullptr)
			oldItem->Release();

		m_Items[position.X + position.Y * m_Size.X] = pNewItem;

		// Invoke
		AssignmentEventHandler.InvokeEvents(std::make_pair(position, pNewItem));
	}
	Inventory::ITEM* Inventory::GetItemAt(SWAEngine::Math::Vector2Int position)
	{
		return m_Items.at(position.X + position.Y * m_Size.X);
	}

	Inventory::COLLECTION::iterator Inventory::GetBegin()
	{
		return m_Items.begin();
	}
	Inventory::COLLECTION::iterator Inventory::GetEnd()
	{
		return m_Items.end();
	}

	SWAEngine::Math::Vector2Int Inventory::GetSize()
	{
		return m_Size;
	}

	void Inventory::Release()
	{
		Clear();
	}
}