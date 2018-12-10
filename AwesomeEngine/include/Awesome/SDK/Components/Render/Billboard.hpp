#pragma once

#ifndef AWESOME_SDK_BILLBOARD_GUARD
#define AWESOME_SDK_BILLBOARD_GUARD

#include <Awesome/SDK/Components/Component.hpp>
#include <Awesome/SDK/Config.hpp>

#include <Awesome/Core/Types.hpp>

namespace aw
{
	namespace sdk
	{
		class Billboard : public Component
		{
			friend class RenderSystem;

		protected:
			uint32 m_meshIndex{ 0 };
			uint32 m_materialIndex{ 0 };

			UniformBufferDynamicBlock m_dynamicBlock{};
			vk::DescriptorSet m_geometryDset{};

			inline Billboard(Actor* actor, uint32 typeId, const std::string& name = defaultName);

		public:
			Billboard() = delete;
			inline explicit Billboard(Actor* actor = nullptr);
			Billboard(Billboard&&) = delete;
			Billboard(const Billboard&) = delete;
			~Billboard() override = default;

			inline uint32 GetMesh() const;
			inline void SetMesh(uint32);

			inline uint32 GetMaterial() const;
			inline void SetMaterial(uint32);

		public:
			inline Billboard& operator=(Billboard&&) = delete;
			inline Billboard& operator=(const Billboard&) = delete;
		};
	}

	template <>
	class TypeInfo<sdk::Billboard>
	{
	public:
		static const uint32 id = uint32(sdk::ComponentID::Billboard);
	};
}

#include <Awesome/SDK/Components/Render/Billboard.inl>

#endif // GUARD
