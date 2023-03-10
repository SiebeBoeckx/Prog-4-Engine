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
	go->SetLocalPosition(glm::vec3( 320.f , 240.f, 0.f));
	go->GetComponent<dae::RenderComponent>()->SetTexture("background.tga");
	scene.Add(go);

	//LOGO

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::Transform>();
	go->AddComponent<dae::RenderComponent>();
	go->SetLocalPosition(glm::vec3(320.f, 200.f, 0.f));
	go->GetComponent<dae::RenderComponent>()->SetTexture("logo.tga");
	scene.Add(go);

	//TITLE
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	const auto to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::Transform>();
	to->AddComponent<dae::RenderComponent>();
	to->AddComponent<dae::TextComponent>( "Programming 4 Assignment", font);
	to->SetLocalPosition(glm::vec3(320.f, 150.f, 0.f));
	scene.Add(to);

	//FPS COUNTER
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

	const auto fps = std::make_shared<dae::GameObject>();
	fps->AddComponent<dae::Transform>();
	fps->AddComponent<dae::RenderComponent>();
	fps->AddComponent<dae::TextComponent>("0", font);
	fps->AddComponent<dae::FPSComponent>();
	fps->SetLocalPosition( glm::vec3(40.f, 15.f, 0.f));
	//fps->GetText()->SetColor({ 0, 255, 0 });
	scene.Add(fps);

	//Moving gameObjects (sceneGraph)
	const auto circleCenter = std::make_shared<dae::GameObject>();
	circleCenter->AddComponent<dae::Transform>();
	circleCenter->SetLocalPosition(glm::vec3(320.f, 350.f, 0.f));
	scene.Add(circleCenter);

	const auto digger = std::make_shared<dae::GameObject>();
	digger->AddComponent<dae::Transform>();
	digger->AddComponent<dae::RenderComponent>();
	digger->AddComponent<dae::Encircle>(20.f, 20.f, false);
	digger->GetComponent<dae::RenderComponent>()->SetTexture("SingleDriller.png");
	scene.Add(digger);
	digger->SetParent(circleCenter.get());

	const auto hoggin = std::make_shared<dae::GameObject>();
	hoggin->AddComponent<dae::Transform>();
	hoggin->AddComponent<dae::RenderComponent>();
	hoggin->AddComponent<dae::Encircle>(20.f, 5.f, false);
	hoggin->GetComponent<dae::RenderComponent>()->SetTexture("SingleHogging.png");
	scene.Add(hoggin);
	hoggin->SetParent(digger.get());
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}