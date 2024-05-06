#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class GameObject;

	class HealthComponent final : public BaseComponent
	{
	public:
		HealthComponent(dae::GameObject* pParent, const int lives = 3);
		virtual ~HealthComponent() = default;
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		void RemoveHealth();
		int GetLives() const;
		Subject* GetLivesDisplay() const;
	private:
		int m_lives;
		std::unique_ptr<Subject> m_pLivesDisplayEvent;
	};
}
