#include "HealthDisplayComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "RenderComponent.h"
#include <iostream>

dae::HealthDisplayComponent::HealthDisplayComponent(GameObject* pObject)
	:BaseComponent::BaseComponent(pObject)
{
	m_pHealthComponent = pObject->GetComponent<HealthComponent>();
	const int health = m_pHealthComponent->GetLives();
	m_pHealthComponent->GetLivesDisplay()->AddObserver(this);

	m_pRenderComponent = pObject->AddComponent<RenderComponent>();
	const std::string filepath = "Sprites/UI/Heart.png";
	auto textureSize = m_pRenderComponent->GetTextureSize(filepath);
	const float offset = 5.f;

	for (int i = 0; i < health - 1; i++)
	{
		m_pRenderComponent->SetTexture(filepath,true,{0,i * (textureSize.y + offset) });
	}
}

void dae::HealthDisplayComponent::Notify(Event e, const GameObject*)
{
	switch (e)
	{
	case dae::Event::PlayerDies:
		UpdateLivesUI(m_pHealthComponent->GetLives());
		break;
	case dae::Event::ScoreChanged:
		break;
	default:
		break;
	}
}

void dae::HealthDisplayComponent::UpdateLivesUI(const int lives)
{
	int maxLives = m_pHealthComponent->GetMaxLives();
	for (int i = 0; i < (maxLives - lives); i++)
	{
		m_pRenderComponent->SetRenderTexture(false,(maxLives - 2) - i);
	}
}
