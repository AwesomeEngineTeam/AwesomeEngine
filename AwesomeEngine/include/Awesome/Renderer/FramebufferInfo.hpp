#pragma once

#ifndef AWESOME_RENDERER_FRAMEBUFFER_INFO_GUARD
#define AWESOME_RENDERER_FRAMEBUFFER_INFO_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>
#include <Awesome/Renderer/Config.hpp>

#include <Awesome/Core/Types.hpp>

#include <vector>

namespace aw
{
	class FramebufferInfo
	{
		mutable vk::FramebufferCreateInfo m_createInfo{};
		std::vector<vk::ImageView> m_attachments;

	public:
		inline FramebufferInfo();
		FramebufferInfo(const FramebufferInfo&) = default;
		FramebufferInfo(FramebufferInfo&&) = default;
		~FramebufferInfo() = default;

		AWESOME_RENDERER_API void SetRenderPass(vk::RenderPass renderPass);
		AWESOME_RENDERER_API void SetAttachmentCount(uint32 count);
		AWESOME_RENDERER_API void SetAttachmentAt(uint32 index, vk::ImageView imageView);
		AWESOME_RENDERER_API void PushAttachment(vk::ImageView imageView);
		inline void SetExtent(const vk::Extent2D& extent);
		inline void SetLayers(uint32 layers);

		inline const vk::FramebufferCreateInfo& GetCreateInfo() const;

		FramebufferInfo& operator=(const FramebufferInfo&) = default;
		FramebufferInfo& operator=(FramebufferInfo&&) = default;
	};
}

#include <Awesome/Renderer/FramebufferInfo.inl>

#endif //GUARD