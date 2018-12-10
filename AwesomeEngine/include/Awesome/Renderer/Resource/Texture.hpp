#pragma once

#ifndef AWESOME_RENDERER_TEXTURE_GUARD
#define AWESOME_RENDERER_TEXTURE_GUARD

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Resource/Image.hpp>
#include <Awesome/Renderer/Resource/TexelBuffer.hpp>

#include <memory>

namespace aw
{
	class CommandBuffer;

	class Texture
	{
	protected:
		Image m_imageHandle;
		vk::UniqueImageView m_imageViewHandle;
		TexelBuffer m_texelBuffer;

		bool m_isCoherent{ false };

	public:
		Texture() = default;
		Texture(const Texture&) = default;
		Texture(Texture&&) = default;
		virtual ~Texture() = default;

		AWESOME_RENDERER_API void Create(vk::ImageType type, vk::Format format, TexelBuffer texelBuffer, vk::ImageUsageFlags usage, vk::ImageAspectFlags aspect);
		AWESOME_RENDERER_API void Create(vk::ImageType type, vk::Format format, vk::Extent3D extent, vk::ImageUsageFlags usage, vk::ImageAspectFlags aspect);

		AWESOME_RENDERER_API void Destroy();

		inline void SetTexelBuffer(TexelBuffer texelBuffer);
		inline const TexelBuffer& GetTexelBuffer() const;

		AWESOME_RENDERER_API void Flush(); // TODO maybe implement SetTexelsImmediate / SetTexelsAsync ?

		inline bool IsCoherent() const;

		inline vk::ImageView GetImageView() const;
		inline vk::Image GetImage() const;

		inline explicit operator Image&();
		inline explicit operator const Image&() const;

		Texture& operator=(const Texture&) = default;
		Texture& operator=(Texture&&) = default;
	};

	class TextureHandle
	{
		vk::Image m_image{};
		vk::ImageView m_imageView{};

	public:
		TextureHandle() = default;
		TextureHandle(const TextureHandle&) = default;
		TextureHandle(TextureHandle&&) = default;
		inline TextureHandle(vk::Image image, vk::ImageView imageView) : m_image(image), m_imageView(imageView) {}
		inline explicit TextureHandle(const Texture& tex) : TextureHandle(tex.GetImage(), tex.GetImageView()) {}

		inline vk::ImageView GetImageView() const { return m_imageView; }
		inline vk::Image GetImage() const { return m_image; }

		TextureHandle& operator=(const TextureHandle&) = default;
		TextureHandle& operator=(TextureHandle&&) = default;
	};
}

#include <Awesome/Renderer/Resource/Texture.inl>

#endif //GUARD