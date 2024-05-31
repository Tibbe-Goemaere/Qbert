#pragma once
#include "BaseComponent.h"

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

	private:
		dae::RenderComponent* m_pRenderComponent;
		dae::LevelComponent* m_pLevelComponent;
		const float m_speed;

		void SetStartPosition(bool isOnLeftSide, int rowNr);
	};
}


