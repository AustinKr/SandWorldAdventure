#pragma once
#include "SWAEngine/Math/vector2.h"
#include "SWAEngine/Inventory/BaseItem.h"
#include "SWAEngine/EventHandler.h"
#include <vector>

namespace SWAEngine::Inventory
{
	class SWA_ENGINE_API Inventory
	{
		typedef BaseItem ITEM;
		typedef std::vector<ITEM*> COLLECTION;

	private:
		COLLECTION m_Items;
		SWAEngine::Math::Vector2Int m_Size;

	public:
		SWAEngine::Math::Vector2Int SelectedItemPosition;

		typedef SWAEngine::Math::Vector2Int RESIZE_EVENT_ARGS;
		typedef std::pair<SWAEngine::Math::Vector2Int, ITEM*> ASSIGNMENT_EVENT_ARGS;

		// Argument is the old size. Invoked whenever the size of the collection changes
		EventHandler<RESIZE_EVENT_ARGS> ResizeEventHandler;
		// Arguments: Vector2Int location, ITEM* pNewItem.  Invoked for every SetItemAt()
		EventHandler<ASSIGNMENT_EVENT_ARGS> AssignmentEventHandler;

		Inventory(SWAEngine::Math::Vector2Int size = { 0,0 });

		void Assign(SWAEngine::Math::Vector2Int newSize);
		// Adds to the size. Assumes the given value is non-negative
		void Append(SWAEngine::Math::Vector2Int addedSize);
		void Clear();

		// Sets the item and releases the old one(if any)
		void SetItemAt(SWAEngine::Math::Vector2Int position, ITEM* pNewItem);
		// Gets the item
		ITEM* GetItemAt(SWAEngine::Math::Vector2Int position);

		COLLECTION::iterator GetBegin();
		COLLECTION::iterator GetEnd();

		SWAEngine::Math::Vector2Int GetSize();

		void Release();
	};
}