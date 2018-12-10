#include <Awesome/Renderer/RenderQueue.hpp>
#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Resource/Mesh.hpp>

namespace aw
{
	void RenderQueue::AddModel(ShaderHandle shader, MaterialHandle material, const Mesh* mesh, uint32 dynOffset, vk::DescriptorSet geoSet)
	{
		m_models.emplace_back(DrawInfo(shader, material, mesh, dynOffset, geoSet));
	}

	void RenderQueue::Clear()
	{
		m_models.clear();
	}

	void RenderQueue::ResizeCommandBuffers(uint32 size)
	{
		if (m_commandBuffers.size() == size)
			return;

		m_commandBuffers.clear();
		m_commandBuffers = Context::GetDevice().Get().allocateCommandBuffersUnique(
			vk::CommandBufferAllocateInfo(
				Context::GetCommandPool(),
				vk::CommandBufferLevel::eSecondary,
				size
			)
		);
	}

	void RenderQueue::DrawSorted(vk::CommandBuffer commandBuffer)
	{
		for (uint64 i = 0; i < m_models.size(); i++)
		{
			m_models[i].shader.Bind(commandBuffer);

			if (!m_models[i].material.descriptorSet)
			{
				Material::BindDescriptorSet(
					commandBuffer,
					m_models[i].shader.pipelineLayout,
					{ m_models[i].geoSet },
					m_models[i].dynOffset
				);
			}
			else
			{
				Material::BindDescriptorSet(
					commandBuffer,
					m_models[i].shader.pipelineLayout,
					{ m_models[i].geoSet, m_models[i].material.descriptorSet },
					m_models[i].dynOffset
				);
			}

			m_models[i].mesh->Draw(commandBuffer);
		}
	}
}