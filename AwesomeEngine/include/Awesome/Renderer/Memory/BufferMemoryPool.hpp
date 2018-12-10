#pragma once

#ifndef AWESOME_RENDERER_BUFFER_MEMORY_POOL_GUARD
#define AWESOME_RENDERER_BUFFER_MEMORY_POOL_GUARD

#include <Awesome/Core/MemoryPool.hpp>

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Memory/Buffer.hpp>
#include <Awesome/Renderer/Memory/SubBuffer.hpp>

namespace aw
{
	enum class BufferType : uint8
	{
		None = 0x0,
		Staging = 0x1,
		Geometry = 0x2,
		Uniform = 0x4,
		View = 0x10
	};

	class BufferMemoryPool : public MemoryPool
	{
		friend class BufferManager;

		static uint64 s_count;

		uint64 m_id = 0;

		Buffer m_buffer;

		BufferType m_bufferType = BufferType::None;

	public:
		BufferMemoryPool() = default;
		inline BufferMemoryPool(BufferMemoryPool&& other) noexcept;
		~BufferMemoryPool() override = default;

		AWESOME_RENDERER_API void Init(uint64 size, BufferType type, bool concurrent = false);

		AWESOME_RENDERER_API SubBuffer Reserve(uint64 size, bool aligned);
		AWESOME_RENDERER_API void Free(MemoryBlock* block);

		AWESOME_RENDERER_API void Write(MemoryBlock* block, void* data, uint64 size, uint64 offset = 0);
		AWESOME_RENDERER_API void Write(MemoryBlock* block, void* data);

		inline uint64 Id() const;
		inline BufferType GetBufferType() const;
		inline Buffer& Buffer();

		inline BufferMemoryPool& operator=(BufferMemoryPool&& rhs) noexcept;
	};
}

#include <Awesome/Renderer/Memory/BufferMemoryPool.inl>

#endif //Guard