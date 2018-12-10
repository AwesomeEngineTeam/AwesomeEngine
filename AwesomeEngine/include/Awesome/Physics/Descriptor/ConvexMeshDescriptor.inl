namespace aw
{
	namespace ph
	{
		inline uint16 operator|(ConvexFlag flag1, ConvexFlag flag2)
		{
			return static_cast<uint16>(flag1) | static_cast<uint16>(flag2);
		}

		inline ConvexMeshDescriptor::ConvexMeshDescriptor(const std::vector<Vector3>& vertices, const std::vector<uint32>& indices, uint16 flags)
		{
			Create(vertices, indices, flags);
		}

		inline ConvexMeshDescriptor::ConvexMeshDescriptor(ConvexMeshDescriptor&& other) : m_handle {std::move(other.m_handle)}
		{
		}

		inline ConvexMeshDescriptor::ConvexMeshDescriptor(const ConvexMeshDescriptor& other) : m_handle{other.m_handle }
		{
		}

		inline const physx::PxConvexMeshDesc& ConvexMeshDescriptor::Get() const
		{
			return m_handle;
		}

		inline bool ConvexMeshDescriptor::IsValid() const
		{
			return m_handle.isValid();
		}

		inline const std::vector<Vector3>& ConvexMeshDescriptor::Vertices() const
		{
			return reinterpret_cast<const std::vector<Vector3>&>(*reinterpret_cast<const Vector3*>(m_handle.points.data));
		}

		inline const std::vector<float>& ConvexMeshDescriptor::Indices() const
		{
			return reinterpret_cast<const std::vector<float>&>(*reinterpret_cast<const float*>(m_handle.points.data));
		}

		inline ConvexFlag& ConvexMeshDescriptor::Flags()
		{
			return reinterpret_cast<ConvexFlag&>(m_handle.flags);
		}

		inline const ConvexFlag& ConvexMeshDescriptor::Flags() const
		{
			return reinterpret_cast<const ConvexFlag&>(m_handle.flags);
		}

		inline void ConvexMeshDescriptor::Create(const std::vector<Vector3>& vertices, const std::vector<uint32>& indices, uint16 flags)
		{
			m_handle.points.count = uint32(vertices.size());
			m_handle.points.stride = sizeof(Vector3);
			m_handle.points.data = vertices.data();

			m_handle.indices.count = uint32(indices.size());
			m_handle.indices.stride = 3 * sizeof(uint32);
			m_handle.indices.data = indices.data();

			m_handle.flags = physx::PxConvexFlag::Enum(flags);
		}

		inline ConvexMeshDescriptor& ConvexMeshDescriptor::operator=(ConvexMeshDescriptor&& rhs)
		{
			m_handle = std::move(rhs.m_handle);

			return *this;
		}

		inline ConvexMeshDescriptor& ConvexMeshDescriptor::operator=(const ConvexMeshDescriptor& rhs)
		{
			m_handle = rhs.m_handle;

			return *this;
		}
	}
}