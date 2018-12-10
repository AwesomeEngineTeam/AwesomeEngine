#pragma once

#ifndef AWESOME_PHYSICS_COOKING_GUARD
#define AWESOME_PHYSICS_COOKING_GUARD

#include <physX/cooking/PxCooking.h>
#include <physX/PxPhysicsVersion.h>

#include <Awesome/Physics/Control/Foundation.hpp>
#include <Awesome/Physics/TolerancesScale.hpp>

namespace aw
{
	namespace ph
	{
		class Cooking
		{
			physx::PxCooking* m_handle;

		public:
			Cooking() = default;
			inline Cooking(Cooking&& other);
			Cooking(const Cooking&) = delete;
			inline ~Cooking();

			inline physx::PxCooking* Get();
			inline const physx::PxCooking* Get() const;

			AWESOME_PHYSICS_API void Create();
			inline void Destroy();

			inline Cooking& operator=(Cooking&& rhs);
			Cooking& operator=(const Cooking&) = delete;
		};
	}
}

#include <Awesome/Physics/Control/Cooking.inl>

#endif // GUARD