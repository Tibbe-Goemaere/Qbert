#pragma once
#include "BaseComponent.h"
#include <string>
#include "CoilyMoveState.h"

namespace dae
{
	class GameObject;
	class FollowPlayerComponent;
	class RenderComponent;
	class LevelComponent;
	class MoveComponent;

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
		void ChangeToSnake();
		bool IsDead();
		GameObject* GetParent() const;

	private:
		dae::MoveComponent* m_pMoveComponent;
		dae::FollowPlayerComponent* m_pCoilyMovement;
		dae::RenderComponent* m_pRenderComponent;
		std::unique_ptr<CoilyMoveState> m_pCoilyState;
	};
}


