namespace aw
{
	inline void RenderPassInfo::SubpassDescription::PushInputAttachmentRef(uint32 attachmentIndex, vk::ImageLayout layout)
	{
		m_inputAttachments.push_back({ attachmentIndex, layout });
	}

	inline void RenderPassInfo::SubpassDescription::PushColorAttachmentRef(uint32 attachmentIndex, vk::ImageLayout layout)
	{
		m_colorAttachments.push_back({ attachmentIndex, layout });
	}

	inline void RenderPassInfo::SubpassDescription::PushResolveAttachmentRef(uint32 attachmentIndex, vk::ImageLayout layout)
	{
		m_resolveAttachments.push_back({ attachmentIndex, layout });
	}

	inline void RenderPassInfo::SubpassDescription::PushPreserveAttachmentRef(uint32 attachmentIndex, vk::ImageLayout layout)
	{
		// TODO implement
		//m_preserveAttachments.push_back({ attachmentIndex, static_cast<VkImageLayout>(layout) });
	}

	inline void RenderPassInfo::SubpassDescription::SetDepthStencilAttachmentRef(uint32 attachmentIndex, vk::ImageLayout layout)
	{
		m_depthAttachment = { attachmentIndex, layout };
	}

	inline uint32 RenderPassInfo::PushAttachmentDescription(vk::Format format, vk::ImageLayout finalLayout, vk::ImageLayout initialLayout, vk::AttachmentLoadOp loadOp, vk::AttachmentStoreOp storeOp, vk::AttachmentLoadOp stencilLoadOp, vk::AttachmentStoreOp stencilStoreOp, vk::SampleCountFlagBits samples)
	{
		vk::AttachmentDescription attachment;
		attachment.format = format;
		attachment.samples = samples;
		attachment.loadOp = loadOp;
		attachment.storeOp = storeOp;
		attachment.stencilLoadOp = stencilLoadOp;
		attachment.stencilStoreOp = stencilStoreOp;
		attachment.initialLayout = initialLayout;
		attachment.finalLayout = finalLayout;

		//{
		//	0,													// VkAttachmentDescriptionFlags    flags;
		//	static_cast<VkFormat>(format),						// VkFormat                        format;
		//	static_cast<VkSampleCountFlagBits>(samples),		// VkSampleCountFlagBits           samples;
		//	static_cast<VkAttachmentLoadOp>(loadOp),			// VkAttachmentLoadOp              loadOp;
		//	static_cast<VkAttachmentStoreOp>(storeOp),			// VkAttachmentStoreOp             storeOp;
		//	static_cast<VkAttachmentLoadOp>(stencilLoadOp),		// VkAttachmentLoadOp              stencilLoadOp;
		//	static_cast<VkAttachmentStoreOp>(stencilStoreOp),	// VkAttachmentStoreOp             stencilStoreOp;
		//	static_cast<VkImageLayout>(initialLayout),			// VkImageLayout                   initialLayout;
		//	static_cast<VkImageLayout>(finalLayout),			// VkImageLayout                   finalLayout; 
		//};

		m_attachmentDescriptions.push_back(attachment);

		return uint32(m_attachmentDescriptions.size() - 1);
	}

