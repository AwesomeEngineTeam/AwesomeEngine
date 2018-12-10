namespace aw
{
	inline TexelBuffer::TexelBuffer(TexelBuffer&& other) noexcept :
		width(other.width),
		height(other.height),
		texels(std::move(other.texels)),
		channels(other.channels)
	{
	}

	inline bool TexelBuffer::IsCompatible(const TexelBuffer& other) const
	{
		return width == other.width &&
				height == other.height &&
				channels == other.channels;
	}

	inline TexelBuffer& TexelBuffer::operator=(TexelBuffer&& other) noexcept
	{
		width = other.width;
		height = other.height;
		channels = other.channels;
		texels = std::move(other.texels);

		return *this;
	}
}