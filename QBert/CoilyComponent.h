#pragma once
#include "BaseComponent.h"
#include <string>

namespace dae
{
	class CoilyState;
	class GameObject;
	class MoveDownComponent;
	class FollowPlayerComponent;
	class RenderComponent;
	class LevelComponent;

	class CoilyComponent final : public BaseComponent
	{
	public:
		void Update() override;

		CoilyComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel);
		virtual ~CoilyComponent() = default;
		CoilyComponent(const CoilyComponent& other) = delete;
		CoilyComponent(CoilyComponent&& other) = delete;
		CoilyComponent& operator=(const CoilyComponent& other) = delete;
		CoilyComponent& operator=(CoilyComponent&& other) = delete;

		void SetTexture(const std::string& filepath);

	private:
		dae::MoveDownComponent* m_pEggMovement;
		dae::FollowPlayerComponent* m_pCoilyMovement;
		dae::RenderComponent* m_pRenderComponent;
		bool doOnce = true;

		std::unique_ptr<CoilyState> m_pCoilyState;
	};
}


