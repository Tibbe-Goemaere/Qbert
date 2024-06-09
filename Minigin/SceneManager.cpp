#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "BaseComponent.h"

void dae::SceneManager::Update()
{
	if (m_currentScene)
	{
		m_currentScene->Update();
	}
}

void dae::SceneManager::LateUpdate()
{
	if (m_currentScene)
	{
		m_currentScene->LateUpdate();
	}
}

void dae::SceneManager::Render()
{
	if (m_currentScene)
	{
		m_currentScene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	if (m_currentScene == nullptr)
	{
		m_currentScene = scene;
	}
	return *scene;
}

void dae::SceneManager::PickScene(const std::string& name)
{
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(),
		[&name](const std::shared_ptr<Scene>& scene)
		{
			return scene->GetSceneName() == name;
		});

	if (it != m_scenes.end())
	{
		m_currentScene = *it;
	}
}

dae::Scene& dae::SceneManager::GetCurrentScene() const
{
	return *m_currentScene;
}
