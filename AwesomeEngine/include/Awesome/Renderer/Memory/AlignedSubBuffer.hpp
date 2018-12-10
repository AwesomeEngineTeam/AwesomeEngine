#pragma once

#ifndef AWESOME_RENDERER_ALIGNED_SUB_BUFFER_GUARD
#define AWESOME_RENDERER_ALIGNED_SUB_BUFFER_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>

#include <Awesome/Core/Types.hpp>
#include <Awesome/Core/Debug.hpp>

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Memory/Buffer.hpp>
#include <Awesome/Renderer/Memory/SubBuffer.hpp>

namespace aw
{
	struct AlignedSubBuffer
	{
		Buffer* buffer{ nullptr };
		uint64 offset{ 0 };
		uint64 elementSize{ 0 };
		uint64 count{ 0 };
		uint64 alignment{ 0 };
		MemoryBlock* block{ nullptr };
		uint64 poolId{ 0 };

		AlignedSubBuffer() = default;
		inline AlignedSubBuffer(Buffer* buffer, uint64 offset, uint64 elementSize, uint64 count, MemoryBlock* block = nullptr, uint64 poolId = 0);
		AlignedSubBuffer(const AlignedSubBuffer&) = default;
		inline AlignedSubBuffer(AlignedSubBuffer&& other) noexcept;
		inline AlignedSubBuffer(SubBuffer&& subBuffer);
		AWESOME_RENDERER_API ~AlignedSubBuffer();

		inline void Write(uint64 index, const void* data, uint64 size, uint64 offset = 0);
		inline void Write(uint64 index, const void* data);

		inline uint64 GetOffsetAt(uint64 index) const;

		inline uint64 ComputeFullSize() const;

		AWESOME_RENDERER_API static uint64 ComputeAlignment(uint64 size);
		inline static uint64 ComputeFullSize(uint64 size, uint64 count);
		inline static uint64 ComputeFullSize(uint64 size, uint64 count, uint64 alignment);

		inline SubBuffer ConvertToSubBuffer();

		AlignedSubBuffer& operator=(const AlignedSubBuffer&) = default;
		inline AlignedSubBuffer& operator=(AlignedSubBuffer&& other) noexcept;
		inline AlignedSubBuffer& operator=(SubBuffer&& other);
	};
}

#include <Awesome/Renderer/Memory/AlignedSubBuffer.inl>

#endif //GUARD