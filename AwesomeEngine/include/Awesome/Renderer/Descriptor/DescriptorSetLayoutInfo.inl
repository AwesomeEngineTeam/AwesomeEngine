namespace aw
{
	inline const vk::DescriptorSetLayoutCreateInfo& DescriptorSetLayoutInfo::GetCreateInfo() const
	{
		m_descriptorSetLayoutCreateInfo.pBindings = m_descriptorSetLayoutBindings.data();
		return m_descriptorSetLayoutCreateInfo;
	}

	inline const std::vector<vk::DescriptorPoolSize>& DescriptorSetLayoutInfo::GetDescriptorPoolSizes() const
	{
		return m_descriptorPoolSizes;
	}

	inline const std::vector<vk::DescriptorSetLayoutBinding>& DescriptorSetLayoutInfo::GetBindings() const
	{
		return m_descriptorSetLayoutBindings;
	}

	inline void DescriptorSetLayoutInfo::AddBinding(vk::DescriptorType type, vk::ShaderStageFlags flags, uint32 descriptorCount, const vk::Sampler* samplers)
	{
		uint32 index = uint32(m_descriptorSetLayoutBindings.size());

		SetBindingCount(uint32(m_descriptorSetLayoutBindings.size() + 1));
		SetBindingType(index, type);
		SetBindingDescriptorCount(index, descriptorCount);
		SetBindingStageFlags(index, flags);
		SetBindingImmutableSamplers(index, samplers);

		m_descriptorSetLayoutBindings[index].binding = index;
	}

	inline void DescriptorSetLayoutInfo::SetBindingCount(uint32 count)
	{
		m_descriptorSetLayoutBindings.resize(count);
		m_descriptorSetLayoutCreateInfo.bindingCount = count;
		m_descriptorPoolSizes.resize(count);
	}

	inline void DescriptorSetLayoutInfo::SetBindingType(uint32 bindingIndex, vk::DescriptorType type)
	{
		if (m_descriptorSetLayoutBindings.size() <= bindingIndex)
		{
			LOG_WARNING("Wrong descriptor set layout binding index specified (specified = %u, binding count = %llu)", bindingIndex, m_descriptorSetLayoutBindings.size());
			return;
		}

		m_descriptorSetLayoutBindings[bindingIndex].descriptorType = type;
		m_descriptorPoolSizes[bindingIndex].type = type;
	}

	inline void DescriptorSetLayoutInfo::SetBindingDescriptorCount(uint32 bindingIndex, uint32 count)
	{
		if (m_descriptorSetLayoutBindings.size() <= bindingIndex)
		{
			LOG_WARNING("Wrong descriptor set layout binding index specified (specified = %u, binding count = %llu)", bindingIndex, m_descriptorSetLayoutBindings.size());
			return;
		}

		m_descriptorSetLayoutBindings[bindingIndex].descriptorCount = count;
		m_descriptorPoolSizes[bindingIndex].descriptorCount = count;
	}

	inline void DescriptorSetLayoutInfo::SetBindingStageFlags(uint32 bindingIndex, vk::ShaderStageFlags flags)
	{
		if (m_descriptorSetLayoutBindings.size() <= bindingIndex)
		{
			LOG_WARNING("Wrong descriptor set layout binding index specified (specified = %u, binding count = %llu)", bindingIndex, m_descriptorSetLayoutBindings.size());
			return;
		}

		m_descriptorSetLayoutBindings[bindingIndex].stageFlags = flags;
	}

	inline void DescriptorSetLayoutInfo::SetBindingImmutableSamplers(uint32 bindingIndex, const vk::Sampler* samplers)
	{
		if (m_descriptorSetLayoutBindings.size() <= bindingIndex)
		{
			LOG_WARNING("Wrong descriptor set layout binding index specified (specified = %u, binding count = %llu)", bindingIndex, m_descriptorSetLayoutBindings.size());
			return;
		}

		m_descriptorSetLayoutBindings[bindingIndex].pImmutableSamplers = samplers;
	}
}