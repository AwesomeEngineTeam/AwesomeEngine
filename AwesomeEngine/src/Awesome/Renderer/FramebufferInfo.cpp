#include <Awesome/Renderer/FramebufferInfo.hpp>

namespace aw
{
	void FramebufferInfo::SetRenderPass(vk::RenderPass renderPass)
	{
		m_createInfo.renderPass = renderPass;
	}

	void FramebufferInfo::SetAttachmentCount(uint32 count)
	{
		m_attachments.resize(count);
	}

	void FramebufferInfo::SetAttachmentAt(uint32 index, vk::ImageView imageView)
	{
		if (m_attachments.size() >= index)
			SetAttachmentCount(index + 1);

		m_attachments[index] = imageView;
	}

	void FramebufferInfo::PushAttachment(vk::ImageView imageView)
	{
		m_attachments.push_back(imageView);
	}
}