#pragma once
#include "GP2D/Math/vector2.h"
#include "GP2D/Math/Float4.h"

namespace SWA
{
	// contains information sent to the DefaultSpriteShader from individual Mesh objects
	struct SpriteShaderProperties
	{
	private:
		SpriteShaderProperties() = default;

	public:
		const char* TextureName;
		GP2D::Math::Float4 TextureColor;

		GP2D::Math::Int2 OutlineThickness;
		GP2D::Math::Float4 OutlineColor;
		float OutlineClip;

		static SpriteShaderProperties* CreateProperties(const char* textureName = nullptr, GP2D::Math::Float4 textureColor = {1,1,1,1}, GP2D::Math::Float2 outlineThickness = {}, GP2D::Math::Float4 outlineColor = {1,1,1,1}, float outlineClip = 0.1);
	};
}