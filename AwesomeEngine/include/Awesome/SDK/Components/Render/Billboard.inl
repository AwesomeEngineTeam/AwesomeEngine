namespace aw
{
	namespace sdk
	{
		inline Billboard::Billboard(Actor* actor, uint32 typeId, const std::string& name) :
			Component(actor, typeId, name)
		{
		}

		inline Billboard::Billboard(Actor* actor) :
			Billboard(actor, uint32(ComponentID::Billboard), "Unnamed Billboard")
		{
		}

		inline uint32 Billboard::GetMesh() const
		{
			return m_meshIndex;
		}

		inline void Billboard::SetMesh(uint32 mesh)
		{
			m_meshIndex = mesh;
		}

		inline uint32 Billboard::GetMaterial() const
		{
			return m_materialIndex;
		}

		inline void Billboard::SetMaterial(uint32 mat)
		{
			m_materialIndex = mat;
		}
	}
}