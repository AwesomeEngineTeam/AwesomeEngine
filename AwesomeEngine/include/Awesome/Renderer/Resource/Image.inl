namespace aw
{
	inline Image::Image(const vk::ImageCreateInfo& createInfo, vk::MemoryPropertyFlagBits properties)
	{
		Create(createInfo, properties);
	}

	//inline Image::Image(Image&& other) noexcept
	//{
	//	m_handle = std::move(other.m_handle);
	//	m_memInfo = std::move(other.m_memInfo);

	//	//other.m_memInfo.poolID = 0;
	//	//other.m_memInfo.block = nullptr;
	//	//other.m_memInfo.memPtr = nullptr;
	//}

	//inline Image::~Image()
	//{
	//	Destroy();
	//}

	inline void Image::Create(vk::ImageType type, vk::Format format, const vk::Extent3D& extent, vk::ImageUsageFlagBits usage, vk::MemoryPropertyFlagBits properties, vk::ImageLayout layout, uint32 sampleCount, uint32 mipLevels, uint32 arrayLayers, bool optimal, bool concurrent)
	{
		vk::ImageCreateInfo createInfo;
		createInfo.imageType = type;
		createInfo.format = format;
		createInfo.extent = extent;
		createInfo.mipLevels = mipLevels;
		createInfo.arrayLayers = arrayLayers;
		createInfo.samples = vk::SampleCountFlagBits(sampleCount);
		createInfo.tiling = vk::ImageTiling(!optimal);
		createInfo.usage = usage;
		createInfo.sharingMode = vk::SharingMode(concurrent);
		createInfo.initialLayout = layout;
		
		Create(createInfo, properties);
	}
	
	inline vk::Image Image::Get() const
	{
		return m_handle.get();
	}

	inline void Image::TransitionLayout(vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::ImageAspectFlags aspect, vk::AccessFlags srcAccessMask, vk::AccessFlags dstAccessMask)
	{
		TransitionLayout(m_handle.get(), oldLayout, newLayout, aspect, srcAccessMask, dstAccessMask);
	}

	inline void Image::TransitionLayout(vk::CommandBuffer commandBuffer, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::ImageAspectFlags aspect, vk::AccessFlags srcAccessMask, vk::AccessFlags dstAccessMask)
	{
		TransitionLayout(commandBuffer, m_handle.get(), oldLayout, newLayout, aspect, srcAccessMask, dstAccessMask);
	}

	//inline Image& Image::operator=(Image&& other) noexcept
	//{
	//	Destroy();

	//	//m_handle.reset(other.m_handle.release());

	//	m_handle = std::move(other.m_handle);
	//	m_memInfo = std::move(other.m_memInfo);
	//	//m_memInfo = other.m_memInfo;

	//	//other.m_memInfo.poolID = 0;
	//	//other.m_memInfo.id = 0;
	//	//other.m_memInfo.memPtr = nullptr;

	//	return *this;
	//}
}