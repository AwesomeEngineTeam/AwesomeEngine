namespace aw
{
	inline VkGraphicsPipelineCreateInfo PipelineInfo::GetCreateInfo()
	{
		if (m_program == nullptr)
		{
			THROW("m_program is nullptr");
		}

		m_viewportState.viewportCount = static_cast<uint32>(m_viewports.size());
		m_viewportState.pViewports = m_viewports.data();

		m_viewportState.scissorCount = static_cast<uint32>(m_scissors.size());
		m_viewportState.pScissors = m_scissors.data();

		m_colorBlendState.attachmentCount = static_cast<uint32>(m_colorBlendAttachments.size());
		m_colorBlendState.pAttachments = m_colorBlendAttachments.data();

		m_colorBlendState.attachmentCount = static_cast<uint32>(m_colorBlendAttachments.size());
		m_colorBlendState.pAttachments = m_colorBlendAttachments.data();

		m_vertexInputState.vertexBindingDescriptionCount = static_cast<uint32>(m_bindingDescriptions.size());
		m_vertexInputState.pVertexBindingDescriptions = m_bindingDescriptions.data();
		m_vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32>(m_attributeDescriptions.size());
		m_vertexInputState.pVertexAttributeDescriptions = m_attributeDescriptions.data();

		m_dynamicState.dynamicStateCount = static_cast<uint32>(m_dynamicStates.size());
		m_dynamicState.pDynamicStates = m_dynamicStates.data();

		vk::GraphicsPipelineCreateInfo pipelineInfo(
			vk::PipelineCreateFlags(),
			m_program->GetStageCount(),
			m_program->GetStages(),
			&m_vertexInputState,								// const VkPipelineVertexInputStateCreateInfo*      pVertexInputState;
			&m_inputAssemblyState,								// const VkPipelineInputAssemblyStateCreateInfo*    pInputAssemblyState;
			nullptr,											// const VkPipelineTessellationStateCreateInfo*     pTessellationState;
			&m_viewportState,									// const VkPipelineViewportStateCreateInfo*         pViewportState;
			//nullptr,											// const VkPipelineViewportStateCreateInfo*         pViewportState;
			&m_rasterizerState,									// const VkPipelineRasterizationStateCreateInfo*    pRasterizationState;
			&m_multisampleState,								// const VkPipelineMultisampleStateCreateInfo*      pMultisampleState;
			&m_depthStencilState,								// const VkPipelineDepthStencilStateCreateInfo*     pDepthStencilState;
			&m_colorBlendState,									// const VkPipelineColorBlendStateCreateInfo*       pColorBlendState;
			&m_dynamicState,									// const VkPipelineDynamicStateCreateInfo*          pDynamicState;
			m_pipelineLayout,									// VkPipelineLayout                                 layout;
			m_renderPass,										// VkRenderPass                                     renderPass;
			0,													// uint32_t                                         subpass;
			vk::Pipeline(),										// VkPipeline                                       basePipelineHandle;
			-1													// int32_t                                          basePipelineIndex;
		);

		return pipelineInfo;
	}

	inline void PipelineInfo::AddViewport(vk::Viewport viewport)
	{
		m_viewports.emplace_back(std::move(viewport));
	}

	inline void PipelineInfo::AddScissor(vk::Rect2D scissor)
	{
		m_scissors.push_back(std::move(scissor));
	}

	inline void PipelineInfo::SetDepthClampEnable(bool enable)
	{
		m_rasterizerState.depthClampEnable = enable;
	}

	inline void PipelineInfo::SetRasterizerDiscardEnable(bool enable)
	{
		m_rasterizerState.rasterizerDiscardEnable = enable;
	}

	inline void PipelineInfo::SetPolygonMode(vk::PolygonMode mode)
	{
		m_rasterizerState.polygonMode = mode;
	}

	inline void PipelineInfo::SetLineWidth(float width)
	{
		m_rasterizerState.lineWidth = width;
	}

	inline void PipelineInfo::SetCullMode(vk::CullModeFlags mode)
	{
		m_rasterizerState.cullMode = mode;
	}

	inline void PipelineInfo::SetFrontFace(vk::FrontFace face)
	{
		m_rasterizerState.frontFace = face;
	}

	inline void PipelineInfo::SetDepthBiasEnable(bool enable)
	{
		m_rasterizerState.depthBiasEnable = enable;
	}

	inline void PipelineInfo::SetDepthBiasConstantFactor(float factor)
	{
		m_rasterizerState.depthBiasConstantFactor = factor;
	}

	inline void PipelineInfo::SetDepthBiasClamp(float clamp)
	{
		m_rasterizerState.depthBiasClamp = clamp;
	}

	inline void PipelineInfo::SetDepthBiasSlopeFactor(float factor)
	{
		m_rasterizerState.depthBiasSlopeFactor = factor;
	}

	inline void PipelineInfo::SetDepthBoundTestEnable(bool enable)
	{
		m_depthStencilState.depthBoundsTestEnable = enable;
	}

	inline void PipelineInfo::SetMinDepthBounds(float bounds)
	{
		m_depthStencilState.minDepthBounds = bounds;
	}

	inline void PipelineInfo::SetMaxDepthBounds(float bounds)
	{
		m_depthStencilState.maxDepthBounds = bounds;
	}

	inline void PipelineInfo::SetDepthCompareOp(vk::CompareOp op)
	{
		m_depthStencilState.depthCompareOp = op;
	}

	inline void PipelineInfo::SetDepthTestEnable(bool enable)
	{
		m_depthStencilState.depthTestEnable = enable;
	}

	inline void PipelineInfo::SetDepthWriteEnable(bool enable)
	{
		m_depthStencilState.depthWriteEnable = enable;
	}

	inline void PipelineInfo::SetStencilTestEnable(bool enable)
	{
		m_depthStencilState.stencilTestEnable = enable;
	}

	inline void PipelineInfo::AddColorBlendAttachment(bool enable, vk::BlendFactor srcColorBlendFactor, vk::BlendFactor dstColorBlendFactor, vk::BlendOp colorBlendOp, vk::BlendFactor srcAlphaBlendFactor, vk::BlendFactor dstAlphaBlendFactor, vk::BlendOp alphaBlendOp, vk::ColorComponentFlags colorWriteMask)
	{
		vk::PipelineColorBlendAttachmentState colorBlendAttachment{
			enable,					// VkBool32                 blendEnable;
			srcColorBlendFactor,	// VkBlendFactor            srcColorBlendFactor;
			dstColorBlendFactor,	// VkBlendFactor            dstColorBlendFactor;
			colorBlendOp,			// VkBlendOp                colorBlendOp;
			srcAlphaBlendFactor,	// VkBlendFactor            srcAlphaBlendFactor;
			dstAlphaBlendFactor,	// VkBlendFactor            dstAlphaBlendFactor;
			alphaBlendOp,			// VkBlendOp                alphaBlendOp;
			colorWriteMask,			// VkColorComponentFlags    colorWriteMask;
		};

		m_colorBlendAttachments.emplace_back(std::move(colorBlendAttachment));
	}

	inline void PipelineInfo::SetColorBlendAttachments(std::vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachmentStates)
	{
		m_colorBlendAttachments = std::move(colorBlendAttachmentStates);
	}

	inline void PipelineInfo::SetLogicOpEnable(bool enable)
	{
		m_colorBlendState.logicOpEnable = enable;
	}

	inline void PipelineInfo::SetLogicOp(vk::LogicOp op)
	{
		m_colorBlendState.logicOp = op;
	}

	inline void PipelineInfo::SetTopology(vk::PrimitiveTopology topology)
	{
		m_inputAssemblyState.topology = topology;
	}

	inline void PipelineInfo::SetPrimitiveRestartEnable(bool enable)
	{
		m_inputAssemblyState.primitiveRestartEnable = enable;
	}

	inline void PipelineInfo::SetPipelineLayout(vk::PipelineLayout pipelineLayout)
	{
		m_pipelineLayout = pipelineLayout;
	}

	inline void PipelineInfo::SetProgram(const Program* program)
	{
		m_program = program;
	}

	inline void PipelineInfo::SetRenderPass(vk::RenderPass renderPass)
	{
		m_renderPass = renderPass;
	}

	inline void PipelineInfo::SetSubpass(uint32 subpass)
	{
		m_subpass = subpass;
	}

	inline void PipelineInfo::AddVertexLayout(const VertexLayout& vertexLayout)
	{
		uint32 binding = uint32(m_bindingDescriptions.size());

		m_bindingDescriptions.push_back(vertexLayout.CreateBindingDescription(binding));

		const auto& attributeDescriptions = vertexLayout.CreateAttributeDescriptions(binding);
		m_attributeDescriptions.insert(m_attributeDescriptions.end(), attributeDescriptions.begin(), attributeDescriptions.end());
	}
}