#pragma once
#include "MoveComponent.h"

namespace dae
{
	class LevelComponent;

	class MoveDownComponent final : public MoveComponent
	{
	public:
		void Update() override;

		MoveDownComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel, float speed = 150.f, float waitTime = 0.5f);
		virtual ~MoveDownComponent() = default;
		MoveDownComponent(const MoveDownComponent& other) = delete;
		MoveDownComponent(MoveDownComponent&& other) = delete;
		MoveDownComponent& operator=(const MoveDownComponent& other) = delete;
		MoveDownComponent& operator=(MoveDownComponent&& other) = delete;

		bool IsAtBottom() const;
		LevelComponent* GetLevel();
		Block* GetCurrentBlock();
	private:
		float m_timer;
		const float m_waitTime;
		bool m_isAtBottom;
	};
}
