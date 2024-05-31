#pragma once
#include "BaseComponent.h"
#include <string>
#include "CoilyState.h"

namespace dae
{
	class GameObject;
	class RenderComponent;
	class LevelComponent;
	class MoveComponent;

	class CoilyComponent final : public BaseComponent
	{
	public:
		void Update() override;

		CoilyComponent(dae::GameObject* pParent, dae::LevelComponent* pLevel);
		virtual ~CoilyComponent() = default;
		CoilyComponent(const CoilyComponent& other) = delete;
		CoilyComponent(CoilyComponent&& other) = delete;
		CoilyComponent& operator=(const CoilyComponent& other) = delete;
		CoilyComponent& operator=(CoilyComponent&& other) = delete;

		GameObject* GetParent() const;

	private:
		dae::MoveComponent* m_pMoveComponent;
		dae::RenderComponent* m_pRenderComponent;
		std::unique_ptr<CoilyState> m_pCoilyState;
	};
}


