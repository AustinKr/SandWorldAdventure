#pragma once
#include "HeaderFiles/GraphicsPipeline/Meshes/Mesh.h"
#include "GUIElement.h"

namespace SandboxEngine::GUISystem
{
	class GUISprite : public GUIElement
	{
	private:
		GraphicsPipeline::Mesh* mp_Mesh;

		virtual void SetElementPosition(Vector2Int position) override;
		virtual void SetElementScale(Vector2Int scale) override;
	public:

		GUISprite(GUISystem* pSystem, int meshID, const char* imagePath);
		GUISprite(GUISystem* pSystem, UID parent, int meshID, const char* imagePath);

		virtual Vector2Int GetPosition() override;
		virtual Vector2Int GetScale() override;
	};
}