#include "SWA/SpriteShaderProperties.h"

namespace SWA
{
	SpriteShaderProperties* SpriteShaderProperties::CreateProperties(const char* textureName, GP2D::Math::Float4 textureColor, GP2D::Math::Float2 outlineThickness, GP2D::Math::Float4 outlineColor, float outlineClip)
	{
		auto p = new SpriteShaderProperties();
		p->TextureName = textureName;
		p->TextureColor = textureColor;
		p->OutlineThickness = outlineThickness;
		p->OutlineColor = outlineColor;
		p->OutlineClip = outlineClip;

		return p;
	}
}