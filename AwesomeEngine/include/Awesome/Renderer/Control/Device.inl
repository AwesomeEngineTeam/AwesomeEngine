namespace aw
{
	inline Device::~Device()
	{
		Destroy();
	}

	inline vk::Device Device::Get() const
	{
		return m_handle.get();
	}

	inline uint32 Device::GetGraphicsQueueFamily() const
	{
		return m_queueFamilyIndices.graphicsFamily;
	}

	inline uint32 Device::GetPresentQueueFamily() const
	{
		return m_queueFamilyIndices.presentFamily;
	}

	inline vk::Queue Device::GetGraphicsQueue() const
	{
		return m_graphicsQueue;
	}

	inline vk::Queue Device::GetPresentQueue() const
	{
		return m_presentQueue;
	}

	inline vk::Fence Device::GetFence() const
	{
		return m_fence.get();
	}

	inline const Device::QueueFamilyIndices& Device::GetQueueFamilyIndices() const
	{
		return m_queueFamilyIndices;
	}

	inline const vk::PhysicalDeviceMemoryProperties& Device::GetMemoryProperties() const
	{
		return m_memoryProperties;
	}

	inline const vk::PhysicalDeviceProperties& Device::GetDeviceProperties() const
	{
		return m_deviceProperties;
	}

	inline vk::MemoryRequirements Device::GetMemoryRequirements(vk::Buffer buffer) const
	{
		return m_handle.get().getBufferMemoryRequirements(buffer);
	}

	inline vk::MemoryRequirements Device::GetMemoryRequirements(vk::Image image) const
	{
		return m_handle.get().getImageMemoryRequirements(image);
	}

	inline const vk::PhysicalDevice Device::GetPhysicalDevice() const
	{
		return m_physicalDevice;
	}
}