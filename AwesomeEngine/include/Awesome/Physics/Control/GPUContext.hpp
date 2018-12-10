#pragma once

#ifndef AWESOME_PHYSICS_GPU_CONTEXT_GUARD
#define AWESOME_PHYSICS_GPU_CONTEXT_GUARD

#include <Awesome/Physics/Config.hpp>
#include <Awesome/Physics/Descriptor/GPUContextDescriptor.hpp>

namespace aw
{
	namespace ph
	{
		class GPUContext
		{
			physx::PxCudaContextManager* m_handle = nullptr;

		public:
			GPUContext() = default;
			inline GPUContext(const GPUContextDescriptor& desc);
			inline GPUContext(GPUContext&& other);
			GPUContext(const GPUContext&) = delete;
			inline ~GPUContext();

			inline physx::PxCudaContextManager* Get();
			inline const physx::PxCudaContextManager* Get() const;

			AWESOME_PHYSICS_API void Create(const GPUContextDescriptor& desc = GPUContextDescriptor());
			inline void Destroy();

			inline GPUContext& operator=(GPUContext&& rhs);
			GPUContext& operator=(const GPUContext&) = delete;
		};
	}
}

#include <Awesome/Physics/Control/GPUContext.inl>

#endif // GUARD