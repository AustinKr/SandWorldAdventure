#include "SWAEngine/Tilemap/TilemapMesh.h"
#include "GP2D/Pipeline/Window/Window.h"
#include "GP2D/Pipeline/GenericPipeline.h"

#include <algorithm>
#include <malloc.h>

#define GLFW_INCLUDE_NONE
#include "GL/glew.h"
#include "GLFW/glfw3.h"

using namespace GP2D;
using namespace GP2D::Math;
using namespace GP2D::Pipeline;
using namespace GP2D::Pipeline::Shader;

namespace SWAEngine::Tilemap
{
	const Vertex2D TilemapMesh::MESH_VERTICES[6] =
	{
		Vertex2D{{-1, -1}, {0, 0, 0}},
		Vertex2D{{ 1, -1}, {1, 0, 0}},
		Vertex2D{{ 1,  1}, {1, 1, 0}},
		
		Vertex2D{{ 1,  1}, {1, 1, 0}},
		Vertex2D{{-1,  1}, {0, 1, 0}},
		Vertex2D{{-1, -1}, {0, 0, 0}},
	};

	const char* TilemapMesh::TextureName = "TilemapTex";
	const char* TilemapMesh::TextureOriginCoordName = "TexOriginCoord";
	const char* TilemapMesh::TextureSizeCoordName = "TexSizeCoord";
	const char* TilemapMesh::TextureSizeName = "TexSize";

	TilemapMesh::TilemapMesh(Tilemap* const pTilemap, int shaderID) : mp_Tilemap(pTilemap), ShaderID(shaderID)
	{
		IsActive = true;

		// Create
		glGenTextures(1, &m_TextureBufferName);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_TextureBufferName);
		// Set params
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void TilemapMesh::Release()
	{
		glDeleteTextures(1, &m_TextureBufferName);
		delete(this);
	}

	// TODO: Could mvoe the origin of the tilemap mesh to the actual tilemap orign and then the tilemap wouldn't overrite the entire screen
	bool TilemapMesh::Render(IPipeline* pPipeline, GP2D::UINT vertexBufferName, GP2D::UINT pVertexArray)
	{
		if (mp_Tilemap->IsEmpty())
			return true; // Nothing to render so just skip

		// Create the buffer
		auto information = CreateTileBufferData(); 
		if (information.TextureSize.X <= 1 || information.TextureSize.Y <= 1) // At least 2x2 tiles
			return true; // Nothing to render so just skip

		// Get the shader and its information which allows us to communicate with specific glsl code
		auto pShader = GenericPipeline::s_Shaders.TryGetShader<BaseShaderType>(ShaderID);
		
		// Pass in vertex buffer data (for drawing vertices and triangles)
		pShader->UpdateVertexData(pPipeline, vertexBufferName, pVertexArray, MESH_VERTICES, 6);

		// Try render with the texture
		if (information.p_Buffer == nullptr) // Buffer shouldn't be null at this point
			return false;

		// Bind and set the texture pixels
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_TextureBufferName);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, information.TextureSize.X, information.TextureSize.Y, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, information.p_Buffer);

		// Update shader uniforms and buffer
		UpdateShaderData(pShader, information);

		// Draw quads
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Release buffer pixels
		std::free(information.p_Buffer);

		return true;
	}

	TileBufferInformation TilemapMesh::CreateTileBufferData()
	{
		Float2 screenWorldBottomLeft = GenericPipeline::s_ActiveCamera.ViewportToWorld(Float2(0, 0));
		Float2 screenWorldTopRight = GenericPipeline::s_ActiveCamera.ViewportToWorld(Float2(1, 1));

		// Find the two tile positions that correspond to the bottom left and top right of the screen
		Math::Vector2Int bottomLeft = std::clamp(
			mp_Tilemap->WorldToTile(Math::Vector2(screenWorldBottomLeft.X, screenWorldBottomLeft.Y)),
			Math::Vector2Int(),
			mp_Tilemap->GetBounds());

		Math::Vector2Int topRight = std::clamp(
			mp_Tilemap->WorldToTile(Math::Vector2(screenWorldTopRight.X, screenWorldTopRight.Y)),
			Math::Vector2Int(),
			mp_Tilemap->GetBounds());

		// If either bounding axis converges with its opposite, return a null bitmap and don't draw anything
		if (bottomLeft.X == topRight.X || bottomLeft.Y == topRight.Y)
			return {};

		// Width and height of texture
		Math::Vector2Int textureSize = topRight - bottomLeft + Math::Vector2Int(1,1);

		// Buffer variables
		int pixelsCount = textureSize.X * textureSize.Y;
		GP2D_HEX_COLOR* const pTiles = (GP2D_HEX_COLOR*)malloc(sizeof(GP2D_HEX_COLOR) * pixelsCount);
		// Variables used inside loops
		for (int px = 0; px < textureSize.X; px++)
		{
			for (int py = 0; py < textureSize.Y; py++)
			{
				Tile tile = mp_Tilemap->GetActiveTile(bottomLeft + Math::Vector2Int(px, py));
				//0xff00ffff
				// Assign the pixel color
				pTiles[px + py * textureSize.X] = tile.Color;
			}
		}
		return { pTiles, {bottomLeft.X, bottomLeft.Y}, {textureSize.X, textureSize.Y} };
	}

	void TilemapMesh::UpdateShaderData(BaseShaderType* pShader, TileBufferInformation information)
	{
		// Calculate the texture coords
		Math::Vector2 worldOrigin = mp_Tilemap->TileToWorld({ information.TextureOrigin.X,information.TextureOrigin.Y });
		Math::Vector2 worldSize = mp_Tilemap->TileToWorld({ information.TextureSize.X,information.TextureSize.Y }, false);
		
		// Convert the coords to viewport(since the mesh takes up the screen, the uv coords given to the shader code are in viewport units)
		Float2 tilemapTextureOriginCoord = GenericPipeline::s_ActiveCamera.WorldToViewport({ (float)worldOrigin.X, (float)worldOrigin.Y });
		Float2 tilemapTextureSizeCoord = GenericPipeline::s_ActiveCamera.WorldToViewport({ (float)worldSize.X, (float)worldSize.Y }, false);
		
		// Set the program
		glUseProgram(pShader->GetProgram()); 

		// Set Uniform variables
		glUniform2f(glGetUniformLocation(pShader->GetProgram(), TextureOriginCoordName),
			tilemapTextureOriginCoord.X, tilemapTextureOriginCoord.Y); // Set the tilemap tex origin
		glUniform2f(glGetUniformLocation(pShader->GetProgram(), TextureSizeCoordName),
			tilemapTextureSizeCoord.X, tilemapTextureSizeCoord.Y); // Set the tilemap tex size in uv coord
		glUniform2i(glGetUniformLocation(pShader->GetProgram(), TextureSizeName),
			information.TextureSize.X, information.TextureSize.Y); // Set the tilemap tex width/height

		// Set uniform
		glUniform1i(glGetUniformLocation(pShader->GetProgram(), TextureName), 0); // Texture unit 0
	}
}