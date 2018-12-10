namespace aw
{
	inline FramebufferInfo::FramebufferInfo()
	{
		m_createInfo.layers = 1;
	}

	inline void FramebufferInfo::SetExtent(const vk::Extent2D& extent)
	{
		m_createInfo.width = extent.width;
		m_createInfo.height = extent.height;
	}

	inline void FramebufferInfo::SetLayers(uint32 layers)
	{
		m_createInfo.layers = layers;
	}

	inline const vk::FramebufferCreateInfo& FramebufferInfo::GetCreateInfo() const
	{
		m_createInfo.attachmentCount = uint32(m_attachments.size());
		m_createInfo.pAttachments = m_attachments.data();

		return m_createInfo;
	}
}