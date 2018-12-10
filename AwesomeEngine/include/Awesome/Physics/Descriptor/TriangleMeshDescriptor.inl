namespace aw
{
	namespace ph
	{
		inline TriangleMeshDescriptor::TriangleMeshDescriptor(const std::vector<Vector3>& vertices, const std::vector<uint32>& indices)
		{
			Create(vertices, indices);
		}

		inline TriangleMeshDescriptor::TriangleMeshDescriptor(TriangleMeshDescriptor&& other) : m_handle{ std::move(other.m_handle) }
		{
		}

		inline TriangleMeshDescriptor::TriangleMeshDescriptor(const TriangleMeshDescriptor& other) : m_handle{ other.m_handle }
		{
		}

		inline const physx::PxTriangleMeshDesc& TriangleMeshDescriptor::Get() const
		{
			return m_handle;
		}

		inline bool TriangleMeshDescriptor::IsValid()
		{
			return m_handle.isValid();
		}

		inline void TriangleMeshDescriptor::Create(const std::vector<Vector3>& vertices, const std::vector<uint32>& indices)
		{
			m_handle.points.count = uint32(vertices.size());
			m_handle.points.stride = sizeof(Vector3);
			m_handle.points.data = vertices.data();

			m_handle.triangles.count = uint32(indices.size());
			m_handle.triangles.stride = 3 * sizeof(uint32);
			m_handle.triangles.data = indices.data();
		}

		inline TriangleMeshDescriptor& TriangleMeshDescriptor::operator=(TriangleMeshDescriptor&& rhs)
		{
			m_handle = std::move(rhs.m_handle);

			return *this;
		}

		inline TriangleMeshDescriptor& TriangleMeshDescriptor::operator=(const TriangleMeshDescriptor& rhs)
		{
			m_handle = rhs.m_handle;

			return *this;
		}
	}
}