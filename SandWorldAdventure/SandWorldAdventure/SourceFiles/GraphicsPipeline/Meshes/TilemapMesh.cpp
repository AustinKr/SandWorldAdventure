#include "HeaderFiles/GraphicsPipeline/Meshes/TilemapMesh.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h" // Needed for IMesh.h
#include "HeaderFiles/GraphicsPipeline/Shaders/TilemapShaderInformation.h"
#include "HeaderFiles/GraphicsPipeline/ShaderTypes/ShaderType.h"

#include "HeaderFiles/Game/GameInstance.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"
#include <vector>

namespace SandboxEngine::GraphicsPipeline
{
	using namespace Game::GameObject::Tilemap;

	const Vertex TilemapMesh::m_Verts[4] =
	{
		Vertex{{-1, -1}, {0, 0, 0}},
		Vertex{{ 1, -1}, {1, 0, 0}},
		Vertex{{ 1,  1}, {1, 1, 0}},
		Vertex{{-1,  1}, {0, 1, 0}},
	};

	StaticQuadtree&& TilemapMesh::CreateOnScreenTilesBuffer()
	{
		// Find the two tile positions that correspond to the bottom left and top right of the screen
		Vector2Int bottomLeft = ((Tilemap*)mp_Tilemap)->FromWorldToTile(
								Game::GameInstance::Pipeline.ActiveCamera.ViewportToWorld(
									Vector2(0, 0))),
				   bottomLeftChunk = ((Tilemap*)mp_Tilemap)->Container.TileToChunkCoordinates(bottomLeft);
		Vector2Int topRight =	((Tilemap*)mp_Tilemap)->FromWorldToTile(
								Game::GameInstance::Pipeline.ActiveCamera.ViewportToWorld(
									Vector2(1, 1))),
				   topRightChunk = ((Tilemap*)mp_Tilemap)->Container.TileToChunkCoordinates(topRight);

		// TODO: In the future, this would be replaced with a dynamic quadtree used to store the tiles.
		// TODO: Tile should inherit IQuadtreeData
		std::vector<std::pair<Quadtree::Vector2, LeafData*>> tileColors = {};
		for (int y = bottomLeftChunk.Y; y < topRightChunk.Y; y++)
		{
			for (int x = bottomLeftChunk.X; x < topRightChunk.X; x++)
			{
				Chunk* pChunk = ((Tilemap*)mp_Tilemap)->Container.GetChunk(x + y * ((Tilemap*)mp_Tilemap)->Container.GetChunkBounds().X);
				if (!pChunk->Tiles.has_value())
					continue;

				tileColors.assign(pChunk->Tiles.value().size(), {});
				for (int i = 0; i < tileColors.size(); i++)
				{
					Vector2Int pos = ((Tilemap*)mp_Tilemap)->Container.CHUNK_SIZE * Vector2(pChunk->X, pChunk->Y)
									+ Vector2Int(i / ((Tilemap*)mp_Tilemap)->Container.GetTileBounds().X,
											  i % ((Tilemap*)mp_Tilemap)->Container.GetTileBounds().X);
					tileColors[i] = std::make_pair(Quadtree::Vector2(pos.X, pos.Y), new LeafData(pChunk->Tiles.value()[i].Color));
				}
			}
		}

		const int leafDepth = 25;
		// Create the quadtree
		Quadtree::Quadtree quadtree = {}; // TODO: fix the quadtree dll so that it is specific to images or bitmaps
		quadtree.AssignRootNodeDimensions(bottomLeft.X, bottomLeft.Y, topRight.X - bottomLeft.X, topRight.Y - bottomLeft.Y);
		quadtree.CompileTree(tileColors.cbegin(), tileColors.cend(), leafDepth);
		
		// Convert the dynamic structure of the quadtree to a buffer to send to the GPU
		StaticQuadtree buffer = {};
		buffer.NodeCount = quadtree.GetNodesSize();
		buffer.p_NodesBegin = (StaticQuadtreeNode*)malloc(sizeof(StaticQuadtreeNode) * buffer.NodeCount);
		auto iter = quadtree.GetNodesBegin();
		for (int i = 0; i < buffer.NodeCount; i++)
		{
			// Set the index
			(buffer.p_NodesBegin + i)->Index = (iter + i)->Index;
			// Set the IsLeaf and Data
			if ((buffer.p_NodesBegin + i)->IsLeaf = (iter + i)->IsLeaf)
			{
				// Get quadtree leaf
				auto leafData = ((Quadtree::QuadtreeLeaf*)(iter + i)->Data)->Data;
				// Put either nullptr or the color of the tile into the node of the buffer
				(buffer.p_NodesBegin + i)->p_Data = leafData.empty() 
															? 0x0 
															: (unsigned long long)((LeafData*)leafData[0].second)->Color;
				continue;
			}
			if ((iter + i)->Data == nullptr);
				continue;
			// Set the node of the buffer 's data to whatever value is in the existing leaf
			(buffer.p_NodesBegin + i)->p_Data = (unsigned int)(iter + i)->Data; // this is an index array of 4
			quadtree.TryGetNode(i)->Data = nullptr; // Set to nullptr so the index array wont be cleared
		}
;
		quadtree.TryRelease(); // TODO: In the future, this probably wont be here
		return (StaticQuadtree&&)buffer;
	}

