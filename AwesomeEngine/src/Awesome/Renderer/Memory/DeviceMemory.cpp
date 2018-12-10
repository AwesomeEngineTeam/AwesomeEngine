#include <Awesome/Renderer/Memory/DeviceMemory.hpp>
#include <Awesome/Renderer/Control/Context.hpp>

#include <Awesome/Core/Logger/Logger.hpp>

namespace aw
{
	void DeviceMemory::Allocate(uint64 size, uint8 memTypeId)
	{
		//CCHECK_ALREADY_CREATED;

		vk::MemoryAllocateInfo allocInfo{
			size,									// VkDeviceSize       allocationSize;
			memTypeId								// uint32_t           memoryTypeIndex; 
		};

		m_handle = Context::GetDevice().Get().allocateMemoryUnique(allocInfo);
		
		//CCHECK_RESULT_CREATED;
		//CLOG_CREATED;
	}

	inline void DeviceMemory::Free()
	{
		m_handle.reset();
	}

	void DeviceMemory::Map(uint64 offset, uint64 size, void** data)
	{
		//CHECK_HANDLE(CLASS " cannot be mapped (handle is null)");

		*data = Context::GetDevice().Get().mapMemory(m_handle.get(), offset, size);

		//ASSERT(result != VK_SUCCESS, CLASS " (%s, %p) cannot map memory of size %llu with offset %llu (error : %s)", VAR_TO_C_STR(m_handle), m_handle, size, offset, VkResultToString(result));
	}

	void DeviceMemory::Unmap()
	{
		//CHECK_HANDLE(CLASS " cannot be unmapped (handle is null)");

		Context::GetDevice().Get().unmapMemory(m_handle.get());
	}

	void DeviceMemory::Flush(uint64 offset, uint64 size)
	{
		//CHECK_HANDLE(CLASS " cannot be flushed (handle is null)");

		vk::MappedMemoryRange flushRange{
			m_handle.get(),								// VkDeviceMemory     memory;
			offset,									// VkDeviceSize       offset;
			size									// VkDeviceSize       size;		
		};

		Context::GetDevice().Get().flushMappedMemoryRanges(1, &flushRange);

		//ASSERT(result != VK_SUCCESS, CLASS " (%s, %p) cannot flush memory with a range of size %llu with offset %llu", VAR_TO_C_STR(m_handle), m_handle, size, offset);
	}

	uint32 DeviceMemory::FindMemoryType(uint32 typeFilter, vk::MemoryPropertyFlagBits properties)
	{
		vk::PhysicalDeviceMemoryProperties memProperties{ Context::GetDevice().GetMemoryProperties() };

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		THROW("No suitable memory found");

		return 0;
	}
}

#undef CLASS