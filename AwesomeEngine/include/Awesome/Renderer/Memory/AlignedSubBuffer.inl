namespace aw
{
	inline AlignedSubBuffer::AlignedSubBuffer(Buffer* buffer, uint64 offset, uint64 elementSize, uint64 count, MemoryBlock* block, uint64 poolId) :
		buffer(buffer),
		offset(offset),
		elementSize(elementSize),
		count(count),
		alignment(ComputeAlignment(elementSize)),
		block(block),
		poolId(poolId)
	{
	}

	inline AlignedSubBuffer::AlignedSubBuffer(AlignedSubBuffer&& other) noexcept :
		buffer(other.buffer),
		offset(other.offset),
		elementSize(other.elementSize),
		count(other.count),
		block(other.block),
		poolId(other.poolId)
	{
		other.buffer = nullptr;
		other.offset = 0;
		other.elementSize = 0;
		other.count = 0;
		other.block = nullptr;
		other.poolId = 0;
	}

	inline AlignedSubBuffer::AlignedSubBuffer(SubBuffer&& subBuffer) :
		buffer(subBuffer.buffer),
		offset(subBuffer.offset),
		block(subBuffer.block),
		poolId(subBuffer.poolId)
	{
		subBuffer.buffer = nullptr;
		subBuffer.offset = 0;
		subBuffer.size = 0;
		subBuffer.block = nullptr;
		subBuffer.poolId = 0;
	}

	inline void AlignedSubBuffer::Write(uint64 index, const void* data, uint64 size, uint64 offset)
	{
		ASSERT_DEBUG(buffer == nullptr, "buffer is nullptr");
		ASSERT_DEBUG(size > elementSize, "Trying to write out of the element");

		buffer->Write(data, size, index * alignment + offset + this->offset);
	}

	inline void AlignedSubBuffer::Write(uint64 index, const void* data)
	{
		ASSERT_DEBUG(buffer == nullptr, "buffer is nullptr");

		buffer->Write(data, elementSize, index * alignment + offset);
	}

	inline uint64 AlignedSubBuffer::GetOffsetAt(uint64 index) const
	{
		return alignment * index + offset;
	}

	inline uint64 AlignedSubBuffer::ComputeFullSize() const
	{
		return ComputeFullSize(elementSize, count, alignment);
	}

	inline uint64 AlignedSubBuffer::ComputeFullSize(uint64 size, uint64 count)
	{
		return ComputeFullSize(size, count, ComputeAlignment(size));
	}

	inline uint64 AlignedSubBuffer::ComputeFullSize(uint64 size, uint64 count, uint64 alignment)
	{
		return count * (alignment - 1) + size;
	}

	inline SubBuffer AlignedSubBuffer::ConvertToSubBuffer()
	{
		SubBuffer subBuffer{
			buffer,
			ComputeFullSize(),
			offset,
			block,
			poolId
		};

		buffer = nullptr;
		offset = 0;
		elementSize = 0;
		count = 0;
		alignment = 0;
		block = nullptr;
		poolId = 0;

		return subBuffer;
	}

	inline AlignedSubBuffer& AlignedSubBuffer::operator=(AlignedSubBuffer&& other) noexcept
	{
		buffer = other.buffer;
		offset = other.offset;
		elementSize = other.elementSize;
		count = other.count;
		block = other.block;
		poolId = other.poolId;

		other.buffer = nullptr;
		other.offset = 0;
		other.elementSize = 0;
		other.count = 0;
		other.block = nullptr;
		other.poolId = 0;

		return *this;
	}

	inline AlignedSubBuffer& AlignedSubBuffer::operator=(SubBuffer&& other)
	{
		buffer = other.buffer;
		offset = other.offset;
		block = other.block;
		poolId = other.poolId;

		other.buffer = nullptr;
		other.offset = 0;
		other.size = 0;
		other.block = nullptr;
		other.poolId = 0;

		return *this;
	}
}