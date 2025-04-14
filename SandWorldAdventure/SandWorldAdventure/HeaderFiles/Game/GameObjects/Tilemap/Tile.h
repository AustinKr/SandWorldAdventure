#pragma once
#include "HeaderFiles/Math.h"
#include <Windows.h>

//#include "Quadtree.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	struct Tile// : public Quadtree::IQuadtreeData
	{
	public:
		UINT Color;
		int BehaviorIndex;

		bool HasValue;

		Vector2 Motion; // The velocity

		double LastMoveTime; // The time since the tile physically got replaced

		inline Tile(UINT color, int behavior)
		{
			Color = color;
			BehaviorIndex = behavior;
			HasValue = true;
			Motion = Vector2(0, 0);
			LastMoveTime = 0;
		}
		inline Tile()
		{
			Color = 0x0;
			BehaviorIndex = 0;
			HasValue = false;
			Motion = Vector2(0, 0);
			LastMoveTime = 0;
		}

		//// Do a check that returns whether the geometry of this element intersects with the node bounds
		//inline virtual bool IntersectsNodeBounds(Quadtree::Vector2 dataOrigin, Quadtree::Vector2 nodeBottomLeft, Quadtree::Vector2 nodeSize) override
		//{
		//	return false;
		//}
		//// Set the bottom left corner and top right corner to the bounding box around the geometry of this element
		//inline virtual void GetBoundingBox(Quadtree::Vector2 dataOrigin, Quadtree::Vector2* pBottomLeft, Quadtree::Vector2* pTopRight) override
		//{
		//	*pBottomLeft = Quadtree::Vector2(0, 0);
		//	*pTopRight = Quadtree::Vector2(0, 0);
		//}
		//inline virtual void Release() override
		//{
		//	// Nothing
		//}
	};
}