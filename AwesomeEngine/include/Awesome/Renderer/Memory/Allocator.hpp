#pragma once

#ifndef AWESOME_RENDERER_ALLOCATOR_GUARD
#define AWESOME_RENDERER_ALLOCATOR_GUARD

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Core/NonCopyable.hpp>

#include <Awesome/Core/Chrono.hpp>

#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Memory/MemInfo.hpp>

#include <memory>
#include <algorithm>

#define BASE_MEMORY_POOL_ALLOC_SIZE (MB_TO_BYTES * 256)

namespace aw
{
	class AWESOME_RENDERER_API Allocator : private NonCopyable
	{
		friend Context;
		friend class GUIDebug;

		static vk::DeviceSize s_bufferImageGranularity;

		static Chrono s_chrono;
		static double s_allocTime;
		static double s_deallocTime;

		class AllocatorMemoryPool;

		static std::vector<AllocatorMemoryPool> s_pools;

	private:
		static void LogDeviceProperties();
		static uint32 GetMemTypeBitsForProperty(vk::MemoryPropertyFlags properties);

		Allocator() = default;
		~Allocator() = default;

		static inline void Init();
		static void InitMemoryPool(vk::DeviceSize sizeInBytes, uint8 memTypeId);

		static AllocatorMemoryPool& FindOrCreateSuitablePool(uint32 availableMemoryTypeBits, vk::MemoryPropertyFlags wantedProperties, uint64 size, uint64 alignment);

		static void ClearMemory();

	public:
		template<typename T>
		static MemInfo Bind(vk::MemoryPropertyFlags memProperties, T object);

		static void Unbind(uint64 poolID, MemoryBlock* block);
	};
}

#include <Awesome/Renderer/Memory/Allocator.inl>

#endif //GUARD