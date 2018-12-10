#include <Awesome/Renderer/Framegraph/Framegraph.hpp>
#include <Awesome/Renderer/Framegraph/FrameResourcesManager.hpp>
#include <Awesome/Renderer/Framegraph/Pass.hpp>
#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/FramebufferInfo.hpp>
#include <Awesome/Renderer/RenderPassInfo.hpp>

#include <Awesome/Core/Debug.hpp>

namespace aw
{
	Pass Framegraph::AddPass(const FrameResourcesManager& frameResourcesManager, const InitFunc& initFunc, const DrawFunc& beforeFunc, const DrawFunc& drawFunc, const DrawFunc& afterFunc)
	{
		ASSERT(!initFunc, "initFunc is empty");

		uint32 renderPassIndex = static_cast<uint32>(m_renderPasses.size());

		m_renderPasses.resize(renderPassIndex + 1);

		uint32 framebufferIndex = m_framebuffers.size();
		m_framebuffers.resize(framebufferIndex + 1);

		auto& renderPass = m_renderPasses.back();

		std::vector<AttachmentProcess> outputProcesses{};
		std::vector<AttachmentProcess> inputProcesses{};
		std::vector<vk::PipelineColorBlendAttachmentState> colorBlendStates{};
		std::vector<uint32> textureIndices{};

		initFunc(inputProcesses, outputProcesses, m_framebuffers.back().extent, renderPass.renderArea);

		RenderPassInfo renderPassInfo;
		RenderPassInfo::SubpassDescription subpassDescription;

		uint32 colorOutputCount{ 0 };

		for (auto& process : outputProcesses)
		{
			textureIndices.push_back(process.attachmentIndex);
			const auto& desc = frameResourcesManager.GetDescription(process.attachmentIndex);
			m_outputProcesses.push_back(process);

			uint32 descriptionIndex = renderPassInfo.PushAttachmentDescription(
				desc.format,
				process.finalLayout,
				process.initialLayout,
				process.loadOp,
				process.storeOp
			);

			if ((desc.aspect & vk::ImageAspectFlagBits::eColor) == vk::ImageAspectFlagBits::eColor)
			{
				subpassDescription.PushColorAttachmentRef(descriptionIndex, process.subpassLayout);

				colorBlendStates.push_back(process.colorBlendState);
				colorOutputCount++;
			}
			else if ((desc.aspect & vk::ImageAspectFlagBits::eDepth) == vk::ImageAspectFlagBits::eDepth)
			{
				subpassDescription.SetDepthStencilAttachmentRef(descriptionIndex, process.subpassLayout);
			}
			else
				THROW("Unhandled image aspect (%s)", vk::to_string(desc.aspect).c_str());

			renderPass.clearValues.push_back(process.clearValue);
		}

		for (auto& process : inputProcesses)
		{
			textureIndices.push_back(process.attachmentIndex);
			const auto& desc = frameResourcesManager.GetDescription(process.attachmentIndex);
			m_inputProcesses.push_back(process);

			uint32 descriptionIndex = renderPassInfo.PushAttachmentDescription(
				desc.format,
				process.finalLayout,
				process.initialLayout,
				process.loadOp,
				process.storeOp
			);

			subpassDescription.PushInputAttachmentRef(descriptionIndex, process.subpassLayout);

			//if ((desc.aspect & vk::ImageAspectFlagBits::eColor) == vk::ImageAspectFlagBits::eColor)
			//{
			//	subpassDescription.PushColorAttachmentRef(descriptionIndex, process.subpassLayout);

			//	colorBlendStates.push_back(process.colorBlendState);
			//	colorOutputCount++;
			//}
			//else if ((desc.aspect & vk::ImageAspectFlagBits::eDepth) == vk::ImageAspectFlagBits::eDepth)
			//{
			//	subpassDescription.SetDepthStencilAttachmentRef(descriptionIndex, process.subpassLayout);
			//}
			//else
			//	THROW("Unhandled image aspect (%s)", vk::to_string(desc.aspect).c_str());

			//renderPass.clearValues.push_back(process.clearValue);
		}

		RenderPassInfo::SubpassDependency dep{};
		dep.source.stageMask = vk::PipelineStageFlagBits::eTopOfPipe;
		dep.source.accessMask = vk::AccessFlagBits(0);
		dep.source.subpass = ~0u;

		dep.destination.stageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
		dep.destination.accessMask = vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
		dep.destination.subpass = 0;

		renderPassInfo.PushSubpassDependency(dep, vk::DependencyFlagBits::eByRegion);

		dep.source.accessMask = vk::AccessFlagBits(0);
		dep.source.subpass = 0;
		dep.destination.accessMask = vk::AccessFlagBits(0);
		dep.destination.subpass = 0;

		renderPassInfo.PushSubpassDependency(dep, vk::DependencyFlagBits::eByRegion);

		dep.source.accessMask = vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
		dep.source.subpass = 0;
		dep.destination.accessMask = vk::AccessFlagBits(0);
		dep.destination.subpass = ~0u;

		renderPassInfo.PushSubpassDependency(dep, vk::DependencyFlagBits::eByRegion);

		renderPassInfo.PushSubpassDescription(subpassDescription);

		renderPass.renderPass = Context::GetDevice().Get().createRenderPassUnique(renderPassInfo.GetCreateInfo());

		m_framebuffers.back().renderPass = renderPass.renderPass.get();
		m_framebuffers.back().textures = std::move(textureIndices);

		CreateFramebuffer(frameResourcesManager, m_framebuffers.back());

		renderPass.framebuffer = framebufferIndex; 

		renderPass.beforeFunc = beforeFunc;
		renderPass.drawFunc = drawFunc;
		renderPass.afterFunc = afterFunc;

		return {
			renderPass.renderPass.get(),
			m_framebuffers.back().framebuffer.get(),
			std::move(colorBlendStates),
			0,
			colorOutputCount
		};
	}

