#include <Awesome/Renderer/Control/RenderWindow.hpp>
#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Control/Device.hpp>
#include <Awesome/Renderer/FramebufferInfo.hpp>
#include <Awesome/Renderer/RenderPassInfo.hpp>

#include <functional>

namespace aw
{
	RenderWindow::~RenderWindow()
	{
		Context::GetDevice().Get().waitForFences({ m_acquireImageFence.get(), m_presentFence.get() }, true, ~0u);
		Context::GetDevice().WaitIdle();
	}

	void RenderWindow::Init()
	{
		m_window.SetDestroyCallback(std::bind(&RenderWindow::Quit, this));
		m_window.SetCloseCallback(std::bind(&RenderWindow::Quit, this));
		
		vk::Win32SurfaceCreateInfoKHR createSurfaceInfo;
		createSurfaceInfo.hinstance = m_window.GetInstance();
		createSurfaceInfo.hwnd = m_window.GetHandle();

		m_renderSurface.Create(createSurfaceInfo);
		m_renderSurface.SetOnOutOfDateCallback(std::bind(&RenderWindow::OnSurfaceOutOfDate, this));

		vk::PhysicalDevice physicalDevice(m_renderSurface.PickCompatiblePhysicalDevice());
		Context::CreateDevice(physicalDevice, m_renderSurface.GetSurface());

		m_renderSurface.InitSwapchain(m_window.GetWidth(), m_window.GetHeight());

		vk::SemaphoreCreateInfo info{};
		m_imageAvailableSemaphore = Context::GetDevice().Get().createSemaphoreUnique(info);
		m_frameFinishedSemaphore = Context::GetDevice().Get().createSemaphoreUnique(info);
		m_blitFrameSemaphore = Context::GetDevice().Get().createSemaphoreUnique(info);

		vk::CommandBufferAllocateInfo cmdAllocInfo(
			Context::GetCommandPool(),
			vk::CommandBufferLevel::ePrimary,
			m_renderSurface.GetImageCount() + 1
		);

		m_commandBuffers = Context::GetDevice().Get().allocateCommandBuffersUnique(cmdAllocInfo);
		m_blitCommandBuffer = std::move(m_commandBuffers[m_renderSurface.GetImageCount()]);
		m_commandBuffers.resize(m_renderSurface.GetImageCount());

		m_acquireImageFence = Context::GetDevice().Get().createFenceUnique(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
		m_presentFence = Context::GetDevice().Get().createFenceUnique(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
	}

	bool RenderWindow::Draw(vk::Image image) const
	{
		auto blitCommandBuffer = m_blitCommandBuffer.get();
		auto imageAvailableSemaphore = m_imageAvailableSemaphore.get();
		auto frameFinishedSemaphore = m_frameFinishedSemaphore.get();
		auto blitFrameSemaphore = m_blitFrameSemaphore.get();
		auto swapchain = m_renderSurface.GetSwapchain();

		vk::Offset3D blitSize{};
		blitSize.x = m_renderSurface.GetExtent().width;
		blitSize.y = m_renderSurface.GetExtent().height;
		blitSize.z = 1;

		vk::ImageBlit imageBlitRegion{};
		imageBlitRegion.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		imageBlitRegion.srcSubresource.mipLevel = 0;
		imageBlitRegion.srcSubresource.baseArrayLayer = 0;
		imageBlitRegion.srcSubresource.layerCount = 1;
		imageBlitRegion.srcOffsets[1] = blitSize;

		imageBlitRegion.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		imageBlitRegion.dstSubresource.mipLevel = 0;
		imageBlitRegion.dstSubresource.baseArrayLayer = 0;
		imageBlitRegion.dstSubresource.layerCount = 1;
		imageBlitRegion.dstOffsets[1] = blitSize;

		vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
		blitCommandBuffer.begin(beginInfo);
		Image::TransitionLayout(blitCommandBuffer, m_renderSurface.GetImages()[m_currentImageIndex].GetImage(), vk::ImageLayout::ePresentSrcKHR, vk::ImageLayout::eTransferDstOptimal, vk::ImageAspectFlagBits::eColor, vk::AccessFlagBits(0), vk::AccessFlagBits(0));
		blitCommandBuffer.blitImage(
			image, vk::ImageLayout::eTransferSrcOptimal,
			m_renderSurface.GetImages()[m_currentImageIndex].GetImage(), vk::ImageLayout::eTransferDstOptimal,
			imageBlitRegion, vk::Filter::eNearest
		);
		Image::TransitionLayout(blitCommandBuffer, m_renderSurface.GetImages()[m_currentImageIndex].GetImage(), vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::ePresentSrcKHR, vk::ImageAspectFlagBits::eColor, vk::AccessFlagBits(0), vk::AccessFlagBits(0));
		blitCommandBuffer.end();

		vk::PipelineStageFlags waitStages[]{ vk::PipelineStageFlagBits::eTopOfPipe };

		std::array<vk::SubmitInfo, 2> submitInfos;
		submitInfos[0].commandBufferCount = 1;
		submitInfos[0].pCommandBuffers = &m_currentCmdBuffer;
		submitInfos[0].waitSemaphoreCount = 1;
		submitInfos[0].pWaitSemaphores = &imageAvailableSemaphore;
		submitInfos[0].pWaitDstStageMask = waitStages;
		submitInfos[0].signalSemaphoreCount = 1;
		submitInfos[0].pSignalSemaphores = &blitFrameSemaphore;

		submitInfos[1].commandBufferCount = 1;
		submitInfos[1].pCommandBuffers = &blitCommandBuffer;
		submitInfos[1].waitSemaphoreCount = 1;
		submitInfos[1].pWaitSemaphores = &blitFrameSemaphore;
		submitInfos[1].pWaitDstStageMask = waitStages;
		submitInfos[1].signalSemaphoreCount = 1;
		submitInfos[1].pSignalSemaphores = &frameFinishedSemaphore;

		Context::GetDevice().Get().waitForFences(m_presentFence.get(), true, ~0ULL);
		Context::GetDevice().Get().resetFences(m_presentFence.get());
		Context::GetDevice().Submit(submitInfos, m_presentFence.get());

		vk::PresentInfoKHR presentInfo;
		presentInfo.pImageIndices = &m_currentImageIndex;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapchain;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &frameFinishedSemaphore;

		return Context::GetDevice().Present(presentInfo);
	}

	RenderingResources RenderWindow::GetNewRenderingResources()
	{
		Context::GetDevice().Get().waitForFences(m_acquireImageFence.get(), true, ~0ULL);
		Context::GetDevice().Get().resetFences(m_acquireImageFence.get());

		m_currentImageIndex = m_renderSurface.AcquireNextImage(m_imageAvailableSemaphore.get(), m_acquireImageFence.get());
		m_currentCmdBuffer = m_commandBuffers[m_currentImageIndex].get();

		return {
			m_currentCmdBuffer,
			m_currentImageIndex
		};
	}

	void RenderWindow::Resize(uint32 w, uint32 h)
	{
		m_renderSurface.InitSwapchain(w, h);

		if (m_onOutOfDate)
			m_onOutOfDate(w, h);

		m_window.m_shouldResize = false;
	}
}

#undef CLASS