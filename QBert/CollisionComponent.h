#pragma once
#include "BaseComponent.h"

namespace dae
{
	class GameObject;
	class LevelComponent;
	class Subject;
	enum class EntityType;

	class CollisionComponent final : public BaseComponent
	{
	public:
		CollisionComponent(GameObject* pParent, LevelComponent* pLevelComponent);
		virtual ~CollisionComponent() = default;

		CollisionComponent(const CollisionComponent& other) = delete;
		CollisionComponent(CollisionComponent&& other) = delete;
		CollisionComponent& operator=(const CollisionComponent& other) = delete;
		CollisionComponent& operator=(CollisionComponent&& other) = delete;

		void CheckCollision(int entityIdx);
		Subject* GetSubject() const;
	private:
		LevelComponent* m_pLevel;
		std::unique_ptr<Subject> m_pDiedEvent;

		void HandleCollisions(EntityType myType, EntityType otherType, dae::GameObject* pOtherObject);
	};
}


