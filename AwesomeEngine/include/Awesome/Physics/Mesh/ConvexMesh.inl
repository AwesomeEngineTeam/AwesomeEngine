namespace aw
{
	namespace ph
	{
		inline ConvexMesh::ConvexMesh(const ConvexMeshDescriptor& desc)
		{
			Create(desc);
		}

		inline ConvexMesh::ConvexMesh(ConvexMesh&& other) : m_handle{ other.m_handle }
		{
			other.m_handle = nullptr;
		}

		inline ConvexMesh::~ConvexMesh()
		{
			Destroy();
		}

		inline physx::PxConvexMesh* ConvexMesh::Get()
		{
			return m_handle;
		}

		inline const physx::PxConvexMesh* ConvexMesh::Get() const
		{
			return m_handle;
		}

		inline void ConvexMesh::Destroy()
		{
			if (m_handle)
				m_handle->release();

			m_handle = nullptr;
		}

		inline ConvexMesh& ConvexMesh::operator=(ConvexMesh&& rhs)
		{
			Destroy();

			m_handle = rhs.m_handle;
			rhs.m_handle = nullptr;

			return *this;
		}
	}
}