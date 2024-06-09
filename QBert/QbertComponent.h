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
	class RenderComponent;

	class QbertComponent final : public BaseComponent, public Observer
	{
	public:
		void Update() override;

		QbertComponent(dae::GameObject* pParent, dae::HealthComponent* pHealthComponent, dae::LevelComponent* pLevel, const std::pair<int,int> gridPos, bool isPlayerTwo = false,float speed = 150.f);
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
		dae::RenderComponent* m_pRenderQbert;

		//Dying and coming to life
		bool m_isDead;
		float m_deadTimer;
		const float m_maxDeadTime;
		const float m_timeToRespawn;
		void Die();
		void Respawn();

		CollisionComponent* m_pCollision;

		bool m_dontMove;
	};
}
