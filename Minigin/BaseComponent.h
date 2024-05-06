#pragma once
#include <memory>
#include "Transform.h"

namespace dae
{
	class GameObject;

	class BaseComponent
	{
	public:
		virtual void Awake();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render() const;
		
		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

	protected:
		explicit BaseComponent(GameObject* pParent) :m_pParent{ pParent } {};
		GameObject* m_pParent;

	private:
		
	};
}
