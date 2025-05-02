#pragma once
// * This file contains structures that are to be sent to the GPU to render a tilemap *

#include "Quadtree.h"
#include "HeaderFiles/Math.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	// TODO: LeafData will be replaced by Tile in the future
	class LeafData : public Quadtree::IQuadtreeData
	{
	public:
		unsigned int Color;
		LeafData(unsigned int col) : Color(col)
		{

		};
		virtual bool IntersectsNodeBounds(Quadtree::Vector2 dataOrigin, Quadtree::Vector2 nodeBottomLeft, Quadtree::Vector2 nodeSize) override
		{
			return dataOrigin.X >= nodeBottomLeft.X && dataOrigin.Y >= nodeBottomLeft.Y && 
				dataOrigin.X < nodeBottomLeft.X + nodeSize.X && dataOrigin.Y < nodeBottomLeft.Y + nodeSize.Y;
		}
		// Set the bottom left corner and top right corner to the bounding box around the geometry of this element
		virtual void GetBoundingBox(Quadtree::Vector2 dataOrigin, Quadtree::Vector2* pBottomLeft, Quadtree::Vector2* pTopRight) override
		{
			// TODO: Fill this out
			*pBottomLeft = dataOrigin;
			*pTopRight = dataOrigin;
		}
		virtual void Release() override
		{
			delete(this);
		}
	};

	struct StaticQuadtreeNode
	{
		// Index relative to this node's parents
		int Index;
		bool IsLeaf;
		//TODO: I dont think pointers may be sent to the gpu- use separate fields instead
		unsigned int p_Data; // Could be 0( no child or leaf data), index array of 4 in buffer to child node, or leaf data( a tile color to be interpreted as an unsigned integer)
	};
	struct StaticQuadtree
	{
		float2 RootOrigin;
		float RootSize;
		int LeafDepth;

		StaticQuadtreeNode *p_NodesBegin;

		int NodeCount; // Additional(not included in shader)
	};
}