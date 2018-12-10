#pragma once

#ifndef AWESOME_RENDERER_PIPELINE_INFO_GUARD
#define AWESOME_RENDERER_PIPELINE_INFO_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>

#include <Awesome/Core/Types.hpp>

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Pipeline/Program.hpp>
#include <Awesome/Renderer/Pipeline/VertexLayout.hpp>

#include <vector>

namespace aw
{
	class PipelineInfo
	{
		std::vector<vk::VertexInputBindingDescription> m_bindingDescriptions;
		std::vector<vk::VertexInputAttributeDescription> m_attributeDescriptions;
		vk::PipelineVertexInputStateCreateInfo m_vertexInputState{};
		std::vector<vk::Viewport> m_viewports;
		std::vector<vk::Rect2D> m_scissors;
		vk::PipelineViewportStateCreateInfo m_viewportState{};
		vk::PipelineRasterizationStateCreateInfo m_rasterizerState{};
		vk::PipelineMultisampleStateCreateInfo m_multisampleState{};
		vk::PipelineDepthStencilStateCreateInfo m_depthStencilState{};
		std::vector<vk::PipelineColorBlendAttachmentState> m_colorBlendAttachments;
		vk::PipelineColorBlendStateCreateInfo m_colorBlendState{};
		std::vector<vk::DynamicState> m_dynamicStates{};
		vk::PipelineDynamicStateCreateInfo m_dynamicState{};
		vk::PipelineInputAssemblyStateCreateInfo m_inputAssemblyState{};
		vk::RenderPass m_renderPass;
		vk::PipelineLayout m_pipelineLayout;
		uint32 m_subpass{ 0 };
		const Program* m_program{ nullptr };

	public:
		AWESOME_RENDERER_API PipelineInfo();
		PipelineInfo(const PipelineInfo&) = delete;
		PipelineInfo(PipelineInfo&&) = default;
		~PipelineInfo() = default;

		inline VkGraphicsPipelineCreateInfo GetCreateInfo();

		inline void AddViewport(vk::Viewport viewport);
		inline void AddScissor(vk::Rect2D scissor);
		inline void SetDepthClampEnable(bool enable);
		inline void SetRasterizerDiscardEnable(bool enable);
		inline void SetPolygonMode(vk::PolygonMode mode);
		inline void SetLineWidth(float width);
		inline void SetCullMode(vk::CullModeFlags mode);
		inline void SetFrontFace(vk::FrontFace face);
		inline void SetDepthBiasEnable(bool enable);
		inline void SetDepthBiasConstantFactor(float factor);
		inline void SetDepthBiasClamp(float clamp);
		inline void SetDepthBiasSlopeFactor(float factor);
		inline void SetDepthBoundTestEnable(bool enable);
		inline void SetMinDepthBounds(float bounds);
		inline void SetMaxDepthBounds(float bounds);
		inline void SetDepthCompareOp(vk::CompareOp op);
		inline void SetDepthTestEnable(bool enable);
		inline void SetDepthWriteEnable(bool enable);
		inline void SetStencilTestEnable(bool enable);
		inline void AddColorBlendAttachment(bool enable, vk::BlendFactor srcColorBlendFactor = vk::BlendFactor::eSrcAlpha, vk::BlendFactor dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha, vk::BlendOp colorBlendOp = vk::BlendOp::eAdd, vk::BlendFactor srcAlphaBlendFactor = vk::BlendFactor::eOne, vk::BlendFactor dstAlphaBlendFactor = vk::BlendFactor::eZero, vk::BlendOp alphaBlendOp = vk::BlendOp::eAdd, vk::ColorComponentFlags colorWriteMask = vk::ColorComponentFlagBits(15));
		inline void SetColorBlendAttachments(std::vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachmentStates);
		inline void SetLogicOpEnable(bool enable);
		inline void SetLogicOp(vk::LogicOp op);
		inline void SetTopology(vk::PrimitiveTopology topology);
		inline void SetPrimitiveRestartEnable(bool enable);
		inline void SetPipelineLayout(vk::PipelineLayout pipelineLayout);
		inline void SetProgram(const Program* program);
		inline void SetRenderPass(vk::RenderPass renderPass);
		inline void SetSubpass(uint32 subpass);
		inline void AddVertexLayout(const VertexLayout& vertexLayout);

		PipelineInfo& operator=(const PipelineInfo&) = delete;
		PipelineInfo& operator=(PipelineInfo&&) = default;
	};
}

#include <Awesome/Renderer/Pipeline/PipelineInfo.inl>

#endif //GUARD