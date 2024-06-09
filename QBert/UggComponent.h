#pragma once
#include "BaseComponent.h"

namespace dae
{
	class LevelComponent;
	class MoveComponent;
	class CollisionComponent;

	class UggComponent final : public BaseComponent
	{
	public:
		void Update();

		UggComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel);
		virtual ~UggComponent() = default;
		UggComponent(const UggComponent& other) = delete;
		UggComponent(UggComponent&& other) = delete;
		UggComponent& operator=(const UggComponent& other) = delete;
		UggComponent& operator=(UggComponent&& other) = delete;

	private:
		float m_timer;
		const float m_waitTime;
		MoveComponent* m_pMoveComponent;
		bool m_isUgg;
		glm::vec2 m_lastDirection;
		CollisionComponent* m_pCollision;
	};
}
