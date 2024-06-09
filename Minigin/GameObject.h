#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include <type_traits>
#include <cassert>

namespace dae
{
	class BaseComponent;
	class Scene;

	class GameObject final
	{
	public:
		void Update();
		void LateUpdate();
		void Render() const;

		void MarkForDestroy();
		bool MarkedForDestroy() const;

		void SetParent(GameObject* pParent, bool keepWorldPos);
		GameObject* GetParent() const;
		GameObject* GetChildAt(int idx) const;
		int GetChildCount() const;

		const glm::vec3& GetWorldPosition();
		void SetWorldPosition(const glm::vec3& pos);
		const glm::vec3& GetLocalPosition();
		void SetLocalPosition(const glm::vec3& pos);

		void SetScene(Scene* pScene);

		void SetRenderLayer(int layer);
		int GetRenderLayer() const;

		GameObject();
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		//add function for adding component, remove component, get component and check if component is added
		template<typename T, typename... TArgs>
		T* AddComponent(TArgs&&... args);

		template<typename T>
		bool HasComponent();

		template<typename T>
		bool RemoveComponent();

		template<typename T>
		T* GetComponent() const;

	private:
		Transform m_transform{};
		std::vector<std::unique_ptr<BaseComponent>> m_pComponents;
		bool m_markedForDestroy{ false };

		GameObject* m_pParent;
		std::vector<std::unique_ptr<GameObject>> m_pChildren;
		void AddChild(GameObject* pChild);
		std::unique_ptr<GameObject> RemoveChild(GameObject* pChild);
		

		glm::vec3 m_worldPosition{};
		glm::vec3 m_localPosition{};
		bool m_positionIsDirty = false;

		void UpdateWorldPosition();
		void SetPositionDirty();

		Scene* m_pScene;

		int m_renderLayer;
	};

	template<typename T, typename... TArgs>
	inline T* GameObject::AddComponent(TArgs&&... args)
	{
		//assert(HasComponent<T>() && "Already has component of this type");
		//assert(std::is_base_of<BaseComponent,T>::value && "Added component is not a basecomponent");
		//check if we already have a parent

		auto pNewComp = std::make_unique<T>(this, std::forward<TArgs>(args)...);
		pNewComp->Awake();

		auto pComp = pNewComp.get();

		m_pComponents.emplace_back(std::move(pNewComp));
		
		return pComp;
	}

	template<typename T>
	inline bool GameObject::HasComponent()
	{
		auto pComp = this->GetComponent<T>();
		if (pComp == nullptr)
		{
			return false;
		}
		return true;
		
	}

	template<typename T>
	inline bool GameObject::RemoveComponent()
	{
		for (auto i{ m_pComponents.begin() }; i != m_pComponents.end(); ++i)
		{
			if (dynamic_cast<T*>(i->get()))
			{
				m_pComponents.erase(i);
				return true;
			}
		}

		return false;
	}

	template<typename T>
	inline T* GameObject::GetComponent() const
	{
		for (auto i{ m_pComponents.begin() }; i != m_pComponents.end(); ++i)
		{
			T* pComp = dynamic_cast<T*>(i->get());
			if (pComp)
			{
				return pComp;
			}
		}
		return nullptr;
	}
}
