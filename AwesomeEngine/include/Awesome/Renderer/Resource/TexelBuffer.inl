namespace aw
{
	inline bool TexelBuffer::IsCompatible(const TexelBuffer& other) const
	{
		return width == other.width &&
				height == other.height &&
				channels == other.channels;
	}
}