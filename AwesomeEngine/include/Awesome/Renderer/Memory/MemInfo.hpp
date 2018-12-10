#pragma once

#ifndef AWESOME_RENDERER_MEM_INFO_GUARD
#define AWESOME_RENDERER_MEM_INFO_GUARD

#include <Awesome/Core/Types.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>
#include <Awesome/Renderer/Config.hpp>

namespace aw
{
	struct MemoryBlock
	{
		uint64 size = 0;
		uint64 offset = 0;

		bool free = true;

		MemoryBlock* prev = nullptr;
		MemoryBlock* next = nullptr;

		inline bool IsAligned(uint64 alignment) const;
	};

	struct MemInfo
	{
		MemoryBlock* block = nullptr;
		uint64 poolID = 0;
		void* memPtr = nullptr;

		MemInfo() = default;
		inline explicit MemInfo(MemoryBlock* block, uint64 poolID = 0, void* memPtr = nullptr);
		MemInfo(const MemInfo&) = delete;
		inline MemInfo(MemInfo&& other) noexcept;
		inline ~MemInfo();

		AWESOME_RENDERER_API void Destroy();

		MemInfo& operator=(const MemInfo&) = delete;
		inline MemInfo& operator=(MemInfo&& other);
	};
}

#include <Awesome/Renderer/Memory/MemInfo.inl>

#endif //Guard