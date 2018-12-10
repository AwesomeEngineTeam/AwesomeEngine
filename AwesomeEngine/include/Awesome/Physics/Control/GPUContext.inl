namespace aw
{
	namespace ph
	{
		inline GPUContext::GPUContext(const GPUContextDescriptor& desc)
		{
			Create(desc);
		}

		inline GPUContext::GPUContext(GPUContext&& other) : m_handle{ other.m_handle }
		{
			other.m_handle = nullptr;
		}

		inline GPUContext::~GPUContext()
		{
			Destroy();
		}

		inline physx::PxCudaContextManager* GPUContext::Get()
		{
			return m_handle;
		}

		inline const physx::PxCudaContextManager* GPUContext::Get() const
		{
			return m_handle;
		}

		inline void GPUContext::Destroy()
		{
			if (m_handle)
				m_handle->release();

			m_handle = nullptr;
		}

		inline GPUContext& GPUContext::operator=(GPUContext&& rhs)
		{
			Destroy();

			m_handle = rhs.m_handle;
			rhs.m_handle = nullptr;

			return *this;
		}
	}
}