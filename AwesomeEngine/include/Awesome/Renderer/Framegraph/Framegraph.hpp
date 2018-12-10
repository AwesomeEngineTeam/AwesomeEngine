#pragma once

#ifndef AWESOME_RENDERER_FRAMEGRAPH_GUARD
#define AWESOME_RENDERER_FRAMEGRAPH_GUARD

#include <Awesome/Renderer/Control/Vulkan.hpp>
#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Resource/SampledTexture.hpp>
#include <Awesome/Renderer/Framegraph/Pass.hpp>

#include <Awesome/Core/Types.hpp>
#include <Awesome/Core/Debug.hpp>

#include <functional>
#include <vector>

namespace aw
{
	class FrameResourcesManager;

	struct AttachmentDescription
	{
		friend class Framegraph;
		friend class FrameResourcesManager;

		vk::Format format{ vk::Format::eR8G8B8A8Unorm };
		vk::ImageType type{ vk::ImageType::e2D };
		vk::ImageAspectFlags aspect{ vk::ImageAspectFlags() };
		vk::Extent3D extent{};
		vk::ImageLayout layout{ vk::ImageLayout::eUndefined };

		bool hasWindowExtent{ true };
		bool isPresentAttachment{ false };

	private:
		vk::ImageUsageFlags m_usage{};
	};

	struct AttachmentProcess
	{
		uint32 attachmentIndex{ ~0u };

		vk::AttachmentLoadOp loadOp{ vk::AttachmentLoadOp::eDontCare };
		vk::AttachmentStoreOp storeOp{ vk::AttachmentStoreOp::eStore };
		vk::ImageLayout initialLayout{ vk::ImageLayout::eUndefined };
		vk::ImageLayout subpassLayout{ vk::ImageLayout::eUndefined };
		vk::ImageLayout finalLayout{ vk::ImageLayout::eUndefined };
		vk::ClearValue clearValue{};
		vk::PipelineColorBlendAttachmentState colorBlendState{
			0,
			vk::BlendFactor::eSrcAlpha,
			vk::BlendFactor::eOneMinusSrcAlpha,
			vk::BlendOp::eAdd,
			vk::BlendFactor::eOne,
			vk::BlendFactor::eZero,
			vk::BlendOp::eAdd,
			vk::ColorComponentFlagBits(15)
		};
	};

	class Framegraph
	{
	public:
		using InitFunc = std::function<void(std::vector<AttachmentProcess>&, std::vector<AttachmentProcess>&, vk::Extent2D& framebufferExtent, vk::Rect2D& renderArea)>;
		using DrawFunc = std::function<void(vk::CommandBuffer)>;

		struct CombinedPass
		{
			vk::UniqueRenderPass renderPass{};
			uint32 framebuffer{ ~0u };
			std::vector<vk::ClearValue> clearValues{};
			DrawFunc beforeFunc{};
			DrawFunc drawFunc{};
			DrawFunc afterFunc{};
			vk::Rect2D renderArea{};
		};

	private:
		struct CombinedFramebuffer
		{
			vk::UniqueFramebuffer framebuffer{};
			vk::RenderPass renderPass{};
			vk::Extent2D extent{};
			std::vector<uint32> textures{};
		};

		std::vector<CombinedPass> m_renderPasses{};
		std::vector<CombinedFramebuffer> m_framebuffers{};
		std::vector<AttachmentProcess> m_outputProcesses{};
		std::vector<AttachmentProcess> m_inputProcesses{};
		//std::vector<AttachmentDescription> m_inputAttachmentDescriptions{};

	public:
		Framegraph() = default;
		Framegraph(const Framegraph&) = default;
		Framegraph(Framegraph&&) = default;
		~Framegraph() = default;

		AWESOME_RENDERER_API Pass AddPass(const FrameResourcesManager&, const InitFunc&, const DrawFunc& beforeFunc, const DrawFunc& drawFunc, const DrawFunc& afterFunc);
		AWESOME_RENDERER_API void Draw(vk::CommandBuffer, vk::SubpassContents = vk::SubpassContents::eInline);

		AWESOME_RENDERER_API void Resize(const FrameResourcesManager&, uint32 width, uint32 height);

	private:
		AWESOME_RENDERER_API void CreateFramebuffer(const FrameResourcesManager&, CombinedFramebuffer&);

	public:
		Framegraph& operator=(const Framegraph&) = default;
		Framegraph& operator=(Framegraph&&) = default;
	};
}

#include <Awesome/Renderer/Framegraph/Framegraph.inl>

#endif //GUARD