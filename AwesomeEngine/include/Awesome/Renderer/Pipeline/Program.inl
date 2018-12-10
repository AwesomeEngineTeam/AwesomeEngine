namespace aw
{
	inline void Program::AddShaderModule(vk::ShaderModule shader, vk::ShaderStageFlagBits stage, const char* name)
	{
		vk::PipelineShaderStageCreateInfo shaderStageInfo(
			vk::PipelineShaderStageCreateFlags(),
			stage,
			shader,
			name
		);

		m_stages.push_back(shaderStageInfo);
	}

	inline void Program::AddShaderModule(vk::UniqueShaderModule&& shader, vk::ShaderStageFlagBits stage, const char* name)
	{
		m_modules.emplace_back(std::move(shader));

		AddShaderModule(m_modules.back().get(), stage, name);
	}

	inline const vk::PipelineShaderStageCreateInfo* Program::GetStages() const
	{
		return m_stages.data();
	}

	inline uint32 Program::GetStageCount() const
	{
		return uint32(m_stages.size());
	}
}