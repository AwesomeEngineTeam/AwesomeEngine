#pragma once

#ifndef AWESOME_RENDERER_FRAME_RESOURCES_MANAGER_GUARD
#define AWESOME_RENDERER_FRAME_RESOURCES_MANAGER_GUARD

#include <Awesome/Renderer/Control/Vulkan.hpp>
#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Framegraph/Framegraph.hpp>

#include <Awesome/Core/Types.hpp>

namespace aw
{
	class FrameResourcesManager
	{
		std::vector<SampledTexture> s_textureResources;
		std::vector<AttachmentDescription> s_attachmentDescriptions;

		vk::UniqueFence m_fence{};

	public:
		AWESOME_RENDERER_API FrameResourcesManager();
		FrameResourcesManager(const FrameResourcesManager&) = delete;
		FrameResourcesManager(FrameResourcesManager&&) = default;
		~FrameResourcesManager() { m_fence.reset(); };

		AWESOME_RENDERER_API uint32 AddDescription(AttachmentDescription desc);

		AWESOME_RENDERER_API SampledTextureHandle GetImage(uint32 index) const;
		AWESOME_RENDERER_API const AttachmentDescription& GetDescription(uint32 index) const;

		AWESOME_RENDERER_API void Resize(uint32 width, uint32 height);

	private:
		AWESOME_RENDERER_API static SampledTexture CreateTextureResource(const AttachmentDescription&, vk::Fence);

	public:
		FrameResourcesManager& operator=(const FrameResourcesManager&) = delete;
		FrameResourcesManager& operator=(FrameResourcesManager&&) = default;
	};
}

//#include <Awesome/Renderer/Framegraph/RenderTexture.inl>

#endif //GUARD