#pragma once

#ifndef AWESOME_RENDERER_ALLOCATOR_MEMORY_POOL_GUARD
#define AWESOME_RENDERER_ALLOCATOR_MEMORY_POOL_GUARD

#include <Awesome/Core/MemoryPool.hpp>

#include <Awesome/Renderer/Memory/Allocator.hpp>
#include <Awesome/Renderer/Memory/DeviceMemory.hpp>

namespace aw
{
	class Allocator::AllocatorMemoryPool : public MemoryPool
	{
		friend class GUIDebug;

		static uint64 s_count;

		uint64 m_id = 0;
		DeviceMemory m_deviceMemory;
		void* m_memPtr = nullptr;

		uint8 m_memTypeId = 0;

	public:
		AllocatorMemoryPool() = default;
		inline AllocatorMemoryPool(AllocatorMemoryPool&& other) noexcept;
		inline ~AllocatorMemoryPool();

		AWESOME_RENDERER_API void Init(uint64 allocSize, uint8 memTypeId, bool isHostVisible);

		inline uint64 Id() const;
		inline uint16 MemTypeBits() const;

		template<typename T>
		MemInfo Bind(vk::MemoryRequirements memRequirements, T object);
		AWESOME_RENDERER_API void Unbind(MemoryBlock* block);

		inline AllocatorMemoryPool& operator=(AllocatorMemoryPool&& rhs) noexcept;

	private:
		void Bind(MemoryBlock* block, vk::Buffer buffer);
		void Bind(MemoryBlock* block, vk::Image image);
	};
}

#include <Awesome/Renderer/Memory/AllocatorMemoryPool.inl>

#endif //Guard