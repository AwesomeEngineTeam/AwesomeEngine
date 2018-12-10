#pragma once

#ifndef AWESOME_PHYSICS_FOUNDATION_GUARD
#define AWESOME_PHYSICS_FOUNDATION_GUARD

#include <physX/foundation/PxFoundation.h>
#include <physX/foundation/PxFoundationVersion.h>

#include <Awesome/Physics/Config.hpp>
#include <Awesome/Physics/Callback/Debug.hpp>
#include <Awesome/Physics/Callback/Allocator.hpp>

namespace aw
{
	namespace ph
	{
		class Foundation
		{
			physx::PxFoundation* m_handle = nullptr;

		public:
			Foundation() = default;
			inline Foundation(AllocatorCallback& allocCallback, DebugCallback& debCallback);
			inline Foundation(Foundation&& other);
			Foundation(const Foundation&) = delete;
			inline ~Foundation();

			inline physx::PxFoundation* Get();
			inline const physx::PxFoundation* Get() const;

			AWESOME_PHYSICS_API void Create(AllocatorCallback& allocCallback, DebugCallback& debCallback);
			inline void Destroy();

			inline Foundation& operator=(Foundation&& rhs);
			Foundation& operator=(const Foundation&) = delete;
		};
	}
}

#include <Awesome/Physics/Control/Foundation.inl>

#endif // GUARD