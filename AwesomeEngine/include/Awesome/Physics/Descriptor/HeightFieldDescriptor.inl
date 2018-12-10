namespace aw
{
	namespace ph
	{
		inline HeightFieldDescriptor::HeightFieldDescriptor(HeightFieldDescriptor&& other) : m_handle{ std::move(other.m_handle) }
		{
		}

		inline HeightFieldDescriptor::HeightFieldDescriptor(const HeightFieldDescriptor& other) : m_handle{ other.m_handle }
		{
		}

		inline const physx::PxHeightFieldDesc& HeightFieldDescriptor::Get() const
		{
			return m_handle;
		}

		inline bool HeightFieldDescriptor::IsValid() const
		{
			return m_handle.isValid();
		}

		inline void HeightFieldDescriptor::Create(uint32 numCols, uint32 numRows, std::vector<HeightFieldSample> samples)
		{
			m_handle.format = physx::PxHeightFieldFormat::eS16_TM;
			m_handle.nbColumns = numCols;
			m_handle.nbRows = numRows;

			m_handle.samples.data = samples.data();
			m_handle.samples.stride = sizeof(HeightFieldSample);
		}

		inline HeightFieldDescriptor& HeightFieldDescriptor::operator=(HeightFieldDescriptor&& rhs)
		{
			m_handle = std::move(rhs.m_handle);

			return *this;
		}

		inline HeightFieldDescriptor& HeightFieldDescriptor::operator=(const HeightFieldDescriptor& rhs)
		{
			m_handle = rhs.m_handle;

			return *this;
		}
	}
}