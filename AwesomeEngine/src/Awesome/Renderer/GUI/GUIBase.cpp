#include <Awesome/Renderer/GUI/GUIBase.hpp>

#include <Awesome/Renderer/GUI/GUIManager.hpp>

namespace aw
{
	GUIBase::GUIBase()
	{
		m_buffer.Create(
			vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
			1024 * 256
		);

	}
}