#pragma once

#ifndef AWESOME_PHYSICS_INSTANCE_GUARD
#define AWESOME_PHYSICS_INSTANCE_GUARD

#include <physX/PxPhysics.h>
#include <physX/PxPhysicsVersion.h>

#include <Awesome/Physics/Config.hpp>
#include <Awesome/Physics/TolerancesScale.hpp>

namespace aw
{
	namespace ph
	{
		class Instance
		{
			physx::PxPhysics* m_handle = nullptr;

		public:
			Instance() = default;
			inline Instance(TolerancesScale scale);
			inline Instance(Instance&& other);
			Instance(const Instance&) = delete;
			inline ~Instance();

			inline physx::PxPhysics* Get();
			inline const physx::PxPhysics* Get() const;
			inline TolerancesScale GetTolerancesScale() const;

			AWESOME_PHYSICS_API void Create(TolerancesScale scale = TolerancesScale());
			inline void Destroy();

			inline Instance& operator=(Instance&& rhs);
			Instance& operator=(const Instance&) = delete;
		};
	}
}

#include <Awesome/Physics/Control/Instance.inl>

#endif // GUARD