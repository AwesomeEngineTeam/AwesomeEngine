#pragma once

#ifndef AWESOME_PHYSICS_DEBUG_GUARD
#define AWESOME_PHYSICS_DEBUG_GUARD

#include <physX/extensions/PxDefaultErrorCallback.h>

#include <Awesome/Core/Debug.hpp>

namespace aw
{
	namespace ph
	{
		class DebugCallback : public physx::PxErrorCallback
		{
		public:
			DebugCallback() = default;
			~DebugCallback() = default;

			inline virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;
		};
	}
}

#include <Awesome/Physics/Callback/Debug.inl>

#endif // GUARD