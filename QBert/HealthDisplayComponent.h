#pragma once
#include "Observer.h"
#include "BaseComponent.h"

namespace dae
{
	class HealthComponent;
	class RenderComponent;
	class GameObject;

	class HealthDisplayComponent final : public Observer, public BaseComponent
	{
	public:
		HealthDisplayComponent(GameObject* pObject);
		virtual ~HealthDisplayComponent() = default;
		void Notify(Event e, const GameObject* pObject) override;

	private:
		void UpdateLivesUI(const int lives);

		dae::HealthComponent* m_pHealthComponent;
		dae::RenderComponent* m_pRenderComponent;
	};
}


