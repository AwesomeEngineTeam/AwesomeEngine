#pragma once

#ifndef AWESOME_SDK_BUFFER_MANAGER_GUARD
#define AWESOME_SDK_BUFFER_MANAGER_GUARD

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Memory/SubBuffer.hpp>
#include <Awesome/Renderer/Memory/BufferMemoryPool.hpp>

#define BASE_BUFFER_POOL_ALLOC_SIZE (MB_TO_BYTES * 1)

namespace aw
{
	class AWESOME_RENDERER_API BufferManager
	{
		friend Context;

		static void InitMemoryPool(uint64 size, BufferType bufferType, bool concurrent = false);
		static inline void Clear();

		static BufferMemoryPool& FindOrCreateSuitablePool(uint64 size, bool aligned, BufferType bufferType, bool concurrent = false);

		static std::vector<std::unique_ptr<BufferMemoryPool>> s_pools;

	public:
		BufferManager() = delete;
		BufferManager(BufferManager&&) = delete;
		BufferManager(const BufferManager&) = delete;
		~BufferManager() = delete;

		static SubBuffer Reserve(uint64 size, bool aligned, BufferType bufferType, bool concurrent = false);
		static void Free(uint64 poolId, MemoryBlock* block);

		BufferManager& operator=(BufferManager&&) = delete;
		BufferManager& operator=(const BufferManager&) = delete;
	};
}

#include <Awesome/Renderer/Memory/BufferManager.inl>

#endif