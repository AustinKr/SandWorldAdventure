#include "SWAEngine/Tilemap/TilemapMesh.h"
#include "GP2D/Pipeline/GenericPipeline.h"

#include <algorithm>

#define GLFW_INCLUDE_NONE
#include "GL/glew.h"
#include "GLFW/glfw3.h"

using namespace GP2D;
using namespace GP2D::Math;
using namespace GP2D::Pipeline;
using namespace GP2D::Pipeline::Shader;

namespace SWAEngine::Tilemap
{
	// TODO: Why are verts defined -1 to 1 instead of 0 to 1?
	const Vertex2D TilemapMesh::TILE_VERTICES[4] =
	{
		Vertex2D{{-1, -1}, {0, 0, 0}},
		Vertex2D{{ 1, -1}, {1, 0, 0}},
		Vertex2D{{ 1,  1}, {1, 1, 0}},
		Vertex2D{{-1,  1}, {0, 1, 0}},
	};

	const char* TilemapMesh::TextureName = "TilemapTex";
	const char* TilemapMesh::TextureOriginCoordName = "TexOriginCoord";
	const char* TilemapMesh::TextureSizeCoordName = "TexSizeCoord";
	const char* TilemapMesh::TextureSizeName = "TexSize";


	bool TilemapMesh::Render(IPipeline* pIPipeline, GP2D::Pipeline::UINT vertexBufferName, GP2D::Pipeline::UINT pVertexArray)
	{
		auto pPipeline = static_cast<GenericPipeline*>(pIPipeline);

		//StaticQuadtree quadtree = CreateOnScreenTilesBuffer();
		auto buffer = CreateOnScreenTilesBuffer();

		// Get the shader and its information which allows us to communicate with specific glsl code
		auto pShader = GenericPipeline::s_Shaders.TryGetShader<BaseShaderType>(ShaderID);
		
		// Pass in vertex buffer data and buffer of tiles(it will handle null bitmaps)
		pShader->UpdateVertexData(pPipeline, vertexBufferName, pVertexArray, TILE_VERTICES, 4);

		// Calculate the texture coords
		Vector2 tilemapTextureOriginCoord = Vector2(0, 0);
		Vector2 tilemapTextureSizeCoord = Vector2(0, 0);
		if (buffer.first != nullptr)
		{
			tilemapTextureOriginCoord =
				GenericPipeline::s_ActiveCamera.WorldToViewport(
					mp_Tilemap->TileToWorld(buffer.second.first)); // Convert the origin to viewport
			tilemapTextureSizeCoord =
				GenericPipeline::s_ActiveCamera.WorldToViewport(
					mp_Tilemap->TileToWorld(buffer.second.second) - mp_Tilemap->Origin, false);
		}

		glUseProgram(pShader->GetProgram()); // Set the program
		
		// Set Uniform variables
		glUniform2f(glGetUniformLocation(pShader->GetProgram(), TextureOriginCoordName),
			tilemapTextureOriginCoord.X, tilemapTextureOriginCoord.Y); // Set the tilemap tex origin
		glUniform2f(glGetUniformLocation(pShader->GetProgram(), TextureSizeCoordName),
			tilemapTextureSizeCoord.X, tilemapTextureSizeCoord.Y); // Set the tilemap tex size in uv coord
		glUniform2i(glGetUniformLocation(pShader->GetProgram(), TextureSizeName),
			buffer.second.second.X, buffer.second.second.Y); // Set the tilemap tex width/height

		if (buffer.first != nullptr) // BaseShaderType::UpdateVertexData(...) will not create the texture if data is nullptr
		{
			glUniform1i(glGetUniformLocation(pShader->GetProgram(), TextureName), 0); // Texture unit 0
			glBindTexture(GL_TEXTURE_2D, m_TextureBufferName);
			glActiveTexture(GL_TEXTURE0);
		}

		// Draw quads
		glDrawArrays(GL_QUADS, 0, 4);

		// Release buffers
		if (buffer.first != nullptr)
		{
			// TODO: Clear buffers
			pShader->p_ShaderInformation->Release();
			free(buffer.first);
		}
	}

	void TilemapMesh::Release()
	{
		// TODO: Clear buffers
		delete(this);
	}
	std::pair<HEX_COLOR* const, std::pair<Vector2Int, Vector2Int>> TilemapMesh::CreateOnScreenTilesBuffer()
	{
		// Find the two tile positions that correspond to the bottom left and top right of the screen
		Vector2Int bottomLeft = std::clamp(mp_Tilemap->WorldToTile(
			GenericPipeline::s_ActiveCamera.ViewportToWorld(
				Vector2(0, 0))),
			{}, (Vector2)mp_Tilemap->GetTileBounds() - Vector2(1, 1));//,
		//bottomLeftChunk = pTilemap->Container.TileToChunkCoordinates(bottomLeft);
		Vector2Int topRight = std::clamp(mp_Tilemap->WorldToTile(
			GenericPipeline::s_ActiveCamera.ViewportToWorld(
				Vector2(1, 1))),
			{}, (Vector2)mp_Tilemap->GetTileBounds() - Vector2(1, 1));//,
		//topRightChunk = pTilemap->Container.TileToChunkCoordinates(topRight);

// If either bounding axis converges with its opposite, return a null bitmap and don't draw anything
		if (bottomLeft.X == topRight.X || bottomLeft.Y == topRight.Y)
			return std::make_pair((HEX_COLOR* const)nullptr, std::make_pair(Vector2Int(0, 0), Vector2Int(0, 0)));

		// Width and height of texture
		Vector2Int textureSize = topRight - bottomLeft + Vector2Int(1, 1); // This value should never be 0 or greater than the size of the tilemap

		// Buffer variables
		int tilesCount = textureSize.X * textureSize.Y;
		HEX_COLOR* const pTiles = (HEX_COLOR*)malloc(sizeof(HEX_COLOR) * tilesCount);
		// Variables used inside loops
		HEX_COLOR pixelColor;
		for (int px = 0; px < textureSize.X; px++)
		{
			for (int py = 0; py < textureSize.Y; py++)
			{
				Tile tile = mp_Tilemap->GetTile(bottomLeft + Vector2Int(px, py));

				// Get the pixel color
				pixelColor = 0x0;//0xff00ffff
				if (tile.HasValue)
					pixelColor = tile.Color;
				// Assign the pixel color
				pTiles[px + py * textureSize.X] = pixelColor;
			}
		}
		return std::make_pair(pTiles, std::make_pair(bottomLeft, textureSize));
	}
}