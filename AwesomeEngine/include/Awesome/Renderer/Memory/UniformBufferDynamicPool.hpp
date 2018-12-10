#pragma once

#ifndef AWESOME_RENDERER_UNIFORM_BUFFER_MEMORY_POOL_GUARD
#define AWESOME_RENDERER_UNIFORM_BUFFER_MEMORY_POOL_GUARD

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Memory/UniformBufferDynamic.hpp>
#include <Awesome/Renderer/Memory/UniformBufferDynamicBlock.hpp>

namespace aw
{
	class UniformBufferDynamicPool
	{
		AWESOME_RENDERER_API static uint64 s_count;

		uint64 m_id{ 0 };

		UniformBufferDynamic m_buffer;

		std::vector<bool> m_availables{};
		uint64 m_availableCount{ 0 };

	public:
		UniformBufferDynamicPool() = default;
		inline UniformBufferDynamicPool(UniformBufferDynamicPool&& other) noexcept;
		~UniformBufferDynamicPool() = default;

		inline void Init(uint64 size, uint64 count);

		AWESOME_RENDERER_API UniformBufferDynamicBlock Reserve();
		inline void Free(uint64 blockIndex);

		inline void Write(uint64 blockIndex, const void* data, uint64 size, uint64 offset);
		inline void Write(uint64 blockIndex, const void* data);

		inline uint64 Id() const;
		inline UniformBufferDynamic& UniformBufferDynamic();
		inline uint64 AvailableBlockCount();
		
		inline UniformBufferDynamicPool& operator=(UniformBufferDynamicPool&& rhs) noexcept;
	};
}

#include <Awesome/Renderer/Memory/UniformBufferDynamicPool.inl>

#endif //Guard