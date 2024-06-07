#include "CollisionComponent.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "Observer.h"
#include <iostream>

dae::CollisionComponent::CollisionComponent(GameObject* pParent, LevelComponent* pLevelComponent)
	:BaseComponent::BaseComponent(pParent)
	,m_pLevel{pLevelComponent}
	,m_pDiedEvent{ std::make_unique<Subject>() }
{

}

void dae::CollisionComponent::CheckCollision(int entityIdx)
{
	auto pThisEntity = m_pLevel->GetEntityByIdx(entityIdx);
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
		HandleCollisions(myType,entity->entityType,entity->pObject);
	}
}

dae::Subject* dae::CollisionComponent::GetSubject() const
{
	return m_pDiedEvent.get();
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
			std::cout << "i hit green";
			pOtherObject->MarkForDestroy();
		}
		else if (otherType == EntityType::PurpleEnemy)
		{
			std::cout << "i hit purple";
			m_pDiedEvent->NotifyObservers(Event::PlayerDies, m_pParent);
		}
		break;
	case dae::EntityType::GreenEnemy:
		if (otherType == EntityType::Player)
		{
			std::cout << "green hit me";
			//m_pDiedEvent->NotifyObservers(Event::EnemyDies, m_pParent);
			m_pParent->MarkForDestroy();
		}
		break;
	case dae::EntityType::PurpleEnemy:
		if (otherType == EntityType::Player)
		{
			std::cout << "purple hit me";
			m_pDiedEvent->NotifyObservers(Event::PlayerDies, pOtherObject);
		}
		break;
	default:
		break;
	}
	
}
