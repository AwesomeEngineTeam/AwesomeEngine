#include <Awesome/Renderer/Resource/Texture.hpp>
#include <Awesome/Renderer/Resource/TexelBuffer.hpp>
#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Memory/Buffer.hpp>

namespace aw
{
	inline static void CmdTransferAndCopyBufferToImage(vk::CommandBuffer commandBuffer, vk::Buffer buffer, vk::Image image, const vk::Extent3D& imageExtent, vk::AccessFlagBits dstAccessMask, vk::ImageLayout newLayout, uint32 srcQueueFamilyIndex, uint32 dstQueueFamilyIndex, uint64 bufferOffset, uint32 bufferRowLength, uint32 bufferImageHeight, vk::ImageSubresourceRange imageSubresourceRange)
	{
		vk::ImageMemoryBarrier imageMemoryBarrier;
		imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits(0);
		imageMemoryBarrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
		imageMemoryBarrier.oldLayout = vk::ImageLayout::eUndefined;
		imageMemoryBarrier.newLayout = vk::ImageLayout::eTransferDstOptimal;
		imageMemoryBarrier.srcQueueFamilyIndex = srcQueueFamilyIndex;
		imageMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange = imageSubresourceRange;

		commandBuffer.pipelineBarrier(
			vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer,
			vk::DependencyFlagBits::eByRegion,
			0, nullptr,
			0, nullptr,
			1, &imageMemoryBarrier
		);
	
		vk::ImageSubresourceLayers imageSubresourceLayers;
		imageSubresourceLayers.aspectMask = imageSubresourceRange.aspectMask;
		imageSubresourceLayers.mipLevel = imageSubresourceRange.baseMipLevel;
		imageSubresourceLayers.baseArrayLayer = imageSubresourceRange.baseArrayLayer;
		imageSubresourceLayers.layerCount = imageSubresourceRange.layerCount;

		vk::BufferImageCopy bufferImageCopyInfo;
		bufferImageCopyInfo.bufferOffset = bufferOffset;
		bufferImageCopyInfo.bufferRowLength = bufferRowLength;
		bufferImageCopyInfo.bufferImageHeight = bufferImageHeight;
		bufferImageCopyInfo.imageSubresource = imageSubresourceLayers;
		bufferImageCopyInfo.imageExtent = imageExtent;
	
		commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, 1, &bufferImageCopyInfo);

		vk::ImageMemoryBarrier imageMemoryBarrier2;
		imageMemoryBarrier2.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		imageMemoryBarrier2.dstAccessMask = dstAccessMask;
		imageMemoryBarrier2.oldLayout = vk::ImageLayout::eTransferDstOptimal;
		imageMemoryBarrier2.newLayout = newLayout;
		imageMemoryBarrier2.srcQueueFamilyIndex = dstQueueFamilyIndex;// srcQueueFamilyIndex; // TODO Why the same index ?
		imageMemoryBarrier2.dstQueueFamilyIndex = dstQueueFamilyIndex;
		imageMemoryBarrier2.image = image;
		imageMemoryBarrier2.subresourceRange = imageSubresourceRange;

