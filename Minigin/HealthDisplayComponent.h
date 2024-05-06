#pragma once
#include "Observer.h"
#include <string>
#include "BaseComponent.h"

namespace dae
{
	class HealthComponent;
	class TextComponent;
	class GameObject;

	class HealthDisplayComponent final : public Observer, public BaseComponent
	{
	public:
		HealthDisplayComponent(GameObject* pObject, TextComponent* pText);
		virtual ~HealthDisplayComponent() = default;
		void Notify(Event e, const GameObject* pObject) override;

	private:
		void UpdateLivesText(const int lives);

		const std::string m_livesText;
		dae::TextComponent* m_pTextComponent;
		dae::HealthComponent* m_pHealthComponent;
	};
}


