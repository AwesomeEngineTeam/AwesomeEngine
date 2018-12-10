#pragma once

#ifndef AWESOME_SDK_SKYBOX_GUARD
#define AWESOME_SDK_SKYBOX_GUARD

#include <Awesome/Renderer/Resource/Model.hpp>

#include <Awesome/SDK/Components/Render/MeshRenderer.hpp>

namespace aw
{
	namespace sdk
	{
		class Skybox : public MeshRenderer
		{
			friend class RenderSystem;

		protected:
			SampledTextureHandle m_texture{};

		public:
			Skybox() = delete;
			inline explicit Skybox(Actor* actor);
			Skybox(Skybox&&) = delete;
			Skybox(const Skybox&) = delete;
			~Skybox() override = default;

			inline Skybox& operator=(Skybox&&) = delete;
			inline Skybox& operator=(const Skybox&) = delete;
		};
	}

	template <>
	class TypeInfo<sdk::Skybox>
	{
	public:
		static const uint32 id = uint32(sdk::ComponentID::Skybox);
	};
}

#include <Awesome/SDK/Components/Render/Skybox.inl>

#endif // GUARD
