#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class GameObject;

	class HealthComponent final : public BaseComponent
	{
	public:
		HealthComponent(dae::GameObject* pParent, const int lives = 4);
		virtual ~HealthComponent() = default;
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		void RemoveHealth();
		int GetLives() const;
		Subject* GetLivesDisplay() const;
		int GetMaxLives() const;
	private:
		int m_lives;
		const int m_maxLives;
		std::unique_ptr<Subject> m_pLivesDisplayEvent;
	};
}