	void Framegraph::Draw(vk::CommandBuffer commandBuffer, vk::SubpassContents subpassContents)
	{
		for (auto& rp : m_renderPasses)
		{
			if (!rp.drawFunc)
				continue;

			vk::RenderPassBeginInfo rpBeginInfo(
				rp.renderPass.get(), m_framebuffers[rp.framebuffer].framebuffer.get(), rp.renderArea, rp.clearValues.size(), rp.clearValues.data()
			);

			// TODO implement correct renderArea
			//if (rp.renderArea.extent.width > m_framebuffers[rp.framebuffer].extent.width)
				//rpBeginInfo.renderArea.extent.width = m_framebuffers[rp.framebuffer].extent.width;
			//if (rp.renderArea.extent.height > m_framebuffers[rp.framebuffer].extent.height)
				//rpBeginInfo.renderArea.extent.height = m_framebuffers[rp.framebuffer].extent.height;

			//rpBeginInfo.renderArea.extent.width = 1280;
			//rpBeginInfo.renderArea.extent.height = 800;

			if (rp.beforeFunc)
				rp.beforeFunc(commandBuffer);

			commandBuffer.beginRenderPass(rpBeginInfo, subpassContents);

			rp.drawFunc(commandBuffer);

			commandBuffer.endRenderPass();

			if (rp.afterFunc)
				rp.afterFunc(commandBuffer);
		}
	}

	void Framegraph::Resize(const FrameResourcesManager& frameResourcesManager, uint32 width, uint32 height)
	{
		for (auto& rp : m_renderPasses)
			rp.renderArea.extent.setWidth(width).setHeight(height);

		for (auto& framebuffer : m_framebuffers)
		{
			for (uint32 resourceIndex : framebuffer.textures)
			{
				auto& desc = frameResourcesManager.GetDescription(resourceIndex);

				if (desc.hasWindowExtent == true)
				{
					framebuffer.extent = vk::Extent2D(width, height);
					framebuffer.framebuffer.reset();
					CreateFramebuffer(frameResourcesManager, framebuffer);
					break;
				}
			}
		}
	}

	void Framegraph::CreateFramebuffer(const FrameResourcesManager& frameResourcesManager, CombinedFramebuffer& framebuffer)
	{
		FramebufferInfo info;

		for (uint32 resourceIndex : framebuffer.textures)
			info.PushAttachment(frameResourcesManager.GetImage(resourceIndex).GetImageView());

		info.SetExtent(framebuffer.extent);
		info.SetRenderPass(framebuffer.renderPass);

		framebuffer.framebuffer = Context::GetDevice().Get().createFramebufferUnique(info.GetCreateInfo());
	}
}