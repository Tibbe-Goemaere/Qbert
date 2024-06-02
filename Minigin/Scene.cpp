#include "Scene.h"
#include "GameObject.h"
#include <algorithm>
#include "BaseComponent.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

GameObject* Scene::Add(std::unique_ptr<GameObject> object)
{
	auto rawObject = object.get();
	object->SetScene(this);
	m_objects.emplace_back(std::move(object));
	return rawObject;
}

void Scene::Remove(std::unique_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

std::unique_ptr<GameObject> dae::Scene::GetObject(GameObject* object)
{
	auto it = std::find_if(m_objects.begin(), m_objects.end(), [&](const std::unique_ptr<GameObject>& ptr)
		{ return ptr.get() == object; });

	if (it != m_objects.end())
	{
		auto pObject = std::move(*it);
		m_objects.erase(it);
		return pObject;
	}
	return nullptr;

}

void dae::Scene::SortObjectsRenderPriority()
{
	std::stable_sort(m_objects.begin(), m_objects.end(), [](const std::unique_ptr<GameObject>& first, const std::unique_ptr<GameObject>& second)
	{
		return first->GetRenderLayer() < second->GetRenderLayer();
	});
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		if (object != nullptr)
		{
			object->Update();
		}
	}
}

void Scene::LateUpdate()
{
	for (auto& object : m_objects)
	{
		object->LateUpdate();
	}
	
	m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
		[](const std::unique_ptr<GameObject>& object)
		{
			return object->MarkedForDestroy();
		}),
		m_objects.end());
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

