#pragma once
#include "Command.h"
#include <string>
#include "HealthComponent.h"

namespace dae
{
	class GameObject;
	class QbertComponent;
	class UIComponent;

	class MovePlayerCommand final : public dae::Command
	{
	public:
		MovePlayerCommand(dae::GameObject* pObject, const glm::vec2& direction);

		void Execute() override;
	private:
		dae::GameObject* m_pGameObject;
		const glm::vec2 m_direction;
		dae::QbertComponent* m_pPlayerMovement;
	};
	
	class MoveArrows final : public dae::Command
	{
	public:
		MoveArrows(bool goUp,UIComponent* pUIComponent);

		void Execute() override;
	private:
		bool m_goUp;
		UIComponent* m_pUIComponent;
	};

	class Scene;
	class ChooseGameMode final : public dae::Command
	{
	public:
		ChooseGameMode(UIComponent* pUiComponen);

		void Execute() override;
	private:
		UIComponent* m_pUIComponent;
	};

	class DieCommand final : public Command
	{
	public:
		DieCommand(HealthComponent* pHealth);

		void Execute() override;
	private:
		HealthComponent* m_pHealthComponent;
	};

}


