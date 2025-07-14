#include "HeaderFiles/GUISystem/Elements/GUISprite.h"
#include "HeaderFiles/GUISystem/GUISystem.h"

namespace SandboxEngine::GUISystem
{
	GUISprite::GUISprite(GUISystem* pSystem, int meshID, const char* imagePath) : GUIElement(pSystem)
	{
		mp_Mesh = p_System->CreateTextureMesh({0,0},{1,1}, imagePath);
		mp_Mesh->color = float4(1, 1, 1, 1);
		p_System->RegisterMesh(mp_Mesh, meshID);
	}
	GUISprite::GUISprite(GUISystem* pSystem, UID parent, int meshID, const char* imagePath) : GUIElement(pSystem, parent)
	{
		GUISprite::GUISprite(pSystem, meshID, imagePath);
		//TODO: may be an issue here with the order of operations( so the parent would maybe be 0 insted of a value)
	}

	void GUISprite::SetElementPosition(Vector2Int position)
	{
		mp_Mesh->Origin = position;
	}
	void GUISprite::SetElementScale(Vector2Int scale)
	{
		mp_Mesh->Scale = scale;
	}

	Vector2Int GUISprite::GetPosition()
	{
		return mp_Mesh->Origin;
	}
	Vector2Int GUISprite::GetScale()
	{
		return mp_Mesh->Scale;
	}
}