	TilemapMesh::TilemapMesh(Game::GameObject::IGameObject* pTilemap) : mp_Tilemap(pTilemap)
	{
		/*nothing*/
	}

	void TilemapMesh::Render(GraphicsPipeline::GraphicsPipeline2D* pPipeline, GLuint vertexBufferName, GLuint pVertexArray)
	{
		//StaticQuadtree quadtree = CreateOnScreenTilesBuffer();

		ShaderTypes::ShaderType* pShader = pPipeline->TryGetShader<ShaderTypes::ShaderType>(GraphicsPipeline::GraphicsPipeline2D::GP2D_TILEMAP_SHADER); // Get the shader
		Shaders::TilemapShaderInformation* pShaderInformation = (Shaders::TilemapShaderInformation*)pShader->p_ShaderInformation;
		pShaderInformation->UpdateVertexData(pPipeline, vertexBufferName, pVertexArray, m_Verts, nullptr/*&quadtree*/); // Pass in vertex buffer data and quadtree of tilemap

		Vector2Int bounds = ((Tilemap*)mp_Tilemap)->Container.GetTileBounds();
		Vector2 tilemapOriginCoord = pPipeline->ActiveCamera.WorldToViewport(((Tilemap*)mp_Tilemap)->Position); // Convert the origin to viewport
		Vector2 tilemapWorldSizeCoord = pPipeline->ActiveCamera.WorldToViewport(((Tilemap*)mp_Tilemap)->TileSize * (Vector2)bounds); // Convert the world size to viewport

		glUseProgram(pShader->GetProgram()); // Set the program
		// Set Uniform variables
		glUniform1f(pShaderInformation->p_UniformTime, glfwGetTime()); // Set the time
		glUniform2d(pShaderInformation->p_UniformTilemapOrigin, tilemapOriginCoord.X, tilemapOriginCoord.Y); // Set the tilemap origin
		glUniform2i(pShaderInformation->p_UniformTilemapBounds, bounds.X, bounds.Y); // Set the tilemap columns and rows
		glUniform2d(pShaderInformation->p_UniformTilemapWorldSize, tilemapWorldSizeCoord.X, tilemapWorldSizeCoord.Y); // Set the tilemap world size
		
		// Draw quads
		glDrawArrays(GL_QUADS, 0, 4);

		//// Free quadtree
		//for (int i = 0; i < quadtree.NodeCount; i++)
		//{
		//	if (!quadtree.p_NodesBegin[i].IsLeaf && quadtree.p_NodesBegin[i].p_Data != 0x0)
		//	{
		//		free((int*)quadtree.p_NodesBegin[i].p_Data);
		//		quadtree.p_NodesBegin[i].p_Data = 0x0;
		//	}
		//}
		//free(quadtree.p_NodesBegin);
	}

	void TilemapMesh::Release()
	{
		((Game::GameObject::Tilemap::Tilemap*)mp_Tilemap)->p_Mesh = nullptr;
		delete(this);
	}
}