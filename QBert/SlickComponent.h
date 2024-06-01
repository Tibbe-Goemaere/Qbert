#pragma once
#include "BaseComponent.h"
#include <string>

namespace dae
{
	class LevelComponent;
	class MoveComponent;

	class SlickComponent final : public BaseComponent
	{
	public:
		void Update();

		SlickComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel, const std::string& filePath);
		virtual ~SlickComponent() = default;
		SlickComponent(const SlickComponent& other) = delete;
		SlickComponent(SlickComponent&& other) = delete;
		SlickComponent& operator=(const SlickComponent& other) = delete;
		SlickComponent& operator=(SlickComponent&& other) = delete;
	private:
		float m_timer;
		const float m_waitTime;
		MoveComponent* m_pMoveComponent;
	};
}
