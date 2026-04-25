#pragma once
#include <SWAEngine/dllClause.h>
#include <SWAEngine/EventHandler.h>
#include <SWAEngine/Math/vector2.h>
#include <string>
#include "IComponent.h"

namespace SWAEngine::GameObject::Component
{
	struct SWA_ENGINE_API Transform : IComponent
	{
	private:
		Math::Vector2 m_Position;
		Math::Vector2 m_Scale;

	public:
		EventHandler<Math::Vector2> OnSetPosition;
		EventHandler<Math::Vector2> OnSetScale;
		
		Transform(Math::Vector2 position = {}, Math::Vector2 scale = {});

		Math::Vector2 GetPosition() const;
		void SetPosition(Math::Vector2 newPosition);
		Math::Vector2 GetScale() const;
		void SetScale(Math::Vector2 newScale);

		static std::string const GetName();
		virtual void Initialize(std::string objName) override;
		virtual void SetActive(bool state) override;
		virtual bool GetActive() override; // Returns true by default
		virtual void Update(std::string, Math::Time) override;
		virtual void Release() override;
	};
}