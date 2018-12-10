#pragma once

#ifndef AWESOME_SDK_MESH_RENDERER_GUARD
#define AWESOME_SDK_MESH_RENDERER_GUARD

#include <Awesome/Renderer/Memory/UniformBufferDynamicBlock.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>

#include <Awesome/Maths/Transform.hpp>
#include <Awesome/Maths/Vector2.hpp>

#include <Awesome/SDK/Components/Component.hpp>

namespace aw
{
	namespace sdk
	{
		class MeshRenderer : public Component
		{
			friend class RenderSystem;

		protected:
			uint32 m_meshIndex{ 0 };
			uint32 m_materialIndex{ 0 };

			UniformBufferDynamicBlock m_dynamicBlock{};
			vk::DescriptorSet m_geometryDset{};

		public:
			bool isLightScatteringEmitter{ false };
			bool isLightScatteringObstacle{ true };

			Transform meshTransform{};

			Vector2 scaleUV{ 1.0f, 1.0f };
			Vector2 offsetUV{ 0.0f, 0.0f };

		protected:
			inline MeshRenderer(Actor* actor, uint32 typeId, const std::string& name = defaultName);

		public:
			MeshRenderer() = delete;
			inline explicit MeshRenderer(Actor* actor = nullptr);
			MeshRenderer(MeshRenderer&&) = delete;
			MeshRenderer(const MeshRenderer&) = delete;
			~MeshRenderer() override = default;

			inline uint32 GetMesh() const;
			inline void SetMesh(uint32);

			inline uint32 GetMaterial() const;
			inline void SetMaterial(uint32);

			inline UniformBufferDynamicBlock& GetDynamicBlock();
			inline const UniformBufferDynamicBlock& GetDynamicBlock() const;

			inline MeshRenderer& operator=(MeshRenderer&&) = delete;
			inline MeshRenderer& operator=(const MeshRenderer&) = delete;

			REFLECT_BEGIN(MeshRenderer)
				REFLECT_MEMBER_INHERITANCE(Component)
				REFLECT_MEMBER(m_meshIndex)
				REFLECT_MEMBER(m_materialIndex)
				REFLECT_MEMBER(isLightScatteringEmitter)
				REFLECT_MEMBER(isLightScatteringObstacle)
				REFLECT_MEMBER(meshTransform)
			REFLECT_END()
		};
	}

	template <>
	class TypeInfo<sdk::MeshRenderer>
	{
	public:
		static const uint32 id = uint32(sdk::ComponentID::MeshRenderer);
	};
}

#include <Awesome/SDK/Components/Render/MeshRenderer.inl>

#endif // GUARD
