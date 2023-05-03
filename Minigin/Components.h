#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "GameObject.h"
#include "imgui_plot.h"

#include "Observer.h"
#include "Subject.h"

namespace dae
{
	class Texture2D;
	class Font;
	//class GameObject;

	class Component
	{
	public:
		//Component() = default;
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;


		virtual void BeginPlay() {};
		virtual void Update([[maybe_unused]] float deltaT) {};
		virtual void Render() const {};
		virtual void RenderUI() const {};
		virtual void SetPosition([[maybe_unused]] float x,[[maybe_unused]] float y,[[maybe_unused]] float z) {};
		virtual void SetPosition([[maybe_unused]] glm::vec3 pos) {};
	protected:
		explicit Component(GameObject* pOwner) : m_pOwner(pOwner) {};
		GameObject* GetOwner() const { return m_pOwner; };
	private:
		GameObject* m_pOwner{nullptr};
	};

	class Transform final : public Component
	{
	public:
		Transform(GameObject* pOwner, float x = 0.f, float y = 0.f, float z = 0.f)
		:Component(pOwner)
		,m_position{ x, y, z }
		,m_startPosition{ x, y, z }
		{
			
		}

		const glm::vec3& GetPosition() const { return m_position; }
		const glm::vec3& GetStartPosition() const { return m_startPosition; }
		void SetPosition(float x, float y, float z);
		void SetPosition(glm::vec3 position);
	private:
		glm::vec3 m_position;
		glm::vec3 m_startPosition;
	};

	class RenderComponent final : public Component
	{
	public:
		RenderComponent(GameObject* pOwner)
		:Component(pOwner)
		{};

		void Update([[maybe_unused]] float deltaT) override;
		void Render() const override;
		void SetTexture(const std::string& filename);
		void SetTexture(const std::shared_ptr<Texture2D>& texture);
		std::shared_ptr<Texture2D> GetTexturePtr() const { return m_texture; };
	private:
		std::shared_ptr<Texture2D> m_texture{ nullptr };
		Transform* m_pOwnerTransform{ nullptr };
	};

	class TextComponent final : public Component
	{
	public:
		TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font);

		void Update([[maybe_unused]] float deltaT) override;

		void SetText(const std::string& text);
		//void SetColor(Uint8 r, Uint8 g, Uint8 b);
	private:
		bool m_needsUpdate; //Only update at initialize or if text changes
		std::string m_text;
		std::shared_ptr<Font> m_font{ nullptr };
		//SDL_Color m_color;
	};

	class FPSComponent final : public Component
	{
	public:
		FPSComponent(GameObject* pOwner)
		:Component(pOwner)
		{};

		void Update(float deltaT) override;
		int GetNrOfFrames() const { return m_NrOfFrames; };

	private:
		int m_NrOfFrames{0};
		TextComponent* m_pOwnerText{ nullptr };
	};

	class Encircle final : public Component
	{
	public:
		Encircle(GameObject* pOwner, float distance = 0.f, float secPerRotation = 1.f, bool clockwise = true)
		:Component(pOwner)
		,m_Distance(distance)
		,m_SecPerRotation(secPerRotation)
		,m_isRotatingClockwise(clockwise)
		{};

		void Update(float deltaT) override;
	private:
		glm::vec3 m_Offset{};
		float m_Distance{};
		float m_SecPerRotation{};
		float m_Time{};
		bool m_isRotatingClockwise{};
	};

	class ImGUIComponenent final : public Component
	{
	public:
		ImGUIComponenent(GameObject* pOwner/* ,SDL_Window* window*/)
			:Component(pOwner)
			//,m_window(window)
		{};

		void RenderUI() const override;
	private:
		struct TransformStruct
		{
			float matrix[16] =
			{
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1
			};
		};

		class GameObject3D
		{
		public:
			TransformStruct transform;
			int ID{1};
		};

		class GameObject3DAlt
		{
		public:
			TransformStruct* transform{};
			int ID{1};
		};
		//SDL_Window* m_window;

		std::unique_ptr<ImGui::PlotConfig> m_pIntPlotConfig{ std::make_unique<ImGui::PlotConfig>() };
		void TrashTheCacheInts(ImGui::PlotConfig& plotConfig, int samples) const;

		std::unique_ptr<ImGui::PlotConfig> m_pGameObjectPlotConfig{ std::make_unique<ImGui::PlotConfig>()  };
		void TrashTheCacheGameObjects(ImGui::PlotConfig& plotConfig, int samples) const;

		std::unique_ptr<ImGui::PlotConfig> m_pGameObjectAltPlotConfig{ std::make_unique<ImGui::PlotConfig>() };
		void TrashTheCacheGameObjectAlts(ImGui::PlotConfig& plotConfig, int samples) const;
	};

	class LifeComponent final : public Component, public Observer
	{
	public:
		LifeComponent(GameObject* pOwner, const std::shared_ptr<Subject>& subject, int lives = 3)
			:Component(pOwner)
			,m_pSubject(subject)
			,m_lives(lives)
		{
			m_pOwnerTransform = pOwner->GetComponent<Transform>();
			subject->AddObserver(this);
		}

		int GetLives() const { return m_lives; }
		void Die();

		void OnNotify(const GameObject& gameObject, Event eventType, int optionalValue = 0) override;

		std::shared_ptr<Subject> m_pSubject{};
		
	private:
		void HandleEvents(Event eventType, int optionalValue = 0);
		void Respawn();

		int m_lives;
		Transform* m_pOwnerTransform;
	};

	class PlayerComponent final : public Component
	{
	public:
		PlayerComponent(GameObject* pOwner, int idx)
			:Component(pOwner)
			, m_Idx(idx)
		{
		}

		int GetIndex() const { return m_Idx; }
	private:
		int m_Idx;
	};
}