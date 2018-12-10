namespace aw
{
	inline ShaderHandle::ShaderHandle(const Shader& s) :
		pipeline{ s.pipeline.get() },
		pipelineLayout{ s.pipelineLayout.get() }
	{
	}
}