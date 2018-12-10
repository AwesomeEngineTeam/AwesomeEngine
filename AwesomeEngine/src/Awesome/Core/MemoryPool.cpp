#include <Awesome/Core/MemoryPool.hpp>

namespace aw
{
	MemoryBlock* MemoryPool::CreateBlock(uint64 size)
	{
		MEMORY_LOG("Creating new block of size %lfMB", size * BYTES_TO_MB);

		MemoryBlock newBlock{};
		newBlock.size = size;

		m_blocks.push_back(std::make_unique<MemoryBlock>(newBlock));
		MEMORY_LOG("There is %d blocks", m_blocks.size());
		
		return m_blocks.back().get();
	}

	MemoryBlock* MemoryPool::InsertBlock(uint64 size, MemoryBlock* prev)
	{
		MEMORY_LOG("InsertBlock (size = %lfMB, prev = %p)", size, prev);

		MemoryBlock* newBlock = CreateBlock(size);

		newBlock->offset = prev->offset + prev->size;
		newBlock->next = prev->next;
		newBlock->prev = prev;

		prev->next = newBlock;
		if (newBlock->next != nullptr)
			newBlock->next->prev = prev->next;

		MEMORY_LOG("InsertBlock %p (size = %lfMB, offset = %lfMB) after %p", newBlock, newBlock->size * BYTES_TO_MB, newBlock->offset * BYTES_TO_MB, prev);

		uint64 end = prev->size + prev->offset;
		uint64 endPage = end & ~(1023);

		uint64 start = newBlock->offset;
		uint64 startPage = start & ~(1023);

		MEMORY_LOG("end %d endPage %d", end, endPage);
		MEMORY_LOG("start %d startPage %d", start, startPage);
		MEMORY_LOG("endPage < startPage ? %s", endPage < startPage ? "yes" : "no");

		return newBlock;
	}

	MemoryBlock* MemoryPool::PushBack(uint64 size, uint64 alignment, MemoryBlock* last)
	{
		MEMORY_LOG("PushBack new block of size %lfMB", size * BYTES_TO_MB);

		if (m_blocks.empty() == true)
			return CreateBlock(size);

		if (last->free == true && last->IsAligned(alignment) == true)
		{
			MEMORY_LOG("Updating last block (%p) size (previous size = %lfMB, new size = %lfMB)", last, last->size * BYTES_TO_MB, size * BYTES_TO_MB);
			last->size = size;

			return last;
		}
		else if (alignment != 0 && ((last->offset + last->size) % alignment) != 0)//TODO Make a function to check that
			last = InsertBlock(GetRequiredPadding(last, alignment), last);

		return InsertBlock(size, last);
	}

	MemoryBlock* MemoryPool::SplitBlock(MemoryBlock* block, uint64 sizeToSplit)
	{
		MEMORY_LOG("Splitting block %p (size = %lfMB) into two blocks of size %lfMB and %lfMB", block, block->size * BYTES_TO_MB, sizeToSplit * BYTES_TO_MB, (block->size - sizeToSplit) * BYTES_TO_MB);

		uint64 blockSize = block->size;
		block->size = sizeToSplit;

		InsertBlock(blockSize - sizeToSplit, block);

		return block;
	}

	MemoryBlock* MemoryPool::MergeBlocks(MemoryBlock* left, MemoryBlock* right)
	{
		MEMORY_LOG("Merging blocks %p and %p", left, right);

		MEMORY_LOG("\tBlock %p : size = %lfMB", left, left->size * BYTES_TO_MB);
		MEMORY_LOG("\t          offset = %lfMB", left->offset * BYTES_TO_MB);

		MEMORY_LOG("\tBlock %p : size = %lfMB", right, right->size * BYTES_TO_MB);
		MEMORY_LOG("\t          offset = %lfMB", right->offset * BYTES_TO_MB);

		left->size += right->size;
		left->next = right->next;

		for (int i = 0; i < m_blocks.size(); ++i)
		{
			if (m_blocks[i].get() == right)
			{
				m_blocks.erase(m_blocks.begin() + i);

				break;
			}
		}

		if (left->next != nullptr)
		{
			left->next->prev = left;

			if (left->next->free == true)
				left = MergeBlocks(left, left->next);
		}
		if (left->prev != nullptr)
		{
			if (left->prev->free == true)
				left = MergeBlocks(left->prev, left);
		}

		return left;
	}

