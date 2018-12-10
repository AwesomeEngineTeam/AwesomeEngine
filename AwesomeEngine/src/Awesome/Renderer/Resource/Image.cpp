#include <Awesome/Renderer/Resource/Image.hpp>
#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Memory/Allocator.hpp>

namespace aw
{
	void Image::Create(const vk::ImageCreateInfo& createInfo, vk::MemoryPropertyFlagBits properties)
	{
		m_handle = Context::GetDevice().Get().createImageUnique(createInfo);

		m_memInfo = Allocator::Bind(properties, m_handle.get());
	}

	void Image::Destroy()
	{
		m_memInfo.Destroy();
		m_handle.reset();
	}

	void Image::TransitionLayout(vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::ImageAspectFlags aspect, vk::AccessFlags srcAccessMask, vk::AccessFlags dstAccessMask)
	{
		vk::CommandBufferAllocateInfo cmdAllocInfo(
			Context::GetCommandPool(),
			vk::CommandBufferLevel::ePrimary,
			1
		);

		auto commandBuffers = Context::GetDevice().Get().allocateCommandBuffersUnique(cmdAllocInfo);
		auto& commandBuffer = commandBuffers.front().get();

		//vk::ImageSubresourceRange range(aspect, 0, 1, 0, VK_REMAINING_ARRAY_LAYERS);

		//vk::ImageMemoryBarrier barrier{};
		//barrier.srcAccessMask = srcAccessMask;
		//barrier.dstAccessMask = dstAccessMask;
		//barrier.oldLayout = oldLayout;
		//barrier.newLayout = newLayout;
		//barrier.srcQueueFamilyIndex = ~0u;
		//barrier.dstQueueFamilyIndex = ~0u;
		//barrier.image = image;
		//barrier.subresourceRange = range;

		vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
		commandBuffer.begin(beginInfo);

		TransitionLayout(commandBuffer, image, oldLayout, newLayout, aspect, srcAccessMask, dstAccessMask);
		//commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eBottomOfPipe, vk::DependencyFlagBits::eByRegion, {}, {}, barrier);
		commandBuffer.end();

		vk::PipelineStageFlags waitStages[]{ vk::PipelineStageFlagBits::eTopOfPipe };

		vk::SubmitInfo submitInfo;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
		submitInfo.waitSemaphoreCount = 0;
		submitInfo.pWaitSemaphores = nullptr;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.signalSemaphoreCount = 0;
		submitInfo.pSignalSemaphores = nullptr;

		vk::UniqueFence fence = Context::GetDevice().Get().createFenceUnique(vk::FenceCreateInfo());

		Context::GetDevice().Submit(submitInfo, fence.get());
		Context::GetDevice().WaitForFences(fence.get(), true, ~0u);
		Context::GetDevice().Get().resetFences(fence.get());
	}

	void Image::TransitionLayout(vk::CommandBuffer commandBuffer, vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::ImageAspectFlags aspect, vk::AccessFlags srcAccessMask, vk::AccessFlags dstAccessMask)
	{
		vk::ImageSubresourceRange range(aspect, 0, 1, 0, VK_REMAINING_ARRAY_LAYERS);

		vk::ImageMemoryBarrier barrier{};
		barrier.srcAccessMask = srcAccessMask;
		barrier.dstAccessMask = dstAccessMask;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = ~0u;
		barrier.dstQueueFamilyIndex = ~0u;
		barrier.image = image;
		barrier.subresourceRange = range;

		commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eBottomOfPipe, vk::DependencyFlagBits::eByRegion, {}, {}, barrier);
	}
}