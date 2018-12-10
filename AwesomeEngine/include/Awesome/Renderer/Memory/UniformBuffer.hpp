#pragma once

#ifndef AWESOME_RENDERER_UNIFORM_BUFFER_GUARD
#define AWESOME_RENDERER_UNIFORM_BUFFER_GUARD

#include <Awesome/Core/Types.hpp>

#include <Awesome/Renderer/Memory/SubBuffer.hpp>
#include <Awesome/Renderer/Memory/BufferManager.hpp>
#include <Awesome/Renderer/Memory/UniformBufferLayout.hpp>

namespace aw
{
	class UniformBuffer
	{
		SubBuffer m_srcSub;
		SubBuffer m_dstSub;

		UniformBufferLayout m_layout;

	public:
		UniformBuffer() = default;
		UniformBuffer(const UniformBuffer&) = default;
		UniformBuffer(UniformBuffer&&) = default;
		~UniformBuffer() = default;

		inline void Create(uint64 size);

		inline void Write(const void* data, uint64 size, uint64 offset = 0);
		inline void Write(const void* data);

		inline void Copy(vk::CommandBuffer commandBuffer);

		inline SubBuffer& GetDestination();
		inline const SubBuffer& GetDestination() const;

		inline void SetLayout(UniformBufferLayout layout);

		UniformBuffer& operator=(const UniformBuffer&) = default;
		UniformBuffer& operator=(UniformBuffer&&) = default;
	};
}

#include <Awesome/Renderer/Memory/UniformBuffer.inl>

#endif //GUARD