#include "CollisionComponent.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "Observer.h"
#include <iostream>

dae::CollisionComponent::CollisionComponent(GameObject* pParent, LevelComponent* pLevelComponent)
	:BaseComponent::BaseComponent(pParent)
	,m_pLevel{pLevelComponent}
	,m_pCollisionEvent{ std::make_unique<Subject>() }
{

}

void dae::CollisionComponent::CheckCollision(int entityIdx)
{
	auto pThisEntity = m_pLevel->GetEntityByIdx(entityIdx);
	if (!pThisEntity->isEnabled)
	{
		return;
	}
	auto pEntities = m_pLevel->LookForEntities(entityIdx);
	if (pEntities.size() == 0)
	{
		return;
	}
	if (pThisEntity == nullptr)
	{
		return;
	}

	auto myType = pThisEntity->entityType;
	for (const auto& entity : pEntities)
	{
		if (entity->isEnabled)
		{
			HandleCollisions(myType, entity->entityType, entity->pObject);
		}
	}
}

dae::Subject* dae::CollisionComponent::GetSubject() const
{
	return m_pCollisionEvent.get();
}

void dae::CollisionComponent::HandleCollisions(dae::EntityType myType, dae::EntityType otherType, dae::GameObject* pOtherObject)
{
	
	switch (myType)
	{
	case dae::EntityType::None:
		return;
		break;
	case dae::EntityType::Player:
		if (otherType == EntityType::GreenEnemy)
		{
			m_pCollisionEvent->NotifyObservers(Event::CaughtSlick, m_pParent);
			pOtherObject->MarkForDestroy();
		}
		else if (otherType == EntityType::PurpleEnemy)
		{
			m_pCollisionEvent->NotifyObservers(Event::PlayerDies, m_pParent);
		}
		break;
	case dae::EntityType::GreenEnemy:
		if (otherType == EntityType::Player)
		{
			m_pCollisionEvent->NotifyObservers(Event::CaughtSlick, m_pParent);
			m_pParent->MarkForDestroy();
		}
		break;
	case dae::EntityType::PurpleEnemy:
		if (otherType == EntityType::Player)
		{
			auto pCollision = pOtherObject->GetComponent<CollisionComponent>();
			pCollision->GetSubject()->NotifyObservers(Event::PlayerDies, pOtherObject);
		}
		break;
	default:
		break;
	}
	
}
