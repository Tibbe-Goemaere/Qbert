#pragma once
#include "BaseComponent.h"

namespace dae
{
	class MoveComponent;
	class LevelComponent;

	class MoveDownComponent final : public BaseComponent
	{
	public:
		void Update() override;

		MoveDownComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel, float speed = 150.f, float waitTime = 0.5f, bool canTransform = false);
		virtual ~MoveDownComponent() = default;
		MoveDownComponent(const MoveDownComponent& other) = delete;
		MoveDownComponent(MoveDownComponent&& other) = delete;
		MoveDownComponent& operator=(const MoveDownComponent& other) = delete;
		MoveDownComponent& operator=(MoveDownComponent&& other) = delete;
	private:
		dae::MoveComponent* m_pMoveComponent;
		const float m_speed;
		const bool m_canTransform;
		float m_timer;
		const float m_waitTime;
	};
}
