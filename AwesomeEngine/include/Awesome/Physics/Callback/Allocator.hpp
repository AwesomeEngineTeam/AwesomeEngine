#pragma once

#ifndef AWESOME_PHYSICS_ALLOCATOR_GUARD
#define AWESOME_PHYSICS_ALLOCATOR_GUARD

#include <physX/extensions/PxDefaultAllocator.h>

#include <Awesome/Core/Debug.hpp>

namespace aw
{
	namespace ph
	{
		class AllocatorCallback : public physx::PxAllocatorCallback
		{
		public:
			AllocatorCallback() = default;
			virtual ~AllocatorCallback() = default;

			//Doesn't use our standard nomenclature due to override.
			inline virtual void* allocate(size_t size, const char* typeName, const char* filename,
				int line) override;
			inline virtual void deallocate(void* ptr) override;
		};
	}
}

#include <Awesome/Physics/Callback/Allocator.inl>	

#endif // GUARD