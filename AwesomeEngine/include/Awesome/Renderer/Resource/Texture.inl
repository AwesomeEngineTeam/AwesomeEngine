namespace aw
{
	inline void Texture::SetTexelBuffer(TexelBuffer texelBuffer)
	{
		m_texelBuffer = std::move(texelBuffer);
		m_isCoherent = false;
	}

	inline const TexelBuffer& Texture::GetTexelBuffer() const
	{
		return m_texelBuffer;
	}

	inline bool Texture::IsCoherent() const
	{
		return m_isCoherent;
	}

	inline vk::ImageView Texture::GetImageView() const
	{
		return m_imageViewHandle.get();
	}

	inline vk::Image Texture::GetImage() const
	{
		return m_imageHandle.Get();
	}

	inline Texture::operator Image&()
	{
		return m_imageHandle;
	}

	inline Texture::operator const Image&() const
	{
		return m_imageHandle;
	}
}