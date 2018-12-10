#pragma once

#ifndef AWESOME_RENDERER_TEXEL_BUFFER_GUARD
#define AWESOME_RENDERER_TEXEL_BUFFER_GUARD

#include <Awesome/Renderer/Config.hpp>

#include <Awesome/Core/Types.hpp>

#include <vector>

namespace aw
{
	struct TexelBuffer
	{
		uint64 width{ 0 };
		uint64 height{ 0 };
		std::vector<uint8> texels{};
		uint64 channels{ 0 };
		uint32 layerCount{ 1 };

		TexelBuffer() = default;
		TexelBuffer(const TexelBuffer&) = default;
		TexelBuffer(TexelBuffer&&) = default;
		~TexelBuffer() = default;

		AWESOME_RENDERER_API void Clear();

		AWESOME_RENDERER_API TexelBuffer& operator+=(const TexelBuffer& other);

		inline bool IsCompatible(const TexelBuffer& other) const;

		TexelBuffer& operator=(const TexelBuffer&) = default;
		TexelBuffer& operator=(TexelBuffer&&) = default;
	};
}

#include <Awesome/Renderer/Resource/TexelBuffer.inl>

#endif //GUARD