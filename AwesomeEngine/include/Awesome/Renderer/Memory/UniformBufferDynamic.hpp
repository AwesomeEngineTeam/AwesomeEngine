#pragma once

#ifndef AWESOME_RENDERER_UNIFORM_BUFFER_DYNAMIC_GUARD
#define AWESOME_RENDERER_UNIFORM_BUFFER_DYNAMIC_GUARD

#include <Awesome/Core/Types.hpp>

#include <Awesome/Renderer/Memory/AlignedSubBuffer.hpp>
#include <Awesome/Renderer/Memory/BufferManager.hpp>
#include <Awesome/Renderer/Memory/UniformBufferLayout.hpp>

namespace aw
{
	class UniformBufferDynamic
	{
		AlignedSubBuffer m_srcSub;
		AlignedSubBuffer m_dstSub;

		UniformBufferLayout m_layout;

	public:
		UniformBufferDynamic() = default;
		UniformBufferDynamic(const UniformBufferDynamic&) = default;
		UniformBufferDynamic(UniformBufferDynamic&&) = default;
		~UniformBufferDynamic() = default;

		inline void Create(uint64 elementSize, uint64 count);

		inline void Write(uint64 index, const void* data, uint64 size, uint64 offset = 0);
		inline void Write(uint64 index, const void* data);

		inline void Copy(vk::CommandBuffer commandBuffer);

		inline AlignedSubBuffer& GetDestination();
		inline const AlignedSubBuffer& GetDestination() const;
		
		inline AlignedSubBuffer& GetSource();
		inline const AlignedSubBuffer& GetSource() const;

		inline void SetLayout(UniformBufferLayout layout);

		UniformBufferDynamic& operator=(const UniformBufferDynamic&) = default;
		UniformBufferDynamic& operator=(UniformBufferDynamic&&) = default;
	};
}

#include <Awesome/Renderer/Memory/UniformBufferDynamic.inl>

#endif //GUARD