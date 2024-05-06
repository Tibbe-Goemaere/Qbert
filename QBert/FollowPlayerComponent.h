#pragma once
#include "BaseComponent.h"

namespace dae
{
	class MoveComponent;
	class LevelComponent;

	class FollowPlayerComponent final : public BaseComponent
	{
	public:
		void Update() override;

		FollowPlayerComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel, float speed = 150.f, float waitTime = 0.5f);
		virtual ~FollowPlayerComponent() = default;
		FollowPlayerComponent(const FollowPlayerComponent& other) = delete;
		FollowPlayerComponent(FollowPlayerComponent&& other) = delete;
		FollowPlayerComponent& operator=(const FollowPlayerComponent& other) = delete;
		FollowPlayerComponent& operator=(FollowPlayerComponent&& other) = delete;
	private:
		dae::MoveComponent* m_pMoveComponent;
		const float m_speed;
		float m_timer;
		const float m_waitTime;
	};
}
