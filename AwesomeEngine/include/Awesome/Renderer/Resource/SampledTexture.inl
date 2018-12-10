namespace aw
{
	inline vk::Sampler SampledTexture::GetSampler() const
	{
		return m_samplerHandle.get();
	}
}