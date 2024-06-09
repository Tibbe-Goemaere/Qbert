#include <stdexcept>
#include <SDL_ttf.h>
#include "RenderComponent.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include <iostream>

dae::RenderComponent::RenderComponent(GameObject* pParent)
	:BaseComponent::BaseComponent(pParent)
{ }

void dae::RenderComponent::Render() const
{
	const auto& pos = m_pParent->GetWorldPosition(); 
	for (const auto& texture : m_pTextures)
	{
		if (texture.pTexture && texture.renderTexture)
		{
			Renderer::GetInstance().RenderTexture(*texture.pTexture, pos.x + texture.offset.x, pos.y + texture.offset.y);
		}
	}

	if (m_pTextTexture)
	{
		Renderer::GetInstance().RenderTexture(*m_pTextTexture, pos.x, pos.y);
	}
}

void dae::RenderComponent::SetTexture(const std::string& filename, bool renderTexture, glm::vec2 offset)
{
	auto pTexture = ResourceManager::GetInstance().LoadTexture(filename);
	m_pTextures.push_back({ pTexture,offset,renderTexture });
}

void dae::RenderComponent::SetTextTexture(std::shared_ptr<Texture2D> texture)
{
	m_pTextTexture = texture;
}

void dae::RenderComponent::SetRenderTexture(bool renderTexture, int textureIndex)
{
	if (textureIndex < static_cast<int>(m_pTextures.size()))
	{
		m_pTextures[textureIndex].renderTexture = renderTexture;
	}
}

void dae::RenderComponent::HideAllTextures()
{
	for (auto& texture : m_pTextures)
	{
		texture.renderTexture = false;
	}
}

void dae::RenderComponent::ShowAllTextures()
{
	for (auto& texture : m_pTextures)
	{
		texture.renderTexture = true;
	}
}

bool dae::RenderComponent::IsShowingTexture(int textureIndex) const
{
	return m_pTextures[textureIndex].renderTexture;
}

int dae::RenderComponent::GetAmountOfTextures()
{
	return static_cast<int>(m_pTextures.size());
}

glm::vec2 dae::RenderComponent::GetTextureSize(const std::string& filename) const
{
	if (filename.empty())
	{
		return glm::vec2(m_pTextures[0].pTexture->GetSize().x, m_pTextures[0].pTexture->GetSize().y);
	}
	auto pTexture = ResourceManager::GetInstance().LoadTexture(filename);
	return glm::vec2(pTexture->GetSize().x, pTexture->GetSize().y);
}

glm::vec2 dae::RenderComponent::GetTextureSizeViaIdx(int textureIdx) const
{
	if (m_pTextures[textureIdx].pTexture != nullptr)
	{
		return glm::vec2(m_pTextures[textureIdx].pTexture->GetSize().x, m_pTextures[textureIdx].pTexture->GetSize().y);
	}
	return { 0,0 };
}

glm::vec2 dae::RenderComponent::GetTextTextureSize() const
{
	return m_pTextTexture->GetSize();
}

