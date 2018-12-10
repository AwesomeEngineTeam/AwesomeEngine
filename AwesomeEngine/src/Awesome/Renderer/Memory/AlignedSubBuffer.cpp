#include <Awesome/Renderer/Memory/AlignedSubBuffer.hpp>
#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Memory/BufferManager.hpp>

namespace aw
{
	AlignedSubBuffer::~AlignedSubBuffer()
	{
		BufferManager::Free(poolId, block);
	}

	uint64 AlignedSubBuffer::ComputeAlignment(uint64 size)
	{
		static const uint64 minUboAlignment = Context::GetDevice().GetDeviceProperties().limits.minUniformBufferOffsetAlignment; // TODO is static portable?

		uint64 alignment = size;
		if (minUboAlignment > 0)
			alignment = (alignment + minUboAlignment - 1) & ~(minUboAlignment - 1);

		return alignment;
	}
}