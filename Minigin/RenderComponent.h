#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "Transform.h"
#include "BaseComponent.h"
#include <vector>

namespace dae
{
	class Font;
	class Texture2D;

	struct TextureInfo
	{
		std::shared_ptr<Texture2D> pTexture;
		glm::vec2 offset;
		bool renderTexture = true;
	};

	class RenderComponent final : public BaseComponent
	{
	public:
		void Render() const override;

		void SetTexture(const std::string& filename, bool renderTexture = true, glm::vec2 offset = { 0,0 });
		void SetTextTexture(std::shared_ptr<Texture2D> texture);
		void SetRenderTexture(bool renderTexture, int textureIndex = 0);

		glm::vec2 GetTextureSize(const std::string& filename = "") const;
		glm::vec2 GetTextureSizeViaIdx(int textureIdx = 0) const;

		RenderComponent(GameObject* pParent);
		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;
	private:
		std::vector<TextureInfo> m_pTextures;
		std::shared_ptr<Texture2D> m_pTextTexture;
	};
}
