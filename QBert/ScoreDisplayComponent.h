#pragma once
#include "Observer.h"
#include "BaseComponent.h"
#include <string>

namespace dae
{
	class TextComponent;
	class ScoreComponent;
	class GameObject;

	class ScoreDisplayComponent final : public Observer, public BaseComponent
	{
	public:
		ScoreDisplayComponent(GameObject* pObject, TextComponent* pText);
		virtual ~ScoreDisplayComponent() = default;
		void Notify(Event e, const GameObject* pObject) override;

	private:
		void UpdatePointsText(const int lives);

		const std::string m_pointsText;
		dae::TextComponent* m_pTextComponent;
		dae::ScoreComponent* m_pScoreComponent;
	};
}


