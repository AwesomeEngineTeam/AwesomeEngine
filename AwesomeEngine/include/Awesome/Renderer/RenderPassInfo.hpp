#pragma once

#ifndef AWESOME_RENDERER_RENDER_PASS_INFO_GUARD
#define AWESOME_RENDERER_RENDER_PASS_INFO_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>
#include <Awesome/Renderer/Config.hpp>

#include <vector>

namespace aw
{
	class RenderPassInfo
	{
		std::vector<vk::AttachmentDescription> m_attachmentDescriptions;
		std::vector<vk::SubpassDescription> m_subpassDescriptions;
		std::vector<vk::SubpassDependency> m_subpassDependencies;

	public:
		class SubpassDescription
		{
			friend RenderPassInfo;

			std::vector<vk::AttachmentReference> m_inputAttachments;
			std::vector<vk::AttachmentReference> m_colorAttachments;
			std::vector<vk::AttachmentReference> m_resolveAttachments;
			vk::AttachmentReference m_depthAttachment{ static_cast<uint32>(-1), vk::ImageLayout::eUndefined };

		public:
			SubpassDescription() = default;
			SubpassDescription(const SubpassDescription&) = default;
			SubpassDescription(SubpassDescription&&) = default;
			~SubpassDescription() = default;

			inline void PushInputAttachmentRef(uint32 attachmentIndex, vk::ImageLayout layout);
			inline void PushColorAttachmentRef(uint32 attachmentIndex, vk::ImageLayout layout);
			inline void PushResolveAttachmentRef(uint32 attachmentIndex, vk::ImageLayout layout);
			inline void PushPreserveAttachmentRef(uint32 attachmentIndex, vk::ImageLayout layout);
			inline void SetDepthStencilAttachmentRef(uint32 attachmentIndex, vk::ImageLayout layout);

			SubpassDescription& operator=(const SubpassDescription&) = default;
			SubpassDescription& operator=(SubpassDescription&&) = default;
		};

		struct SubpassDependency
		{
			struct
			{
				uint32 subpass{ 0 };
				vk::PipelineStageFlagBits stageMask{ vk::PipelineStageFlagBits::eTopOfPipe };
				vk::AccessFlags accessMask;
			} source, destination;
		};

		RenderPassInfo() = default;
		RenderPassInfo(const RenderPassInfo&) = default;
		RenderPassInfo(RenderPassInfo&&) = default;
		~RenderPassInfo() = default;

		inline uint32 PushAttachmentDescription(vk::Format format, vk::ImageLayout finalLayout, vk::ImageLayout initialLayout = vk::ImageLayout::eUndefined, vk::AttachmentLoadOp loadOp = vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp storeOp = vk::AttachmentStoreOp::eStore, vk::AttachmentLoadOp stencilLoadOp = vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp stencilStoreOp = vk::AttachmentStoreOp::eDontCare, vk::SampleCountFlagBits samples = vk::SampleCountFlagBits::e1);
		inline uint32 PushSubpassDescription(const SubpassDescription& subpassDescription);
		inline void PushSubpassDependency(const SubpassDependency&, vk::DependencyFlagBits dependencyFlags);
		
		inline vk::RenderPassCreateInfo GetCreateInfo() const;

		RenderPassInfo& operator=(const RenderPassInfo&) = default;
		RenderPassInfo& operator=(RenderPassInfo&&) = default;
	};
}

#include <Awesome/Renderer/RenderPassInfo.inl>

#endif //GUARD