#pragma once
#include "BaseComponent.h"
#include <string>

namespace dae
{
	class LevelComponent;
	class MoveComponent;

	class UggComponent final : public BaseComponent
	{
	public:
		void Update();

		UggComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel, const std::string& filePath, bool isUgg);
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
	};
}
