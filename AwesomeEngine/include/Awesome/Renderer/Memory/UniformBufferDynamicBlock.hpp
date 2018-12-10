#pragma once

#ifndef AWESOME_RENDERER_UNIFORM_SUB_BUFFER_GUARD
#define AWESOME_RENDERER_UNIFORM_SUB_BUFFER_GUARD

#include <Awesome/Core/Types.hpp>

#include <Awesome/Renderer/Config.hpp>

namespace aw
{
	class UniformBufferDynamicBlock
	{
		class UniformBufferDynamicPool* m_pool{ nullptr };
		uint64 m_index{ ~0 };
		uint64 m_offset{ ~0 };

	public:
		UniformBufferDynamicBlock() = default;
		inline UniformBufferDynamicBlock(UniformBufferDynamicPool* pool, uint64 index, uint64 offset);
		UniformBufferDynamicBlock(const UniformBufferDynamicBlock&) = delete;
		inline UniformBufferDynamicBlock(UniformBufferDynamicBlock&& other) noexcept;
		inline ~UniformBufferDynamicBlock();

		AWESOME_RENDERER_API void Write(const void* data, uint64 size, uint64 offset = 0);
		AWESOME_RENDERER_API void Write(const void* data);

		AWESOME_RENDERER_API void Free();
		inline void Clear();

		inline uint64 GetOffset() const;

		UniformBufferDynamicBlock& operator=(const UniformBufferDynamicBlock&) = delete;
		inline UniformBufferDynamicBlock& operator=(UniformBufferDynamicBlock&& other) noexcept;
	};
}

#include <Awesome/Renderer/Memory/UniformBufferDynamicBlock.inl>

#endif //GUARD