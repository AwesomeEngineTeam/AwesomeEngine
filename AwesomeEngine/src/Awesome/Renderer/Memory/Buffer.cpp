#include <Awesome/Renderer/Memory/Buffer.hpp>
#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Memory/Allocator.hpp>

namespace aw
{
	void Buffer::Create(vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, uint64 size, bool concurrent)
	{
		vk::BufferCreateInfo createInfo;
		createInfo.size = size;
		createInfo.usage = usage;
		createInfo.sharingMode = vk::SharingMode(concurrent);

		m_handle = Context::GetDevice().Get().createBufferUnique(createInfo);

		m_memInfo = Allocator::Bind(properties, m_handle.get());
	}

	void Buffer::Destroy()
	{
		m_memInfo.Destroy();
		m_handle.reset();

		/*if (m_memInfo.id != 0)
		{
			Allocator::Unbind(m_memInfo);

			m_memInfo.poolID = 0;
			m_memInfo.id = 0;
			m_memInfo.memPtr = nullptr;
		}*/
	}

	void Buffer::Read(void* data, uint64 size, uint64 offset)
	{
		ASSERT(data == nullptr, "data is nullptr");

		//TODO Check if size < bufferSize
		if (m_memInfo.memPtr != nullptr)
			memcpy(data, static_cast<char*>(m_memInfo.memPtr) + offset, size);
	}

	void Buffer::Write(const void* data, uint64 size, uint64 offset)
	{
		//TODO Check if size < bufferSize
		if (m_memInfo.memPtr != nullptr)
			memcpy(static_cast<char*>(m_memInfo.memPtr) + offset, data, size);
	}
}