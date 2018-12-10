#pragma once

#ifndef AWESOME_PHYSICS_GPU_CONTEXT_DESCRIPTOR_GUARD
#define AWESOME_PHYSICS_GPU_CONTEXT_DESCRIPTOR_GUARD

#include <utility>

#include <physX/cudamanager/PxCudaContextManager.h>

namespace aw
{
	namespace ph
	{
		class GPUContextDescriptor
		{
			physx::PxCudaContextManagerDesc m_handle;

		public:
			GPUContextDescriptor() = default;
			inline GPUContextDescriptor(GPUContextDescriptor&& other);
			inline GPUContextDescriptor(const GPUContextDescriptor& other);
			~GPUContextDescriptor() = default;

			inline physx::PxCudaContextManagerDesc Get();
			inline const physx::PxCudaContextManagerDesc& Get() const;

			inline GPUContextDescriptor& operator=(GPUContextDescriptor&& rhs);
			inline GPUContextDescriptor& operator=(const GPUContextDescriptor& rhs);
		};
	}
}

#include <Awesome/Physics/Descriptor/GPUContextDescriptor.inl>

#endif // GUARD