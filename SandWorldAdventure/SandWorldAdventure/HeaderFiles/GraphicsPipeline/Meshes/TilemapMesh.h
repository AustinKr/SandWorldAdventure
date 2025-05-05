#pragma once
#include "IMesh.h"
#include "HeaderFiles/Game/GameObjects/IGameObject.h"

#include "Quadtree.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/StaticQuadtreeTypes.h"

namespace SandboxEngine::GraphicsPipeline
{
	class TilemapMesh : public IMesh
	{
	private:
		typedef unsigned int TILE_DATA_TYPE; // A color (?first 2 bits: red; second 2: green; third 2: blue?)

		// Note: this object will always be a tilemap- IGameObject* is only used for simplicity
		Game::GameObject::IGameObject* mp_Tilemap;
		static const Vertex m_Verts[4];

		Game::GameObject::Tilemap::StaticQuadtree CreateOnScreenTilesBuffer();

		//TODO: Remove quadtree function because textures would work better
		template<typename Iter>
		inline Game::GameObject::Tilemap::StaticQuadtree&& CreateQuadtree(Vector2 bottomLeft, Vector2 topRight, Iter bufferBegin, Iter bufferEnd)
		{
			const int leafDepth = 5;
			// Create the quadtree
			Quadtree::Quadtree quadtree = {}; // TODO: fix the quadtree dll so that it is specific to images or bitmaps
			double width = topRight.X - bottomLeft.X, height = topRight.Y - bottomLeft.Y, rootSize = width > height ? width : height;
			quadtree.AssignRootNodeDimensions(bottomLeft.X, bottomLeft.Y, rootSize, rootSize);
			quadtree.CompileTree(bufferBegin, bufferEnd, leafDepth);

			// Convert the dynamic structure of the quadtree to a simplized structure to send to the GPU
			Game::GameObject::Tilemap::StaticQuadtree buffer = {};
			buffer.RootOrigin = float2(quadtree.GetNodesBegin()->BottomLeft.X, quadtree.GetNodesBegin()->BottomLeft.Y);
			buffer.RootSize = quadtree.GetRootNodeSize().X;
			buffer.LeafDepth = leafDepth;
			buffer.NodeCount = quadtree.GetNodesSize();
			buffer.p_NodesBegin = (Game::GameObject::Tilemap::StaticQuadtreeNode*)malloc(sizeof(Game::GameObject::Tilemap::StaticQuadtreeNode) * buffer.NodeCount);
			auto iter = quadtree.GetNodesBegin();
			for (int i = 0; i < buffer.NodeCount; i++)
			{
				// Default initialize
				*(buffer.p_NodesBegin + i) = {};

				// Set the index
				(buffer.p_NodesBegin + i)->Index = (iter + i)->Index;
				// Set the IsLeaf
				(buffer.p_NodesBegin + i)->IsLeaf = (iter + i)->IsLeaf;
				if ((iter + i)->Data == nullptr)
				{
					(buffer.p_NodesBegin + i)->IsNull = true;
					continue;
				}
				// Set the data (nothing or color)
				if ((iter + i)->IsLeaf) // Check: is this a leaf?
				{
					// Get quadtree leaf
					auto leafData = ((Quadtree::QuadtreeLeaf*)(iter + i)->Data)->Data;
					// Check for null vector
					if (leafData.empty())
					{
						(buffer.p_NodesBegin + i)->IsNull = true;
						continue;
					}
					// Put the color of the tile into the node of the buffer
					unsigned int colorHex = ((Game::GameObject::Tilemap::LeafData*)leafData[0].second)->Color;
					(buffer.p_NodesBegin + i)->TileColor = float3((colorHex >> 16) & 0xff, (colorHex >> 8) & 0xff, colorHex & 0xff);
					continue;
				}
				// Set the node of the buffer 's data to whatever value is in the existing node(integer array)
				std::copy((int*)(iter + i)->Data, (int*)(iter + i)->Data + 4, (int*)(buffer.p_NodesBegin + i)->ChildrenIndicies);
				//quadtree.TryGetNode(i)->Data = nullptr; // Set to nullptr so the index array wont be cleared
			}
			quadtree.TryRelease(); // TODO: In the future, this probably wont be here
			return std::move(buffer);
		}
	public:
		TilemapMesh(Game::GameObject::IGameObject* pTilemap);

		virtual void Render(GraphicsPipeline::GraphicsPipeline2D* pPipeline, GLuint vertexBufferName, GLuint pVertexArray) override;
		virtual void Release() override;
	};
}