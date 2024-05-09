#pragma once
#include "MoveComponent.h"

namespace dae
{
	class LevelComponent;
	enum class Direction;

	class FollowPlayerComponent final : public MoveComponent
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
		float m_timer;
		const float m_waitTime;

		dae::Direction FindNextBlock();
	};
}
