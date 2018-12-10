/*#include <Awesome/Renderer/Control/RenderTarget.hpp>
#include <Awesome/Renderer/FramebufferInfo.hpp>
#include <Awesome/Renderer/Framegraph/Pass.hpp>

namespace aw
{
	RenderTarget::RenderTarget()
	{
		m_fence = Context::GetDevice().Get().createFenceUnique(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
	}

	void RenderTarget::InitSwapchain(const Device& device, uint32 resX, uint32 resY)
	{
		m_renderSurface.InitSwapchain(device, resX, resY);

		vk::CommandBufferAllocateInfo cmdAllocInfo(
			Context::GetCommandPool(),
			vk::CommandBufferLevel::ePrimary,
			1
		);

		auto tmpCmdBuffers = Context::GetDevice().Get().allocateCommandBuffersUnique(cmdAllocInfo);

		m_commandBuffer = std::move(tmpCmdBuffers[0]);
	}

	void RenderTarget::InitFramebuffers(Pass* renderPass, const std::vector<vk::ImageView>& attachments)
	{
		ASSERT(renderPass == nullptr, "renderPass == nullptr");

		const auto& images = m_renderSurface.GetImages();
		m_framebuffers.resize(images.size());
		for (uint64 i = 0; i < m_framebuffers.size(); i++)
		{
			FramebufferInfo info;
			info.PushAttachment(images[i].GetImageView());

			for (const auto& attachment : attachments)
				info.PushAttachment(attachment);

			info.SetExtent(m_renderSurface.GetExtent());
			info.SetRenderPass(renderPass->GetRenderPass());

			m_framebuffers[i] = Context::GetDevice().Get().createFramebufferUnique(info.GetCreateInfo());
		}

		m_renderPass = renderPass;
	}

	void RenderTarget::Update(vk::Semaphore imageAvailableSemaphore)
	{
		ASSERT(m_renderPass == nullptr, "renderPass == nullptr");

		m_imageIndex = m_renderSurface.AcquireNextImage(imageAvailableSemaphore, m_fence.get());

		vk::CommandBufferBeginInfo beginInfo(
			vk::CommandBufferUsageFlagBits::eSimultaneousUse
		);

		m_commandBuffer->begin(beginInfo);

		if (m_prePassSecondaryCommandBuffers.empty() == false)
			m_commandBuffer->executeCommands(uint32(m_prePassSecondaryCommandBuffers.size()), m_prePassSecondaryCommandBuffers.data());

		{
			 vk::Rect2D renderArea{};
			 renderArea.extent = m_renderSurface.GetExtent();

			m_renderPass->Begin(m_commandBuffer.get(), m_framebuffers[m_imageIndex].get(), renderArea, vk::SubpassContents::eSecondaryCommandBuffers);

			if (m_passSecondaryCommandBuffers.empty() == false)
				m_commandBuffer->executeCommands(uint32(m_passSecondaryCommandBuffers.size()), m_passSecondaryCommandBuffers.data());

			m_renderPass->End(m_commandBuffer.get());
		}

		m_commandBuffer->end();
	}

	bool RenderTarget::Render(vk::Semaphore imageAvailableSemaphore, vk::Semaphore frameFinishedSemaphore)
	{
		vk::PipelineStageFlags waitStages[]{ vk::PipelineStageFlagBits::eTopOfPipe };

		vk::SubmitInfo submitInfo;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffer.get();
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &imageAvailableSemaphore;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &frameFinishedSemaphore;

		WaitFence();
		ResetFence();
		Context::GetDevice().Submit(submitInfo, m_fence.get());

		vk::SwapchainKHR swapchain = m_renderSurface.GetSwapchain();

		vk::PresentInfoKHR presentInfo;
		presentInfo.pImageIndices = &m_imageIndex;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapchain;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &frameFinishedSemaphore;

		return Context::GetDevice().Present(presentInfo);
	}
}*/