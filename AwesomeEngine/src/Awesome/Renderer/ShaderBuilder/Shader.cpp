#include <Awesome/Renderer/ShaderBuilder/Shader.hpp>
#include <Awesome/Renderer/Pipeline/PipelineInfo.hpp>
#include <Awesome/Renderer/Control/Context.hpp>

namespace aw
{
	void Shader::Create(PipelineInfo&& pipelineInfo, const vk::PipelineLayoutCreateInfo& layoutInfo)
	{
		pipelineLayout = Context::GetDevice().Get().createPipelineLayoutUnique(layoutInfo);
		
		pipelineInfo.SetPipelineLayout(pipelineLayout.get());

		pipelineCache = Context::GetDevice().Get().createPipelineCacheUnique(
			vk::PipelineCacheCreateInfo()
		);

		pipeline = Context::GetDevice().Get().createGraphicsPipelineUnique(
			pipelineCache.get(),
			pipelineInfo.GetCreateInfo()
		);
	}

	void Shader::Bind(vk::CommandBuffer commandBuffer) const
	{
		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.get());
	}

	void ShaderHandle::Bind(vk::CommandBuffer commandBuffer) const
	{
		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
	}
}