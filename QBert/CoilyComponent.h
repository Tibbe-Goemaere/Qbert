#pragma once
#include "BaseComponent.h"
#include <string>
#include "CoilyState.h"

namespace dae
{
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
		bool IsAtBottom();
		void ChangeToSnake();
		bool IsDead();

	private:
		dae::MoveDownComponent* m_pEggMovement;
		dae::FollowPlayerComponent* m_pCoilyMovement;
		dae::RenderComponent* m_pRenderComponent;

		std::unique_ptr<CoilyState> m_pCoilyState;
	};
}


