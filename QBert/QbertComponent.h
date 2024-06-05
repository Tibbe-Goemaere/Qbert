#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	enum class Direction;
	class ScoreComponent;
	class HealthComponent;
	class LevelComponent;
	class CollisionComponent;
	class MoveComponent;
	class DiskComponent;

	class QbertComponent final : public BaseComponent, public Observer
	{
	public:
		void Update() override;

		QbertComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel, float speed = 150.f);
		virtual ~QbertComponent() = default;
		QbertComponent(const QbertComponent& other) = delete;
		QbertComponent(QbertComponent&& other) = delete;
		QbertComponent& operator=(const QbertComponent& other) = delete;
		QbertComponent& operator=(QbertComponent&& other) = delete;

		void MovePlayer(const glm::vec2& direction);
		void GetOffDisk();

		void Notify(Event e, const GameObject* pObject) override;
	private:
		dae::ScoreComponent* m_pScoreComponent;
		dae::HealthComponent* m_pHealthComponent;
		DiskComponent* m_pDisk;
		bool m_justDropped;
		dae::MoveComponent* m_pMovecomponent;

		CollisionComponent* m_pCollision;
	};
}
