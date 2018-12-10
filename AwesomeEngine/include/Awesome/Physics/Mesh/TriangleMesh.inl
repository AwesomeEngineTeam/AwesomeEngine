namespace aw
{
	namespace ph
	{
		inline TriangleMesh::TriangleMesh(const TriangleMeshDescriptor& desc)
		{
			Create(desc);
		}

		inline TriangleMesh::TriangleMesh(TriangleMesh&& other) : m_handle{ other.m_handle }
		{
			other.m_handle = nullptr;
		}

		inline TriangleMesh::~TriangleMesh()
		{
			Destroy();
		}

		inline physx::PxTriangleMesh* TriangleMesh::Get()
		{
			return m_handle;
		}

		inline const physx::PxTriangleMesh* TriangleMesh::Get() const
		{
			return m_handle;
		}

		inline void TriangleMesh::Destroy()
		{
			if (m_handle)
				m_handle->release();

			m_handle = nullptr;
		}

		inline TriangleMesh& TriangleMesh::operator=(TriangleMesh&& rhs)
		{
			Destroy();

			m_handle = rhs.m_handle;
			rhs.m_handle = nullptr;

			return *this;
		}
	}
}