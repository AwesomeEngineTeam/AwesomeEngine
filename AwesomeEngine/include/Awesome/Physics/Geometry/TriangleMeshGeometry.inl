namespace aw
{
	namespace ph
	{
		inline TriangleMeshGeometry::TriangleMeshGeometry(std::shared_ptr<TriangleMesh> mesh, const MeshScale& scale) :
			m_handle(mesh->Get(), scale.Get()),
			m_sharedMesh(mesh)
		{
		}

		inline TriangleMeshGeometry::TriangleMeshGeometry(TriangleMeshGeometry&& other) :
			m_handle{ std::move(other.m_handle) },
			m_sharedMesh{ std::move(other.m_sharedMesh) }
		{
		}

		inline TriangleMeshGeometry::TriangleMeshGeometry(const TriangleMeshGeometry& other) :
			m_handle{other.m_handle},
			m_sharedMesh{ other.m_sharedMesh }
		{
		}

		inline GeometryType TriangleMeshGeometry::GetType() const
		{
			return GeometryType(m_handle.getType());
		}

		inline physx::PxGeometry& TriangleMeshGeometry::Get()
		{
			return m_handle;
		}

		inline const physx::PxGeometry& TriangleMeshGeometry::Get() const
		{
			return m_handle;
		}

		inline bool TriangleMeshGeometry::IsValid() const
		{
			return m_handle.isValid();
		}

		inline MeshScale& TriangleMeshGeometry::Scale()
		{
			return reinterpret_cast<MeshScale&>(m_handle.scale);
		}

		inline const MeshScale& TriangleMeshGeometry::Scale() const
		{
			return reinterpret_cast<const MeshScale&>(m_handle.scale);
		}

		inline std::shared_ptr<TriangleMesh> TriangleMeshGeometry::Mesh()
		{
			return m_sharedMesh;
		}

		inline const std::shared_ptr<TriangleMesh> TriangleMeshGeometry::Mesh() const
		{
			return m_sharedMesh;
		}

		inline TriangleMeshGeometry& TriangleMeshGeometry::operator=(TriangleMeshGeometry&& rhs)
		{
			m_handle = std::move(rhs.m_handle);
			m_sharedMesh = std::move(rhs.m_sharedMesh);

			return *this;
		}

		inline TriangleMeshGeometry& TriangleMeshGeometry::operator=(const TriangleMeshGeometry& rhs)
		{
			m_handle = rhs.m_handle;
			m_sharedMesh = rhs.m_sharedMesh;

			return *this;
		}
	}
}