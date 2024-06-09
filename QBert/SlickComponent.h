#pragma once
#include "BaseComponent.h"

namespace dae
{
	class LevelComponent;
	class MoveComponent;
	class CollisionComponent;

	class SlickComponent final : public BaseComponent
	{
	public:
		void Update();

		SlickComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel);
		virtual ~SlickComponent() = default;
		SlickComponent(const SlickComponent& other) = delete;
		SlickComponent(SlickComponent&& other) = delete;
		SlickComponent& operator=(const SlickComponent& other) = delete;
		SlickComponent& operator=(SlickComponent&& other) = delete;

	private:
		float m_timer;
		const float m_waitTime;
		MoveComponent* m_pMoveComponent;

		CollisionComponent* m_pCollision;

		void ChangeBackBlock();
	};
}