		commandBuffer.pipelineBarrier(
			vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader,
			vk::DependencyFlagBits::eByRegion,
			0, nullptr,
			0, nullptr,
			1, &imageMemoryBarrier2
		);
	}

	void Texture::Create(vk::ImageType type, vk::Format format, TexelBuffer texelBuffer, vk::ImageUsageFlags usage, vk::ImageAspectFlags aspect)
	{
		vk::Extent3D extent{ static_cast<uint32>(texelBuffer.width), static_cast<uint32>(texelBuffer.height), 1u };

		Create(type, format, extent, usage, aspect);

		m_texelBuffer = std::move(texelBuffer);

		Flush();
	}

	void Texture::Create(vk::ImageType type, vk::Format format, vk::Extent3D extent, vk::ImageUsageFlags usage, vk::ImageAspectFlags aspect)
	{
		if (type == vk::ImageType::e3D)
		{
			vk::ImageCreateInfo createInfo(
				vk::ImageCreateFlagBits::eCubeCompatible,
				vk::ImageType::e2D,
				format,
				std::move(extent),
				1, // mipLevels
				6, // arrayLayers
				vk::SampleCountFlagBits::e1,
				vk::ImageTiling::eOptimal,
				usage,
				vk::SharingMode::eExclusive,
				0, // queueFamilyIndexCount
				nullptr, // pQueueFamilyIndices // TODO Set Queue families if concurrent mode is enabled
				vk::ImageLayout::eUndefined
			);
			
			m_imageHandle.Create(createInfo, vk::MemoryPropertyFlagBits::eDeviceLocal);

			vk::ImageViewCreateInfo viewInfo;
			viewInfo.image = m_imageHandle.Get();
			viewInfo.viewType = vk::ImageViewType::eCube;
			viewInfo.format = format;
			viewInfo.subresourceRange = vk::ImageSubresourceRange(aspect, 0, 1, 0, 6);

			m_imageViewHandle = Context::GetDevice().Get().createImageViewUnique(viewInfo);
		}
		else
		{
			vk::ImageCreateInfo createInfo(
				vk::ImageCreateFlags(),
				vk::ImageType::e2D,
				format,
				std::move(extent),
				1, // mipLevels
				1, // arrayLayers
				vk::SampleCountFlagBits::e1,
				vk::ImageTiling::eOptimal,
				usage,
				vk::SharingMode::eExclusive,
				0, // queueFamilyIndexCount
				nullptr, // pQueueFamilyIndices // TODO Set Queue families if concurrent mode is enabled
				vk::ImageLayout::eUndefined
			);

			m_imageHandle.Create(createInfo, vk::MemoryPropertyFlagBits::eDeviceLocal);

			vk::ImageViewCreateInfo viewInfo;
			viewInfo.image = m_imageHandle.Get();
			viewInfo.viewType = vk::ImageViewType::e2D;
			viewInfo.format = format;
			viewInfo.subresourceRange = vk::ImageSubresourceRange(aspect, 0, 1, 0, 1);

			m_imageViewHandle = Context::GetDevice().Get().createImageViewUnique(viewInfo);
		}
	}

	void Texture::Destroy()
	{
		m_imageHandle.Destroy();
		m_imageViewHandle.reset();
	}

	void Texture::Flush()
	{
		vk::Extent3D extent{ static_cast<uint32>(m_texelBuffer.width), static_cast<uint32>(m_texelBuffer.height), 1u };

		const uint64 imageSize = m_texelBuffer.texels.size();

		Buffer stagingBuffer;
		stagingBuffer.Create(
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
			uint32(imageSize)
		);

		stagingBuffer.Write(m_texelBuffer.texels.data(), imageSize);

		vk::ImageSubresourceRange imageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, m_texelBuffer.layerCount);

		vk::CommandBufferAllocateInfo tmpCmdBufAllocInfo(Context::GetCommandPool(), vk::CommandBufferLevel::ePrimary, 1);
		vk::UniqueCommandBuffer tempCommandBuffer =
			std::move(Context::GetDevice().Get().allocateCommandBuffersUnique(tmpCmdBufAllocInfo)[0]);

		tempCommandBuffer->begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
		CmdTransferAndCopyBufferToImage(*tempCommandBuffer, stagingBuffer.Get(), m_imageHandle.Get(), extent, vk::AccessFlagBits::eShaderRead, vk::ImageLayout::eShaderReadOnlyOptimal, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED, 0, 0, 0, imageSubresourceRange);
		tempCommandBuffer->end();

		vk::SubmitInfo submitInfo;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &tempCommandBuffer.get();

		Context::GetDevice().Submit(1, &submitInfo, vk::Fence());
		Context::GetDevice().WaitGraphicsIdle();

		m_isCoherent = true;
	}
}