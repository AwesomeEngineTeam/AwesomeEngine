namespace aw
{
	inline bool MemoryBlock::IsAligned(uint64 alignment) const
	{
		return alignment == 0 || offset % alignment == 0;
		//return offset % alignment == 0;
	}

	inline MemInfo::MemInfo(MemoryBlock* block, uint64 poolID, void* memPtr) :
		block(block),
		poolID(poolID),
		memPtr(memPtr)
	{
	}

	inline MemInfo::MemInfo(MemInfo&& other) noexcept :
		block(other.block),
		poolID(other.poolID),
		memPtr(other.memPtr)
	{
		other.block = nullptr;
		other.poolID = 0;
		other.memPtr = nullptr;
	}

	inline MemInfo::~MemInfo()
	{
		Destroy();
	}

	inline MemInfo& MemInfo::operator=(MemInfo&& other)
	{
		block = other.block;
		poolID = other.poolID;
		memPtr = other.memPtr;

		other.block = nullptr;
		other.poolID = 0;
		other.memPtr = nullptr;

		return *this;
	}
}