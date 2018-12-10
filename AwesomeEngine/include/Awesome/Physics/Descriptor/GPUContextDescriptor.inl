namespace aw
{
	namespace ph
	{
		inline GPUContextDescriptor::GPUContextDescriptor(GPUContextDescriptor&& other) : m_handle{ std::move(other.m_handle) }
		{
		}

		inline GPUContextDescriptor::GPUContextDescriptor(const GPUContextDescriptor& other) : m_handle{ other.m_handle }
		{

		}

		inline physx::PxCudaContextManagerDesc GPUContextDescriptor::Get()
		{
			return m_handle;
		}

		inline const physx::PxCudaContextManagerDesc& GPUContextDescriptor::Get() const
		{
			return m_handle;
		}

		inline GPUContextDescriptor& GPUContextDescriptor::operator=(GPUContextDescriptor&& rhs)
		{
			m_handle = std::move(rhs.m_handle);

			return *this;
		}

		inline GPUContextDescriptor& GPUContextDescriptor::operator=(const GPUContextDescriptor& rhs)
		{
			m_handle = rhs.m_handle;

			return *this;
		}
	}
}