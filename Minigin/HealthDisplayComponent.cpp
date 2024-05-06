#include "HealthDisplayComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "TextComponent.h"

dae::HealthDisplayComponent::HealthDisplayComponent(GameObject* pObject, TextComponent* pText)
	:m_livesText{ "# lives: " }
	,BaseComponent::BaseComponent(pObject)
{
	m_pTextComponent = pText;
	m_pHealthComponent = pObject->GetComponent<HealthComponent>();
	const int health = m_pHealthComponent->GetLives();
	m_pTextComponent->SetText(m_livesText + std::to_string(health));
	m_pHealthComponent->GetLivesDisplay()->AddObserver(this);
}

void dae::HealthDisplayComponent::Notify(Event e, const GameObject*)
{
	switch (e)
	{
	case dae::Event::PlayerDies:
		UpdateLivesText(m_pHealthComponent->GetLives());
		break;
	case dae::Event::ScoreChanged:
		break;
	default:
		break;
	}
}

void dae::HealthDisplayComponent::UpdateLivesText(const int lives)
{
	m_pTextComponent->SetText(m_livesText + std::to_string(lives));
}
