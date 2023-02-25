#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>


namespace dae
{
	class Texture2D;
	class Font;

	class Component
	{
	public:
		enum class ComponentType
		{
			none,
			transform,
			texture,
			text
		};
		ComponentType m_type{ ComponentType::none };

		Component() = default;
		virtual ~Component() {};

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		ComponentType GetType() const { return m_type; };
	private:
	};

	class Transform final : public Component
	{
	public:
		Transform(float x = 0.f, float y = 0.f, float z = 0.f) : m_position{ x, y, z }
		{
			Component::m_type = ComponentType::transform;
		};

		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z);
	private:
		glm::vec3 m_position;
	};

	class TextureComponent final : public Component
	{
	public:
		TextureComponent() 
		{
			Component::m_type = ComponentType::texture;
		};

		void SetTexture(const std::string& filename);
		std::shared_ptr<Texture2D> GetTexturePtr() const { return m_texture; };
	private:
		std::shared_ptr<Texture2D> m_texture{ nullptr };
	};

	class TextComponent final : public Component
	{
	public:
		void Update();
		void Render() const;

		void SetText(const std::string& text);
		void SetPosition(float x, float y);

		TextComponent(const std::string& text, std::shared_ptr<Font> font);
		~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_needsUpdate;
		std::string m_text;
		Transform m_transform{};
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
	};
}