namespace aw
{
	inline ImageView::ImageView(ImageView&& other) : m_handle{ other.m_handle }
	{
		other.m_handle = VK_NULL_HANDLE;
	}

	inline ImageView::ImageView(const VkImageViewCreateInfo* createInfo)
	{
		Create(createInfo);
	}

	inline ImageView::ImageView(const Image& image, ImageViewType type, Format format, ComponentMapping swizzle, ImageAspectBit aspectMask)
	{
		Create(image, type, format, swizzle, aspectMask);
	}

	inline ImageView::~ImageView()
	{
		Destroy();
	}

	inline void ImageView::Create(const Image& image, ImageViewType type, Format format, ComponentMapping swizzle, ImageAspectBit aspectMask, uint32 layerCount)
	{
		ImageSubresourceRange subresourceRange{ aspectMask, 0, 1, 0, layerCount };

		VkImageViewCreateInfo createInfo{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,											// VkStructureType          sType
			nullptr,																			// const void              *pNext
			0,																					// VkImageViewCreateFlags   flags
			reinterpret_cast<const VkImage&>(image),											// VkImage                  image
			static_cast<VkImageViewType>(type),													// VkImageViewType          viewType
			static_cast<VkFormat>(format),														// VkFormat                 format
			reinterpret_cast<const VkComponentMapping&>(swizzle),								// VkComponentMapping       components
			reinterpret_cast<const VkImageSubresourceRange&>(subresourceRange)					// VkImageSubresourceRange  subresourceRange;
		};

		Create(&createInfo);
	}
}