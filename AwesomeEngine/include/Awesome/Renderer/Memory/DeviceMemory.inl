namespace aw
{
	inline DeviceMemory::DeviceMemory(DeviceMemory&& other)
	{
		if (m_handle)
			Free();

		m_handle.swap(other.m_handle);
	}

	inline DeviceMemory::~DeviceMemory()
	{
		Free();
	}

	inline vk::DeviceMemory DeviceMemory::Get() const
	{
		return m_handle.get();
	}
	
	//inline void DeviceMemory::Allocate(uint32 imageCount, Image* images, MemoryPropertyBit memoryProperty)
	//{
	//	ASSERT_DEBUG(imageCount == 0, "imageCount is 0");
	//	ASSERT(images == nullptr, "images == nullptr");

	//	VkMemoryRequirements requirements = images[0].GetRequirements();
	//	//typedef struct VkMemoryRequirements
	//	//{
	//	//	VkDeviceSize    size;
	//	//	VkDeviceSize    alignment;
	//	//	uint32_t        memoryTypeBits;
	//	//} VkMemoryRequirements;

	//	//LOG_LOG("Requirements %d : size(%d) alignment(%d) memoryTypeBits(%d)", 0, requirements.size, requirements.alignment, requirements.memoryTypeBits);

	//	//uint64_t minUboAlignment = 0x400;
	//	
	//	for (uint32 i = 1; i < imageCount; i++)
	//	{
	//		VkMemoryRequirements tmpRequirements = images[i].GetRequirements();
	//		//LOG_LOG("Requirements %d : size(%d) alignment(%d) memoryTypeBits(%d)", i, tmpRequirements.size, tmpRequirements.alignment, tmpRequirements.memoryTypeBits);

	//		ASSERT(requirements.memoryTypeBits != tmpRequirements.memoryTypeBits, "Requirements not compatibles"); // TODO make compatible for some cases

	//		/*uint64_t dynamicAlignment = requirements.size;
	//		if (minUboAlignment > 0)
	//			dynamicAlignment = (dynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
	//		requirements.size = dynamicAlignment;*/

	//		requirements.size += tmpRequirements.size;
	//	}

	//	//LOG_LOG("Final requirements : size(%d) alignment(%d) memoryTypeBits(%d)", requirements.size, requirements.alignment, requirements.memoryTypeBits);

	//	Allocate(requirements, static_cast<VkMemoryPropertyFlags>(memoryProperty));
	//}

	//inline void DeviceMemory::Allocate(uint32 bufferCount, Buffer* buffers, MemoryPropertyBit memoryProperty)
	//{
	//	ASSERT_DEBUG(bufferCount == 0, "bufferCount is 0");
	//	ASSERT(buffers == nullptr, "buffers == nullptr");

	//	VkMemoryRequirements requirements = buffers[0].GetRequirements();
	//	//typedef struct VkMemoryRequirements
	//	//{
	//	//	VkDeviceSize    size;
	//	//	VkDeviceSize    alignment;
	//	//	uint32_t        memoryTypeBits;
	//	//} VkMemoryRequirements;

	//	//LOG_LOG("Requirements %d : size(%d) alignment(%d) memoryTypeBits(%d)", 0, requirements.size, requirements.alignment, requirements.memoryTypeBits);

	//	for (uint32 i = 1; i < bufferCount; i++)
	//	{
	//		VkMemoryRequirements tmpRequirements = buffers[i].GetRequirements();
	//		//LOG_LOG("Requirements %d : size(%d) alignment(%d) memoryTypeBits(%d)", i, tmpRequirements.size, tmpRequirements.alignment, tmpRequirements.memoryTypeBits);

	//		ASSERT(requirements.memoryTypeBits != tmpRequirements.memoryTypeBits, "Requirements not compatibles"); // TODO make compatible for some cases

	//		requirements.size += tmpRequirements.size;
	//	}

	//	//LOG_LOG("Final requirements : size(%d) alignment(%d) memoryTypeBits(%d)", requirements.size, requirements.alignment, requirements.memoryTypeBits);

	//	Allocate(requirements, static_cast<VkMemoryPropertyFlags>(memoryProperty));
	//}

	inline DeviceMemory& DeviceMemory::operator=(DeviceMemory&& rhs)
	{
		if (m_handle)
			Free();

		m_handle.swap(rhs.m_handle);

		return *this;
	}
}