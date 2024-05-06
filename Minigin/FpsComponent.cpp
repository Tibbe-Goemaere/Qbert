#include <stdexcept>
#include <SDL_ttf.h>
#include "FpsComponent.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include <format>
#include "TimeManager.h"

dae::FpsComponent::FpsComponent(GameObject* pParent)
	:m_fps{ 60.f }
	,m_timer{ 0.f }
	,m_count{0}
	,BaseComponent::BaseComponent(pParent)
{}

dae::FpsComponent::~FpsComponent()
{
	
}

void dae::FpsComponent::Awake()
{
	m_pTextComponent = BaseComponent::m_pParent->GetComponent<TextComponent>();
}

void dae::FpsComponent::Update()
{
	BaseComponent::Update();

	m_timer += TimeManager::GetInstance().GetDeltaTime();
	m_count++;
	float updateFrequency = 1.f;

	if (m_timer >= updateFrequency)
	{
		//auto textComponent = BaseComponent::m_pParent->GetComponent<TextComponent>();
		if (m_pTextComponent)
		{
			m_pTextComponent->SetText(std::format("{:.1f}", static_cast<int>(m_count) / m_timer) + " FPS");
		}

		m_timer = 0.f;
		m_count = 0;
	}
}

