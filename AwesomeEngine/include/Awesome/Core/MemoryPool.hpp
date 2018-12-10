#pragma once

#ifndef AWESOME_CORE_MEMORY_POOL_GUARD
#define AWESOME_CORE_MEMORY_POOL_GUARD

#include <Awesome/Core/Config.hpp>
#include <Awesome/Core/Types.hpp>
#include <Awesome/Core/Debug.hpp>

#include <Awesome/Renderer/Memory/MemInfo.hpp> // TODO change that (renderer in core wtf)

#include <vector>
#include <memory>

#define MB_TO_BYTES (1000 * 1000)
#define BYTES_TO_MB (1.0 / (1000.0 * 1000.0))

//#define MEMORY_LOG(...) LOG_LOG(__VA_ARGS__)
#define MEMORY_LOG(...)

namespace aw
{
	class MemoryPool
	{
	protected:
		uint64 m_size = 0;
		uint64 m_memoryLeft = 0;
		uint64 m_biggestFreeBlock = 0;

		std::vector<std::unique_ptr<MemoryBlock>> m_blocks;

	public:
		MemoryPool() = default;
		inline MemoryPool(MemoryPool&& other) noexcept;
		virtual ~MemoryPool() = default;

		inline void Init(uint64 size);

		inline uint64 MemoryLeft() const;
		inline uint64 BiggestFreeBlock() const;

		inline MemoryPool& operator=(MemoryPool&& rhs) noexcept;

	protected:
		AWESOME_CORE_API MemoryBlock* CreateBlock(uint64 size);
		AWESOME_CORE_API MemoryBlock* InsertBlock(uint64 size, MemoryBlock* prev);

		inline uint64 GetRequiredPadding(MemoryBlock* prev, uint64 alignment) const;

		AWESOME_CORE_API MemoryBlock* PushBack(uint64 size, uint64 alignment, MemoryBlock* last);
		AWESOME_CORE_API MemoryBlock* SplitBlock(MemoryBlock* block, uint64 sizeToSplit);

		AWESOME_CORE_API MemoryBlock* MergeBlocks(MemoryBlock* left, MemoryBlock* right);
		AWESOME_CORE_API void FreeBlock(MemoryBlock* block);

		AWESOME_CORE_API MemoryBlock* FindSuitableBlock(uint64 size, uint64 alignment);
	};
}

#include <Awesome/Core/MemoryPool.inl>

#endif //Guard