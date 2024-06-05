#include <stdexcept>
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "RenderComponent.h"

dae::TextComponent::TextComponent(GameObject* pParent, const std::string& text, std::shared_ptr<Font> font)
	: m_needsUpdate(true), m_text(text), m_font(std::move(font)), m_textTexture(nullptr)
	, BaseComponent::BaseComponent(pParent)
{ 
	m_pRenderComponent = BaseComponent::m_pParent->GetComponent<RenderComponent>();
	MakeTextTexture();
}

void dae::TextComponent::Awake()
{
	
}

void dae::TextComponent::Update()
{
	if (m_needsUpdate)
	{
		MakeTextTexture();
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::MakeTextTexture()
{
	const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
	const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	m_textTexture = std::make_shared<Texture2D>(texture);
	m_needsUpdate = false;

	if (m_pRenderComponent)
	{
		m_pRenderComponent->SetTextTexture(m_textTexture);
	}
}


