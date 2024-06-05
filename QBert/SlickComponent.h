#pragma once
#include "BaseComponent.h"
#include <string>
#include "Observer.h"

namespace dae
{
	class LevelComponent;
	class MoveComponent;
	class CollisionComponent;

	class SlickComponent final : public BaseComponent, public Observer
	{
	public:
		void Update();

		SlickComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel, const std::string& filePath);
		virtual ~SlickComponent() = default;
		SlickComponent(const SlickComponent& other) = delete;
		SlickComponent(SlickComponent&& other) = delete;
		SlickComponent& operator=(const SlickComponent& other) = delete;
		SlickComponent& operator=(SlickComponent&& other) = delete;

		void Notify(Event e, const GameObject* pObject) override;
	private:
		float m_timer;
		const float m_waitTime;
		MoveComponent* m_pMoveComponent;

		CollisionComponent* m_pCollision;

		void ChangeBackBlock();
	};
}
