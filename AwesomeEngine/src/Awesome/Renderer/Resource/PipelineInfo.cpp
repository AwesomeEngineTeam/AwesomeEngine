#include <Awesome/Renderer/Pipeline/PipelineInfo.hpp>

namespace aw
{
	PipelineInfo::PipelineInfo()
	{
		m_inputAssemblyState.topology = vk::PrimitiveTopology::eTriangleList;

		m_rasterizerState.lineWidth = 1.0f;
		m_rasterizerState.cullMode = vk::CullModeFlagBits::eBack;
		m_rasterizerState.frontFace = vk::FrontFace::eCounterClockwise;
		m_rasterizerState.polygonMode = vk::PolygonMode::eFill;

		m_multisampleState.sampleShadingEnable = VK_FALSE;// VK_TRUE;
		m_multisampleState.rasterizationSamples = vk::SampleCountFlagBits::e1;
		m_multisampleState.minSampleShading = 1.0f;

		m_depthStencilState.minDepthBounds = 0.0f; // Optional
		m_depthStencilState.maxDepthBounds = 1.0f; // Optional
		m_depthStencilState.depthCompareOp = vk::CompareOp::eLess;
		m_depthStencilState.depthTestEnable = VK_TRUE;
		m_depthStencilState.depthWriteEnable = VK_TRUE;

		m_dynamicStates.push_back(vk::DynamicState::eViewport);
		m_dynamicStates.push_back(vk::DynamicState::eScissor);

		m_colorBlendState.logicOpEnable = VK_FALSE; // Optional
		m_colorBlendState.logicOp = vk::LogicOp::eCopy; // Optional
	}
}