#pragma once

#ifndef AWESOME_RENDERER_RENDER_QUEUE_GUARD
#define AWESOME_RENDERER_RENDER_QUEUE_GUARD

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>
#include <Awesome/Renderer/Resource/Material.hpp>

#include <Awesome/Core/Types.hpp>

#include <vector>

namespace aw
{
	struct Pass;
	class Mesh;
	//TODO SubRenderQueues system into renderqueue
	class RenderQueue
	{
		struct DrawInfo
		{
			ShaderHandle shader{};
			MaterialHandle material{};
			const Mesh* mesh{ nullptr };
			uint32 dynOffset{ 0 };
			vk::DescriptorSet geoSet{ nullptr };

			DrawInfo(ShaderHandle shader, MaterialHandle material, const Mesh* mesh, uint32 dynOffset, vk::DescriptorSet geoSet) :
				shader{ shader },
				material{ material },
				mesh{ mesh },
				dynOffset{ dynOffset },
				geoSet{ geoSet }
			{
			}
		};
		
		std::vector<vk::UniqueCommandBuffer> m_commandBuffers;
		//std::vector<ModelInstance*> m_models;
		std::vector<DrawInfo> m_models;

	public:
		RenderQueue() = default;
		RenderQueue(const RenderQueue&) = delete;
		RenderQueue(RenderQueue&&) = delete;

		//AWESOME_RENDERER_API void AddModel(ModelInstance*);
		AWESOME_RENDERER_API void AddModel(ShaderHandle, MaterialHandle, const Mesh*, uint32 dynOffset, vk::DescriptorSet geoSet);
		//AWESOME_RENDERER_API void RemoveModel(const ModelInstance*);

		AWESOME_RENDERER_API void Clear();

		AWESOME_RENDERER_API void ResizeCommandBuffers(uint32 size);

		//AWESOME_RENDERER_API std::vector<vk::CommandBuffer> GetSortedSecondaryCommandBuffers(vk::RenderPass renderPass);
		AWESOME_RENDERER_API void DrawSorted(vk::CommandBuffer);
		AWESOME_RENDERER_API void DrawSortedWithMaterial(vk::CommandBuffer, class Material&);

		RenderQueue& operator=(const RenderQueue&) = delete;
		RenderQueue& operator=(RenderQueue&&) = delete;
	};
}

#include <Awesome/Renderer/RenderQueue.inl>

#endif //GUARD