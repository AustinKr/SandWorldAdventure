#pragma once
#include "SWAEngine/dllClause.h"
#include "SWAEngine/Tilemap/Tilemap.h"
#include "GP2D/Pipeline/Mesh/IMesh.h"
#include "GP2D/Math/Vertex2D.h"

namespace SWAEngine::Tilemap
{
	struct SWA_ENGINE_API TilemapMesh : GP2D::Pipeline::Mesh::IMesh
	{
	public:
		static const char* TextureName;
		static const char* TextureOriginCoordName;
		static const char* TextureSizeCoordName;
		static const char* TextureSizeName;

		int ShaderID;

		virtual bool Render(GP2D::Pipeline::IPipeline* pPipeline, GP2D::Pipeline::UINT vertexBufferName, GP2D::Pipeline::UINT pVertexArray) override;
		virtual void Release() override;

		TilemapMesh(Tilemap* const pTilemap, int shaderID);

	private:
		Tilemap* const mp_Tilemap;
		GLuint m_TextureBufferName; // TODO: Need to create texture(use code from TilemapShaderInformation.cpp)

		static const GP2D::Math::Vertex2D TILE_VERTICES[4];
		std::pair<HEX_COLOR* const, std::pair<Vector2Int, Vector2Int>> CreateOnScreenTilesBuffer();
	};
}