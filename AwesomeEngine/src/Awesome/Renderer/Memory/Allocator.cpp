#include <Awesome/Renderer/Memory/Allocator.hpp>
#include <Awesome/Renderer/Memory/AllocatorMemoryPool.hpp>

namespace aw
{
	vk::DeviceSize Allocator::s_bufferImageGranularity = 1;

	Chrono Allocator::s_chrono;
	double Allocator::s_allocTime = 0;
	double Allocator::s_deallocTime = 0;

	std::vector<Allocator::AllocatorMemoryPool> Allocator::s_pools;

	template MemInfo Allocator::Bind<vk::Buffer>(vk::MemoryPropertyFlags, vk::Buffer);
	template MemInfo Allocator::Bind<vk::Image>(vk::MemoryPropertyFlags, vk::Image);

	void Allocator::ClearMemory()
	{
		s_pools.clear();
	}

	void Allocator::InitMemoryPool(vk::DeviceSize sizeInBytes, uint8 memTypeId)
	{
		s_pools.emplace_back();

		vk::MemoryRequirements requirements{
			sizeInBytes,					// VkDeviceSize    size;
			0,								// VkDeviceSize    alignment;
			1u << memTypeId					// uint32_t        memoryTypeBits;
		};

		const vk::PhysicalDeviceMemoryProperties& memProperties = Context::GetDevice().GetMemoryProperties();
		s_pools.back().Init(requirements.size, memTypeId, (memProperties.memoryTypes[memTypeId].propertyFlags & vk::MemoryPropertyFlagBits::eHostVisible) == vk::MemoryPropertyFlagBits::eHostVisible);

		MEMORY_LOG("[Allocator] Memory pool %u allocated. (size = %lfMB)", s_pools.back().Id(), sizeInBytes * BYTES_TO_MB);
	}

	void Allocator::LogDeviceProperties()
	{
		const vk::PhysicalDeviceMemoryProperties& memProperties = Context::GetDevice().GetMemoryProperties();

		for (uint8 i = 0; i < memProperties.memoryHeapCount; ++i)
		{
			MEMORY_LOG("[Allocator] Memory Heap index : %u", i);
			MEMORY_LOG("[Allocator] Memory Heap size : %lfMB", memProperties.memoryHeaps[i].size * BYTES_TO_MB);
			MEMORY_LOG("[Allocator] Memory Heap properties :");

			if (memProperties.memoryHeaps[i].flags & vk::MemoryHeapFlagBits::eDeviceLocal)
				MEMORY_LOG("[Allocator] VK_MEMORY_HEAP_DEVICE_LOCAL_BIT");
			if (memProperties.memoryHeaps[i].flags & vk::MemoryHeapFlagBits::eMultiInstanceKHR)
				MEMORY_LOG("[Allocator] VK_MEMORY_HEAP_MULTI_INSTANCE_BIT_KHR");

			MEMORY_LOG("");
		}

		for (uint8 i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			MEMORY_LOG("[Allocator] Memory Type id : %u", i);
			MEMORY_LOG("[Allocator] Memory Type heap index : %u", memProperties.memoryTypes[i].heapIndex);
			MEMORY_LOG("[Allocator] Memory Type properties :");

			if (memProperties.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal)
				MEMORY_LOG("[Allocator] VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT");
			if (memProperties.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eHostVisible)
				MEMORY_LOG("[Allocator] VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT");
			if (memProperties.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eHostCoherent)
				MEMORY_LOG("[Allocator] VK_MEMORY_PROPERTY_HOST_COHERENT_BIT");
			if (memProperties.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eHostCached)
				MEMORY_LOG("VK_MEMORY_PROPERTY_HOST_CACHED_BIT");
			if (memProperties.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eLazilyAllocated)
				MEMORY_LOG("[Allocator] VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT");

			MEMORY_LOG("");
		}
	}

	uint32 Allocator::GetMemTypeBitsForProperty(vk::MemoryPropertyFlags properties)
	{
		const vk::PhysicalDeviceMemoryProperties& memProperties = Context::GetDevice().GetMemoryProperties();

		uint32 result = 0;
		for (uint32 i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			if ((memProperties.memoryTypes[i].propertyFlags & properties) == properties)
				result += (1u << i);
		}

		return result;
	}

	Allocator::AllocatorMemoryPool& Allocator::FindOrCreateSuitablePool(uint32 availableMemoryTypeBits, vk::MemoryPropertyFlags wantedProperties, uint64 size, uint64 alignment)
	{
		uint32 memoryTypeBits = (availableMemoryTypeBits & GetMemTypeBitsForProperty(wantedProperties));

		MEMORY_LOG("FindOrCreateSuitablePool");

		for (auto& pool : s_pools)
		{
			MEMORY_LOG("pool %d biggest: %d, size: %d, alignment: %d fits? %s", pool.Id(), pool.BiggestFreeBlock(), size, alignment, (pool.BiggestFreeBlock() >= size + alignment) ? "true" : "false");

			if ((pool.MemTypeBits() & memoryTypeBits) == pool.MemTypeBits() && pool.BiggestFreeBlock() >= (size + alignment) * 2) // TODO optimize this (it means remove this *2)
			{
				MEMORY_LOG("suitable pool found (%d)", pool.Id());
				return pool;
			}
		}

		for (uint8 i = 0; i < sizeof(uint32) * 8; ++i)
		{
			if ((memoryTypeBits & (1u << i)) == (1u << i))
			{
				InitMemoryPool(BASE_MEMORY_POOL_ALLOC_SIZE > size ? BASE_MEMORY_POOL_ALLOC_SIZE : size, i);
				return s_pools.back();
			}
		}

		THROW("[Allocator] Could not find or create any appropriate memory type (available memory types: %u, wanted properties : %s)", availableMemoryTypeBits, to_string(wantedProperties).c_str());

		return s_pools.back();//Just to avoid compilation warning/errors, it won't ever reach this point anyway
	}

	template<typename T>
	MemInfo Allocator::Bind(vk::MemoryPropertyFlags memProperties, T object)
	{
		s_chrono.Restart();

		vk::MemoryRequirements memRequirements = Context::GetDevice().GetMemoryRequirements(object);

		AllocatorMemoryPool& pool = FindOrCreateSuitablePool(memRequirements.memoryTypeBits, memProperties, memRequirements.size, memRequirements.alignment);
		MemInfo info{ pool.Bind(memRequirements, object) };

		double time = s_chrono.GetElapsedTime<Chrono::Milliseconds>();
		MEMORY_LOG("[Allocator] Binding object to memory took %lfms", time);
		
		s_allocTime += time;

		return info;
	}

	void Allocator::Unbind(uint64 poolID, MemoryBlock* block)
	{
		s_chrono.Restart();
		for (auto& pool : s_pools)
		{
			if (pool.Id() == poolID)
			{
				pool.Unbind(block);

				double time = s_chrono.GetElapsedTime<Chrono::Milliseconds>();
				MEMORY_LOG("[Allocator] Unbinding block %p took %lfms", block, time);

				s_deallocTime += time;

				return;
			}
		}
	}
}