	inline uint32 RenderPassInfo::PushSubpassDescription(const SubpassDescription& subpassDescription)
	{
		//const bool useResolve = (subpassDescription.m_resolveAttachments.size() == 0);
		const bool useDepthStencil = (subpassDescription.m_depthAttachment.attachment != -1);

		vk::SubpassDescription subpass;
		subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
		subpass.inputAttachmentCount = uint32(subpassDescription.m_inputAttachments.size());
		subpass.pInputAttachments = subpassDescription.m_inputAttachments.data();
		subpass.colorAttachmentCount = uint32(subpassDescription.m_colorAttachments.size());
		subpass.pColorAttachments = subpassDescription.m_colorAttachments.data();
		//subpass.preserveAttachmentCount = uint32(subpassDescription.m_preserveAttachments.size()); // TODO implement
		//subpass.ReserveAttachments = subpassDescription.m_preserveAttachments.data(); // TODO implement
		if (useDepthStencil == true)
			subpass.pDepthStencilAttachment = &subpassDescription.m_depthAttachment;

		//{
		//	0,																	// VkSubpassDescriptionFlags       flags;
		//	VK_PIPELINE_BIND_POINT_RENDERER,									// VkPipelineBindPoint             pipelineBindPoint;
		//	uint32(subpassDescription.m_inputAttachments.size()),				// uint32_t                        inputAttachmentCount;
		//	subpassDescription.m_inputAttachments.data(),						// const VkAttachmentReference*    pInputAttachments;
		//	uint32(subpassDescription.m_colorAttachments.size()),				// uint32_t                        colorAttachmentCount;
		//	subpassDescription.m_colorAttachments.data(),						// const VkAttachmentReference*    pColorAttachments;
		//	//useResolve ? subpassDescription.m_resolveAttachments.data() : nullptr,							// const VkAttachmentReference*    pResolveAttachments;
		//	nullptr,
		//	useDepthStencil ? &subpassDescription.m_depthAttachment : nullptr,	// const VkAttachmentReference*    pDepthStencilAttachment;
		//	//uint32(subpassDescription.m_preserveAttachments.size()),			// uint32_t                        preserveAttachmentCount;
		//	//subpassDescription.m_preserveAttachments.data()					// const uint32_t*                 pPreserveAttachments; 
		//	0, nullptr
		//}; // TODO implement

		m_subpassDescriptions.push_back(subpass);

		return uint32(m_subpassDescriptions.size() - 1);
	}

	inline void RenderPassInfo::PushSubpassDependency(const SubpassDependency& dependency, vk::DependencyFlagBits dependencyFlags)
	{
		vk::SubpassDependency vkDependency;
		vkDependency.srcSubpass = dependency.source.subpass;
		vkDependency.srcStageMask = dependency.source.stageMask;
		vkDependency.srcAccessMask = dependency.source.accessMask;

		vkDependency.dstSubpass = dependency.destination.subpass;
		vkDependency.dstStageMask = dependency.destination.stageMask;
		vkDependency.dstAccessMask = dependency.destination.accessMask;


		//{
		//	dependency.source.subpass,												// uint32_t                srcSubpass;
		//	dependency.destination.subpass,											// uint32_t                dstSubpass;
		//	static_cast<VkPipelineStageFlags>(dependency.source.stageMask),			// VkPipelineStageFlags    srcStageMask;
		//	static_cast<VkPipelineStageFlags>(dependency.destination.stageMask),	// VkPipelineStageFlags    dstStageMask;
		//	static_cast<VkAccessFlags>(dependency.source.accessMask),				// VkAccessFlags           srcAccessMask;
		//	static_cast<VkAccessFlags>(dependency.destination.accessMask),			// VkAccessFlags           dstAccessMask;
		//	static_cast<VkDependencyFlags>(dependencyFlags)							// VkDependencyFlags       dependencyFlags; 
		//};

		m_subpassDependencies.push_back(vkDependency);
	}

	inline vk::RenderPassCreateInfo RenderPassInfo::GetCreateInfo() const
	{
		vk::RenderPassCreateInfo createInfo;
		createInfo.attachmentCount = uint32(m_attachmentDescriptions.size());
		createInfo.pAttachments = m_attachmentDescriptions.data();
		createInfo.subpassCount = uint32(m_subpassDescriptions.size());
		createInfo.pSubpasses = m_subpassDescriptions.data();
		createInfo.dependencyCount = uint32(m_subpassDependencies.size());
		createInfo.pDependencies = m_subpassDependencies.data();

		//{
		//	VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,	// VkStructureType                   sType;
		//	nullptr,									// const void*                       pNext;
		//	0,											// VkRenderPassCreateFlags           flags;
		//	uint32(m_attachmentDescriptions.size()),	// uint32_t                          attachmentCount;
		//	m_attachmentDescriptions.data(),			// const VkAttachmentDescription*    pAttachments;
		//	uint32(m_subpassDescriptions.size()),		// uint32_t                          subpassCount;
		//	m_subpassDescriptions.data(),				// const VkSubpassDescription*       pSubpasses;
		//	uint32(m_subpassDependencies.size()),		// uint32_t                          dependencyCount;
		//	m_subpassDependencies.data()				// const VkSubpassDependency*        pDependencies;
		//};

		return createInfo;
	}
}