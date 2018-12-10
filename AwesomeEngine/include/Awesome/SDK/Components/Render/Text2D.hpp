#pragma once

#ifndef AWESOME_SDK_TEXT_2D_GUARD
#define AWESOME_SDK_TEXT_2D_GUARD

#include <Awesome/SDK/Components/Component.hpp>
#include <Awesome/SDK/Config.hpp>

#include <Awesome/Core/Types.hpp>

#include <Awesome/Renderer/Resource/Mesh.hpp>
#include <Awesome/Renderer/Resource/FontAtlas.hpp>
#include <Awesome/Renderer/Memory/UniformBufferDynamicBlock.hpp>

namespace aw
{
	namespace sdk
	{
		class Text2D : public Component
		{
			friend class RenderSystem;

			mutable std::mutex m_mutex;

		protected:
			Mesh m_mesh{};
			FontAtlas* m_fontAtlas{ nullptr };

			uint32 m_meshIndex{ 0 };
			uint32 m_materialIndex{ 0 };

			UniformBufferDynamicBlock m_dynamicBlock{};
			vk::DescriptorSet m_geometryDset{};

			std::string m_content{};

			std::atomic_bool m_updated = false;

			inline Text2D(Actor* actor, uint32 typeId, const std::string& name = defaultName);

		private:
			AWESOME_SDK_API bool UpdateContent();

		public:
			Text2D() = delete;
			inline explicit Text2D(Actor* actor = nullptr);
			Text2D(Text2D&&) = delete;
			Text2D(const Text2D&) = delete;
			~Text2D() override = default;

			inline std::string GetContent() const;
			inline void SetContent(std::string);

		public:
			inline Text2D& operator=(Text2D&&) = delete;
			inline Text2D& operator=(const Text2D&) = delete;

			REFLECT_BEGIN(Text2D)
				REFLECT_MEMBER_INHERITANCE(Component)
				REFLECT_MEMBER(m_content)
			REFLECT_END()
		};
	}

	template <>
	class TypeInfo<sdk::Text2D>
	{
	public:
		static const uint32 id = uint32(sdk::ComponentID::Text2D);
	};
}

#include <Awesome/SDK/Components/Render/Text2D.inl>

#endif // GUARD
