#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update([[maybe_unused]] float dt)
{
	for(auto& scene : m_scenes)
	{
		scene->Update(dt);
	}
}

void dae::SceneManager::FixedUpdate([[maybe_unused]] float dt)
{
	for (auto& scene : m_scenes)
	{
		scene->FixedUpdate(dt);
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

void dae::SceneManager::RenderUI()
{
	for (const auto& scene : m_scenes)
	{
		scene->RenderUI();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
