namespace aw
{
	namespace ph
	{
		inline void* AllocatorCallback::allocate(size_t size, const char* typeName, const char* filename,
			int line)
		{
			void* ptr = ::malloc(size);
			ASSERT((reinterpret_cast<size_t>(ptr) & 15) != 0, "PhysX Allocator failed!");
			return ptr;
		}
		inline void AllocatorCallback::deallocate(void* ptr)
		{
			::free(ptr);
		}
	}
}