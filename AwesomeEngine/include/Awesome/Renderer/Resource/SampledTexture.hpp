#pragma once

#ifndef AWESOME_RENDERER_SAMPLED_TEXTURE_2D_GUARD
#define AWESOME_RENDERER_SAMPLED_TEXTURE_2D_GUARD

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Resource/Texture.hpp>

#include <memory>

namespace aw
{
	class CommandBuffer;
	struct TexelBuffer;

	class SampledTexture : public Texture
	{
		vk::UniqueSampler m_samplerHandle;

	public:
		AWESOME_RENDERER_API SampledTexture();
		AWESOME_RENDERER_API SampledTexture(const vk::SamplerCreateInfo& createInfo);
		SampledTexture(const SampledTexture&) = default;
		SampledTexture(SampledTexture&&) = default;
		~SampledTexture() override = default;

		inline vk::Sampler GetSampler() const;

		SampledTexture& operator=(const SampledTexture&) = default;
		SampledTexture& operator=(SampledTexture&&) = default;
	};

	class SampledTextureHandle
	{
		vk::Image m_image{};
		vk::ImageView m_imageView{};
		vk::Sampler m_sampler{};

	public:
		SampledTextureHandle() = default;
		SampledTextureHandle(const SampledTextureHandle&) = default;
		SampledTextureHandle(SampledTextureHandle&&) = default;
		inline SampledTextureHandle(vk::Image image, vk::ImageView imageView, vk::Sampler sampler) : m_image(image), m_imageView(imageView), m_sampler(sampler) {}
		inline explicit SampledTextureHandle(const SampledTexture& tex) : SampledTextureHandle(tex.GetImage(), tex.GetImageView(), tex.GetSampler()) {}

		inline vk::ImageView GetImageView() const { return m_imageView; }
		inline vk::Image GetImage() const { return m_image; }
		inline vk::Sampler GetSampler() const { return m_sampler; }

		SampledTextureHandle& operator=(const SampledTextureHandle&) = default;
		SampledTextureHandle& operator=(SampledTextureHandle&&) = default;
		/*SampledTextureHandle& operator=(const SampledTexture& tex)
		{
			m_image = tex.GetImage();
			m_imageView = tex.GetImageView();
			m_sampler = tex.GetSampler();

			return *this;
		};*/
	};
}

#include <Awesome/Renderer/Resource/SampledTexture.inl>

#endif //GUARD