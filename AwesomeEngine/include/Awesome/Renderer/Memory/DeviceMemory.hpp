#pragma once

#ifndef AWESOME_RENDERER_DEVICE_MEMORY_GUARD
#define AWESOME_RENDERER_DEVICE_MEMORY_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>

#include <Awesome/Renderer/Control/Vulkan.hpp>

#include <Awesome/Core/Logger/Logger.hpp>

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Resource/Image.hpp>
#include <Awesome/Renderer/Memory/Buffer.hpp>

namespace aw
{
	class DeviceMemory
	{
		vk::UniqueDeviceMemory m_handle;

	public:
		DeviceMemory() = default;
		DeviceMemory(const DeviceMemory&) = delete;
		inline DeviceMemory(DeviceMemory&&);
		inline ~DeviceMemory();

		AWESOME_RENDERER_API void Allocate(uint64 size, uint8 memTypeId);
		//inline void Allocate(uint32 imageCount, Image* images, MemoryPropertyBit memoryProperty);
		//inline void Allocate(uint32 bufferCount, Buffer* buffers, MemoryPropertyBit memoryProperty);
		AWESOME_RENDERER_API void Free();

		AWESOME_RENDERER_API void Map(uint64 offset, uint64 size, void** data);
		AWESOME_RENDERER_API void Unmap();

		AWESOME_RENDERER_API void Flush(uint64 offset, uint64 size);

		inline vk::DeviceMemory Get() const;

	private:
		AWESOME_RENDERER_API uint32 FindMemoryType(uint32 typeFilter, vk::MemoryPropertyFlagBits properties);

	public:
		DeviceMemory& operator=(const DeviceMemory&) = delete;
		inline DeviceMemory& operator=(DeviceMemory&& rhs);
	};
}

#include <Awesome/Renderer/Memory/DeviceMemory.inl>

#endif //GUARD