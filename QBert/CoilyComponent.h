#pragma once
#include "BaseComponent.h"
#include <string>
#include "CoilyState.h"
#include "Observer.h"

namespace dae
{
	class GameObject;
	class RenderComponent;
	class LevelComponent;
	class MoveComponent;
	class CollisionComponent;

	class CoilyComponent final : public BaseComponent, public Observer
	{
	public:
		void Update() override;

		CoilyComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel);
		virtual ~CoilyComponent() = default;
		CoilyComponent(const CoilyComponent& other) = delete;
		CoilyComponent(CoilyComponent&& other) = delete;
		CoilyComponent& operator=(const CoilyComponent& other) = delete;
		CoilyComponent& operator=(CoilyComponent&& other) = delete;

		GameObject* GetParent() const;
		void Notify(Event e, const GameObject* pObject) override;
		void CheckCollision();
		void MoveCoily(const glm::vec2 direction);
	private:
		dae::MoveComponent* m_pMoveComponent;
		dae::RenderComponent* m_pRenderComponent;
		std::unique_ptr<CoilyState> m_pCoilyState;
		CollisionComponent* m_pCollision;
	};
}


