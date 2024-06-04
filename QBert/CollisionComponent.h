#pragma once
#include "BaseComponent.h"

namespace dae
{
	class GameObject;

	class CollisionComponent final : public BaseComponent
	{
	public:
		CollisionComponent(GameObject* pParent);
		virtual ~CollisionComponent() = default;

	private:
		
	};
}


