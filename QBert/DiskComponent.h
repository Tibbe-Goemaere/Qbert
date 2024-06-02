#pragma once
#include "BaseComponent.h"
#include "utility"

namespace dae
{
	class GameObject;
	class RenderComponent;
	class LevelComponent;

	class DiskComponent final : public BaseComponent
	{
	public:
		void Update() override;

		DiskComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel, bool isOnLeftSide = true, int rowNr = 0);
		virtual ~DiskComponent() = default;
		DiskComponent(const DiskComponent& other) = delete;
		DiskComponent(DiskComponent&& other) = delete;
		DiskComponent& operator=(const DiskComponent& other) = delete;
		DiskComponent& operator=(DiskComponent&& other) = delete;

		std::pair<int, int> GetGridPosition() const;
		void Activate();
		GameObject* GetParent() const;
	private:
		dae::RenderComponent* m_pRenderComponent;
		dae::LevelComponent* m_pLevelComponent;
		const float m_speed;
		std::pair<int, int> m_gridPosition;
		bool m_activated;
		glm::vec3 m_targetPosition;
		glm::vec3 m_direction;

		void SetStartPosition(bool isOnLeftSide, int rowNr);
	};
}


