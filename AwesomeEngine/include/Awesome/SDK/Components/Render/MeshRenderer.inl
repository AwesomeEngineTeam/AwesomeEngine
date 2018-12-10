namespace aw
{
	namespace sdk
	{
		inline MeshRenderer::MeshRenderer(sdk::Actor* actor, uint32 typeId, const std::string& name) :
			Component(actor, typeId, name)
		{
		}

		inline MeshRenderer::MeshRenderer(sdk::Actor* actor) :
			MeshRenderer(actor, uint32(ComponentID::MeshRenderer), "Unnamed MeshRenderer")
		{
		}

		inline uint32 MeshRenderer::GetMesh() const
		{
			return m_meshIndex;
		}

		inline void MeshRenderer::SetMesh(uint32 mesh)
		{
			m_meshIndex = mesh;
		}

		inline uint32 MeshRenderer::GetMaterial() const
		{
			return m_materialIndex;
		}

		inline void MeshRenderer::SetMaterial(uint32 mat)
		{
			m_materialIndex = mat;
		}

		inline UniformBufferDynamicBlock& MeshRenderer::GetDynamicBlock()
		{
			return m_dynamicBlock;
		}

		inline const UniformBufferDynamicBlock& MeshRenderer::GetDynamicBlock() const
		{
			return m_dynamicBlock;
		}
	}
}