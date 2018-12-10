namespace aw
{
	inline void Allocator::Init()
	{
		LogDeviceProperties();
		s_bufferImageGranularity = Context::GetDevice().GetDeviceProperties().limits.bufferImageGranularity;
	}
}