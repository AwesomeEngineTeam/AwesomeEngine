#include <Awesome/Renderer/Resource/TexelBuffer.hpp>

#include <Awesome/Core/Debug.hpp>

namespace aw
{
	void TexelBuffer::Clear()
	{
		width = 0;
		height = 0;
		channels = 0;
		layerCount = 1;
		texels.clear();
	}

	TexelBuffer& TexelBuffer::operator+=(const TexelBuffer& other)
	{
		if (IsCompatible(other) == true)
		{
			uint64 oldSize = texels.size();
			texels.resize(oldSize + other.texels.size());
			memcpy(texels.data() + oldSize, other.texels.data(), other.texels.size());
		}
		else
		{
			LOG_ERROR("TexelBuffers not compatible (different sizes or channels) (%d/%d %d/%d %d/%d)", width, other.width, height, other.height, channels, other.channels);
		}

		return *this;
	}
}