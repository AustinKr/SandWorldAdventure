#pragma once
#include "SWAEngine/dllClause.h"
#include "SWAEngine/Tilemap/Tilemap.h"
#include "SWAEngine/Tilemap/TileBufferInformation.h"

#include "GP2D/Pipeline/Mesh/IMesh.h"
#include "GP2D/Math/Vertex2D.h"
#include "GP2D/Pipeline/Shader/BaseShaderType.h"

namespace SWAEngine::Tilemap
{
	// The mesh for the tilemap
	// Takes up the entire screen
	struct SWA_ENGINE_API TilemapMesh : GP2D::Pipeline::Mesh::IMesh
	{
	public:
		static const char* TextureName;
		static const char* TextureOriginCoordName;
		static const char* TextureSizeCoordName;
		static const char* TextureSizeName;

		const char* ShaderName;

		// Initializes the mesh and generates a texture to send to the shader
		TilemapMesh(Tilemap* const pTilemap, const char* shader);

		virtual void Release() override;

		virtual bool Render(GP2D::Pipeline::IPipeline* pPipeline, GP2D::UINT vertexBufferName, GP2D::UINT pVertexArray) override;

	private:
		Tilemap* const mp_Tilemap;
		GP2D::UINT m_TextureBufferName;

		// The verts for the mesh. Takes up the entire screen
		static const GP2D::Math::Vertex2D MESH_VERTICES[6];

		// TODO: Maybe rename
		// Creates the pixels for the texture buffer, which is a texture that is the visible(on screen) portion of the tilemap
		// The buffer must be re-created every frame
		TileBufferInformation CreateTileBufferData();

		void UpdateShaderData(GP2D::Pipeline::Shader::BaseShaderType* pShader, TileBufferInformation information);
	};
}