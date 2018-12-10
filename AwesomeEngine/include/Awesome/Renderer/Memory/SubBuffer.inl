namespace aw
{
	inline SubBuffer::SubBuffer(Buffer* buffer, uint64 size, uint64 offset, MemoryBlock* block, uint64 poolId) :
		buffer(buffer),
		size(size),
		offset(offset),
		block(block),
		poolId(poolId)
	{
	}

	inline SubBuffer::SubBuffer(const SubBuffer& other, uint64 size, uint64 offset, MemoryBlock* block, uint64 poolId) :
		buffer(other.buffer),
		size(size),
		offset(other.size + other.offset + offset),
		block(block),
		poolId(poolId)
	{
	}

	inline SubBuffer::SubBuffer(SubBuffer&& other) :
		buffer(other.buffer),
		size(other.size),
		offset(other.offset),
		block(other.block),
		poolId(other.poolId)
	{
		other.Clear();
	}

	inline SubBuffer::~SubBuffer()
	{
		Free();
		Clear();
	}

	inline void SubBuffer::Read(void* data, uint64 size, uint64 offset)
	{
		ASSERT_DEBUG(buffer == nullptr, "buffer is nullptr");

		buffer->Read(data, size, this->offset + offset);
	}

	inline void SubBuffer::Write(const void* data, uint64 size, uint64 offset)
	{
		ASSERT_DEBUG(buffer == nullptr, "buffer is nullptr");
		ASSERT_DEBUG(this->size < size, "Trying to write out of the sub buffer (size: %d, tried: %d)", this->size, size);
		// TODO check offset

		buffer->Write(data, size, this->offset + offset);
	}

	inline void SubBuffer::Write(const void* data)
	{
		ASSERT_DEBUG(buffer == nullptr, "buffer is nullptr");

		buffer->Write(data, size, offset);
	}

	inline void SubBuffer::Clear()
	{
		buffer = nullptr;
		size = 0;
		offset = 0;
		block = nullptr;
		poolId = 0;
	}

	inline SubBuffer& SubBuffer::operator=(SubBuffer&& other)
	{
		Free();

		buffer = other.buffer;
		size = other.size;
		offset = other.offset;
		block = other.block;
		poolId = other.poolId;

		other.Clear();

		return *this;
	}
}