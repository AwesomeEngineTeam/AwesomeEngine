#pragma once

#ifndef AWESOME_RENDERER_BUFFER_GUARD
#define AWESOME_RENDERER_BUFFER_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>

#include <Awesome/Renderer/Control/Vulkan.hpp>

#include <Awesome/Core/Types.hpp>

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Memory/MemInfo.hpp>

namespace aw
{
	class Buffer
	{
		vk::UniqueBuffer m_handle;
		MemInfo m_memInfo;

	public:
		Buffer() = default;
		inline Buffer(vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, uint32 size, bool concurent = false);
		Buffer(const Buffer&) = delete;
		inline Buffer(Buffer&& other) noexcept;
		inline ~Buffer();

		AWESOME_RENDERER_API void Create(vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, uint64 size, bool concurrent = false);
		AWESOME_RENDERER_API void Destroy();

		inline vk::Buffer Get() const;

		AWESOME_RENDERER_API void Read(void* data, uint64 size, uint64 offset = 0);
		AWESOME_RENDERER_API void Write(const void* data, uint64 size, uint64 offset = 0);

		Buffer& operator=(const Buffer&) = delete;
		inline Buffer& operator=(Buffer&& rhs) noexcept;
	};
}

#include <Awesome/Renderer/Memory/Buffer.inl>

#endif //GUARD