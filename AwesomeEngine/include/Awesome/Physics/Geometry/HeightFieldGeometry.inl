namespace aw
{
	namespace ph
	{
		inline HeightFieldGeometry::HeightFieldGeometry(std::shared_ptr<HeightField> mesh, float heightScale, float rowScale, float columnScale) :
			m_handle(mesh->Get(), physx::PxMeshGeometryFlags(), heightScale, rowScale, columnScale),
			m_sharedMesh(mesh)
		{
		}

		inline HeightFieldGeometry::HeightFieldGeometry(std::shared_ptr<HeightField> mesh, Vector3 scale) :
			m_handle(mesh->Get(), physx::PxMeshGeometryFlags(), scale.x, scale.y, scale.z),
			m_sharedMesh(mesh)
		{
		}

		inline HeightFieldGeometry::HeightFieldGeometry(HeightFieldGeometry&& other) :
			m_handle{ std::move(other.m_handle) },
			m_sharedMesh{ std::move(other.m_sharedMesh) }
		{
		}

		inline HeightFieldGeometry::HeightFieldGeometry(const HeightFieldGeometry& other) :
			m_handle{ other.m_handle },
			m_sharedMesh{ other.m_sharedMesh }
		{
		}

		inline GeometryType HeightFieldGeometry::GetType() const
		{
			return GeometryType(m_handle.getType());
		}

		inline physx::PxGeometry& HeightFieldGeometry::Get()
		{
			return m_handle;
		}

		inline const physx::PxGeometry& HeightFieldGeometry::Get() const
		{
			return m_handle;
		}

		inline bool HeightFieldGeometry::IsValid() const
		{
			return m_handle.isValid();
		}

		inline float& HeightFieldGeometry::HeightScale()
		{
			return m_handle.heightScale;
		}

		inline float HeightFieldGeometry::HeightScale() const
		{
			return m_handle.heightScale;
		}

		inline float& HeightFieldGeometry::RowScale()
		{
			return m_handle.rowScale;
		}

		inline float HeightFieldGeometry::RowScale() const
		{
			return m_handle.rowScale;
		}

		inline float& HeightFieldGeometry::ColumnScale()
		{
			return m_handle.columnScale;

		}

		inline float HeightFieldGeometry::ColumnScale() const
		{
			return m_handle.columnScale;
		}

		inline std::shared_ptr<HeightField> HeightFieldGeometry::Mesh()
		{
			return m_sharedMesh;
		}

		inline const std::shared_ptr<HeightField> HeightFieldGeometry::Mesh() const
		{
			return m_sharedMesh;
		}

		inline HeightFieldGeometry& HeightFieldGeometry::operator=(HeightFieldGeometry&& rhs)
		{
			m_handle = std::move(rhs.m_handle);
			m_sharedMesh = std::move(rhs.m_sharedMesh);

			return *this;
		}

		inline HeightFieldGeometry& HeightFieldGeometry::operator=(const HeightFieldGeometry& rhs)
		{
			m_handle = rhs.m_handle;
			m_sharedMesh = rhs.m_sharedMesh;

			return *this;
		}
	}
}