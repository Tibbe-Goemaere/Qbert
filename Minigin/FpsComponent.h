#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "BaseComponent.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent;
	class FpsComponent final : public BaseComponent
	{
	public:
		void Awake() override;
		void Update() override;

		FpsComponent(GameObject* pParent);
		virtual ~FpsComponent();
		FpsComponent(const FpsComponent& other) = delete;
		FpsComponent(FpsComponent&& other) = delete;
		FpsComponent& operator=(const FpsComponent& other) = delete;
		FpsComponent& operator=(FpsComponent&& other) = delete;
	private:
		float m_fps;
		float m_timer;
		int m_count;
		TextComponent* m_pTextComponent;
	};
}
