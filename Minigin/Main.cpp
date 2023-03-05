#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Components.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//BACKGROUND

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::Transform>();
	go->AddComponent<dae::RenderComponent>();
	go->GetComponent<dae::RenderComponent>()->SetTexture("background.tga");
	scene.Add(go);

	//LOGO

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::Transform>();
	go->AddComponent<dae::RenderComponent>();
	go->SetLocalPosition(glm::vec3(216.f, 180.f, 0.f));
	go->GetComponent<dae::RenderComponent>()->SetTexture("logo.tga");
	scene.Add(go);

	//TITLE
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	const auto to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::Transform>();
	to->AddComponent<dae::RenderComponent>();
	to->AddComponent<dae::TextComponent>( "Programming 4 Assignment", font);
	to->SetLocalPosition(glm::vec3(80.f, 20.f, 0.f));
	scene.Add(to);

	//FPS COUNTER
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

	const auto fps = std::make_shared<dae::GameObject>();
	fps->AddComponent<dae::Transform>();
	fps->AddComponent<dae::RenderComponent>();
	fps->AddComponent<dae::TextComponent>("0", font);
	fps->AddComponent<dae::FPSComponent>();
	fps->SetLocalPosition( glm::vec3(10.f, 10.f, 0.f));
	//fps->GetText()->SetColor({ 0, 255, 0 });
	scene.Add(fps);

	//Moving gameObjects (sceneGraph)
	const auto digger = std::make_shared<dae::GameObject>();
	digger->AddComponent<dae::Transform>();
	digger->AddComponent<dae::RenderComponent>();
	digger->AddComponent<dae::Encircle>(100.f, 5.f, true);
	digger->GetComponent<dae::RenderComponent>()->SetTexture("logo.tga");
	scene.Add(digger);
	digger->SetParent(go.get());

	const auto nobbin = std::make_shared<dae::GameObject>();
	nobbin->AddComponent<dae::Transform>();
	nobbin->AddComponent<dae::RenderComponent>();
	nobbin->AddComponent<dae::Encircle>(100.f, 5.f, false);
	nobbin->GetComponent<dae::RenderComponent>()->SetTexture("logo.tga");
	scene.Add(nobbin);
	nobbin->SetParent(digger.get());
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}