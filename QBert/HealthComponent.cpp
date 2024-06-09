#include "HealthComponent.h"
#include "GameManager.h"


dae::HealthComponent::HealthComponent(GameObject* pParent, const int lives)
	:m_lives{ lives }
	, BaseComponent::BaseComponent(pParent)
	,m_maxLives{lives}
{
	m_pLivesDisplayEvent = std::make_unique<Subject>();
}

void dae::HealthComponent::RemoveHealth()
{
	--m_lives;
	if (m_lives <= 0)
	{
		GameManager::GetInstance().MakeLeaderboard();
	}
	else
	{
		m_pLivesDisplayEvent->NotifyObservers(dae::Event::PlayerDies, m_pParent);
	}
}

int dae::HealthComponent::GetLives() const
{
	return m_lives;
}

dae::Subject* dae::HealthComponent::GetLivesDisplay() const
{
	return m_pLivesDisplayEvent.get();
}

int dae::HealthComponent::GetMaxLives() const
{
	return m_maxLives;
}


