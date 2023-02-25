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

	dae::Transform* pTransform = new dae::Transform();
	dae::TextureComponent* pTexture = new dae::TextureComponent();

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent(pTransform);
	go->AddComponent(pTexture);
	go->SetTexture("background.tga");
	scene.Add(go);

	pTransform = new dae::Transform();
	pTexture = new dae::TextureComponent();

	go = std::make_shared<dae::GameObject>();
	go->AddComponent(pTransform);
	go->AddComponent(pTexture);
	go->SetPosition(216, 180);
	go->SetTexture("logo.tga");
	scene.Add(go);

	pTransform = new dae::Transform();

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	dae::TextComponent* pText = new dae::TextComponent("Programming 4 Assignment", font);

	auto to = std::make_shared<dae::GameObject>();
	to->AddComponent(pTransform);
	to->AddComponent(pText);
	to->SetPosition(80, 20);
	scene.Add(to);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}