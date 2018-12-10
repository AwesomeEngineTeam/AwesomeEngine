namespace aw
{
	namespace ph
	{
		inline ConvexMeshGeometry::ConvexMeshGeometry(std::shared_ptr<ConvexMesh> mesh, const MeshScale& scale) :
			m_handle(mesh->Get(), scale.Get()),
			m_sharedMesh(mesh)
		{
		}

		inline ConvexMeshGeometry::ConvexMeshGeometry(ConvexMeshGeometry&& other) :
			m_handle{ std::move(other.m_handle) },
			m_sharedMesh{ std::move(other.m_sharedMesh) }
		{
		}
		
		inline ConvexMeshGeometry::ConvexMeshGeometry(const ConvexMeshGeometry& other) :
			m_handle{ other.m_handle },
			m_sharedMesh{ other.m_sharedMesh }
		{
		}

		inline GeometryType ConvexMeshGeometry::GetType() const
		{
			return GeometryType(m_handle.getType());
		}

		inline physx::PxGeometry& ConvexMeshGeometry::Get()
		{
			return m_handle;
		}
		
		inline const physx::PxGeometry& ConvexMeshGeometry::Get() const
		{
			return m_handle;
		}

		inline bool ConvexMeshGeometry::IsValid() const
		{
			return m_handle.isValid();
		}

		inline MeshScale& ConvexMeshGeometry::Scale()
		{
			return reinterpret_cast<MeshScale&>(m_handle.scale);
		}

		inline const MeshScale& ConvexMeshGeometry::Scale() const
		{
			return reinterpret_cast<const MeshScale&>(m_handle.scale);
		}

		inline std::shared_ptr<ConvexMesh> ConvexMeshGeometry::Mesh()
		{
			return m_sharedMesh;
		}

		inline const std::shared_ptr<ConvexMesh> ConvexMeshGeometry::Mesh() const
		{
			return m_sharedMesh;
		}

		inline ConvexMeshGeometry& ConvexMeshGeometry::operator=(ConvexMeshGeometry&& rhs)
		{
			m_handle = std::move(rhs.m_handle);
			m_sharedMesh = std::move(rhs.m_sharedMesh);

			return *this;
		}
		
		inline ConvexMeshGeometry& ConvexMeshGeometry::operator=(const ConvexMeshGeometry& rhs)
		{
			m_handle = rhs.m_handle;
			m_sharedMesh = rhs.m_sharedMesh;

			return *this;
		}
	}
}