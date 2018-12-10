#pragma once

#ifndef AWESOME_RENDERER_SUB_BUFFER_GUARD
#define AWESOME_RENDERER_SUB_BUFFER_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>

#include <Awesome/Core/Types.hpp>
#include <Awesome/Core/Debug.hpp>

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Memory/Buffer.hpp>

namespace aw
{
	struct SubBuffer
	{
		Buffer* buffer{ nullptr };
		uint64 size{ 0 };
		uint64 offset{ 0 };
		MemoryBlock* block{ nullptr };
		uint64 poolId{ 0 };

		SubBuffer() = default;
		inline SubBuffer(Buffer* buffer, uint64 size, uint64 offset = 0, MemoryBlock* block = nullptr, uint64 poolId = 0);
		inline SubBuffer(const SubBuffer& other, uint64 size, uint64 offset = 0, MemoryBlock* block = nullptr, uint64 poolId = 0);
		SubBuffer(const SubBuffer&) = delete;
		inline SubBuffer(SubBuffer&& other);
		inline ~SubBuffer();

		inline void Read(void* data, uint64 size, uint64 offset = 0);

		inline void Write(const void* data, uint64 size, uint64 offset = 0);
		inline void Write(const void* data);

		AWESOME_RENDERER_API void Free();
		inline void Clear();

		SubBuffer& operator=(const SubBuffer&) = delete;
		inline SubBuffer& operator=(SubBuffer&& other);
	};
}

#include <Awesome/Renderer/Memory/SubBuffer.inl>

#endif //GUARD