	void MemoryPool::FreeBlock(MemoryBlock* block)
	{
		if (block == nullptr)
		{
			LOG_WARNING("MemoryPool::FreeBlock(Block* block) : block is nullptr");
			return;
		}

		block->free = true;

		m_memoryLeft += block->size;

		MemoryBlock* oldBlock = block;
		MemoryBlock* oldPrev = block->prev;
		MemoryBlock* oldNext = block->next;

		MEMORY_LOG("Block %p (prev: %p, next: %p size = %lfMB) freed.", block, block->prev, block->next, block->size * BYTES_TO_MB);
		MEMORY_LOG("Memory left in pool : %lfMB", m_memoryLeft * BYTES_TO_MB);

		if (block->prev != nullptr && block->prev->free == true)
			block = MergeBlocks(block->prev, block);
		if (block->next != nullptr && block->next->free == true)
			block = MergeBlocks(block, block->next);

		m_biggestFreeBlock = 0;

		if (block->next == nullptr)
		{
			for (int i = 0; i < m_blocks.size(); ++i)
			{
				if (m_blocks[i].get() == block)
				{
					uint64 space = m_size - block->offset;

					if (space > m_biggestFreeBlock)
						m_biggestFreeBlock = space;

					if (block->prev != nullptr)
						block->prev->next = nullptr;
					
					MEMORY_LOG("Block to delete found. Deleting %p", m_blocks[i].get());
					m_blocks.erase(m_blocks.begin() + i);
					break;
				}
			}
		}

		for (uint64 i{ 0 }; i < m_blocks.size(); i++)
		{
			if (m_blocks[i]->free == true && m_blocks[i]->size > m_biggestFreeBlock)
				m_biggestFreeBlock = m_blocks[i]->size;
		}
	}

	MemoryBlock* MemoryPool::FindSuitableBlock(uint64 size, uint64 alignment)
	{
		MEMORY_LOG("There is %d blocks", m_blocks.size());

		MemoryBlock* last{ nullptr };

		if (m_blocks.empty() == true)
			return CreateBlock(size);

		for (uint64 i{ 0 }; i < m_blocks.size(); i++)
		{
			if (m_blocks[i]->next == nullptr)
				last = m_blocks[i].get();

			if (m_blocks[i]->free == false || m_blocks[i]->size < size)
				continue;

			if (m_blocks[i]->size == size)
			{
				if (m_blocks[i]->IsAligned(alignment) == false)
					continue;

				return m_blocks[i].get();
			}
			else 
			{
				if (m_blocks[i]->IsAligned(alignment) == true)
					return SplitBlock(m_blocks[i].get(), size);
				else
				{
					uint64 padding = alignment - m_blocks[i]->offset % alignment;

					if (m_blocks[i]->size < size + padding)
						continue;

					MEMORY_LOG("Not aligned (offset %d alignment %d modulo %d)", m_blocks[i]->offset, alignment, m_blocks[i]->offset % alignment);
					MEMORY_LOG("Create padding of %d", padding);

					uint64 end = m_blocks[i].get()->size + m_blocks[i].get()->offset;
					uint64 endPage = end & ~(1023);

					uint64 start = end + padding;
					uint64 startPage = start & ~(1023);

					if (endPage >= startPage)
					{
						padding += alignment;

						if (m_blocks[i]->size < size + padding)
							continue;
					}

					SplitBlock(m_blocks[i].get(), padding);

					MEMORY_LOG("Padding block (%p) size %d offset %d size+offset %d", m_blocks[i].get(), m_blocks[i].get()->size, m_blocks[i].get()->offset, m_blocks[i].get()->size + m_blocks[i].get()->offset);
					
					MEMORY_LOG("Block after padding (%p) is aligned ? %d", m_blocks[i].get()->next, m_blocks[i].get()->next->IsAligned(alignment));
					MEMORY_LOG("Block after padding (%p) size %d offset %d both %d", m_blocks[i].get()->next, m_blocks[i].get()->next->size, m_blocks[i].get()->next->offset, m_blocks[i].get()->next->size + m_blocks[i].get()->next->offset);
					
					return SplitBlock(m_blocks[i].get()->next, size);
				}
			}
		}

		ASSERT(last == nullptr, "last is nullptr but pool not empty");

		MEMORY_LOG("FindSuitableBlock no suitable block (or no block at all)");

		return PushBack(size, alignment, last);
	}

}