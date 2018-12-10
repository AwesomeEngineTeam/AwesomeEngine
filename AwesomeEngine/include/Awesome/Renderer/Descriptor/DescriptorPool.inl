namespace aw
{
	inline DescriptorPool::DescriptorPool(const vk::DescriptorPoolCreateInfo& createInfo)
	{
		Create(createInfo);
	}

	inline DescriptorPool::~DescriptorPool()
	{
		Destroy();
	}

	inline vk::DescriptorPool DescriptorPool::Get() const
	{
		return m_handle.get();
	}
}