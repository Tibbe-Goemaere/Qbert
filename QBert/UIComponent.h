#pragma once
#include "BaseComponent.h"
#include <vector>

namespace dae
{
	class GameObject;
	class RenderComponent;

	class UIComponent final : public BaseComponent
	{
	public:
		UIComponent(GameObject* pParent, std::vector<RenderComponent*> pArrows);
		virtual ~UIComponent() = default;

		UIComponent(const UIComponent& other) = delete;
		UIComponent(UIComponent&& other) = delete;
		UIComponent& operator=(const UIComponent& other) = delete;
		UIComponent& operator=(UIComponent&& other) = delete;

		int GetCurrentSelection() const;
		void ChangeSelection(bool goUp);
	private:
		int m_currentSelection;
		std::vector<RenderComponent*> m_pArrows;
	};
}


