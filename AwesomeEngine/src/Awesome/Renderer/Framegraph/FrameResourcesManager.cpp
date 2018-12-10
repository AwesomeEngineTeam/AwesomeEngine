#include <Awesome/Renderer/Framegraph/FrameResourcesManager.hpp>
#include <Awesome/Renderer/Control/Context.hpp>

namespace aw
{
	FrameResourcesManager::FrameResourcesManager()
	{
		//m_fence = Context::GetDevice().Get().createFenceUnique(vk::FenceCreateInfo());
	}

	uint32 FrameResourcesManager::AddDescription(AttachmentDescription desc)
	{
		if ((desc.aspect & vk::ImageAspectFlagBits::eColor) == vk::ImageAspectFlagBits::eColor)
		{
			if (desc.isPresentAttachment == true)
				desc.m_usage = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc;
			else
				desc.m_usage = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eColorAttachment;
		}
		else if ((desc.aspect & vk::ImageAspectFlagBits::eDepth) == vk::ImageAspectFlagBits::eDepth)
		{
			desc.m_usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
		}
		else
			THROW("Unhandled image aspect (%s)", vk::to_string(desc.aspect).c_str());

		if(!m_fence)
			m_fence = Context::GetDevice().Get().createFenceUnique(vk::FenceCreateInfo());

		s_textureResources.emplace_back(CreateTextureResource(desc, m_fence.get()));

		s_attachmentDescriptions.push_back(std::move(desc));

		return s_attachmentDescriptions.size() - 1;
	}

	SampledTextureHandle FrameResourcesManager::GetImage(uint32 index) const
	{
		return SampledTextureHandle(s_textureResources[index]);
	}

	const AttachmentDescription& FrameResourcesManager::GetDescription(uint32 index) const
	{
		return s_attachmentDescriptions[index];
	}

	void FrameResourcesManager::Resize(uint32 width, uint32 height)
	{
		for (uint32 i{ 0 }; i < s_textureResources.size(); i++)
		{
			if (s_attachmentDescriptions[i].hasWindowExtent == true)
			{
				s_attachmentDescriptions[i].extent.setWidth(width).setHeight(height);
				s_textureResources[i].Destroy();

				if (!m_fence)
					m_fence = Context::GetDevice().Get().createFenceUnique(vk::FenceCreateInfo());

				s_textureResources[i] = CreateTextureResource(s_attachmentDescriptions[i], m_fence.get());
			}
		}
	}

	SampledTexture FrameResourcesManager::CreateTextureResource(const AttachmentDescription& desc, vk::Fence fence)
	{
		SampledTexture res;
		res.Create(
			desc.type,
			desc.format,
			desc.extent,
			desc.m_usage,
			desc.aspect
		);

		Image::TransitionLayout(res.GetImage(), vk::ImageLayout::eUndefined, desc.layout, desc.aspect, vk::AccessFlagBits(0), vk::AccessFlagBits(0));

		//if ((desc.m_usage & vk::ImageUsageFlagBits::eColorAttachment) == vk::ImageUsageFlagBits::eColorAttachment)
		//	Image::TransitionLayout(res.GetImage(), vk::ImageLayout::eUndefined, vk::ImageLayout::eShaderReadOnlyOptimal, desc.aspect);
		//else if ((desc.m_usage & vk::ImageUsageFlagBits::eDepthStencilAttachment) == vk::ImageUsageFlagBits::eDepthStencilAttachment)
		//	Image::TransitionLayout(res.GetImage(), vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, desc.aspect);
		//else
		//{
		//	THROW("Image usage not handled (%s)", vk::to_string(desc.m_usage).c_str());
		//}
		
		return res;
	}
}