#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class TextObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		//void Add(std::shared_ptr<TextObject> text);
		void Remove(std::shared_ptr<GameObject> object);
		//void Remove(std::shared_ptr<TextObject> text);
		void RemoveAll();

		void Update(float dt);
		void FixedUpdate(float dt);
		void Render() const;
		void RenderUI() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter; 
	};

}
