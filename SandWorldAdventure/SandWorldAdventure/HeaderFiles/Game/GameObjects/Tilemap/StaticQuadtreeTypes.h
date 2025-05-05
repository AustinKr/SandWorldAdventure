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
		int Index; // Index relative to this node's parents
		bool IsNull; // If set to true, TileColor and ChildrenIndices will be not have values
		bool IsLeaf;

		float3 TileColor; // Only is initialized if IsLeaf
		int ChildrenIndicies[4]; // Null if IsLeaf

		inline StaticQuadtreeNode() :Index(0), IsNull(0), IsLeaf(0), TileColor({})
		{
			ChildrenIndicies[0] = 0;
			ChildrenIndicies[1] = 0;
			ChildrenIndicies[2] = 0;
			ChildrenIndicies[3] = 0;
		}
	};
	struct StaticQuadtree
	{
		StaticQuadtreeNode* p_NodesBegin; // This is the only item that will be in the buffer
		int NodeCount; // Additional(not included in shader)
		
		// These are sent as uniforms
		float2 RootOrigin;
		float RootSize;
		int LeafDepth;
	};
}