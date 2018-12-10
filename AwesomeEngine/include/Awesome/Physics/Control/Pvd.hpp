#pragma once

#ifndef AWESOME_PHYSICS_PVD_GUARD
#define AWESOME_PHYSICS_PVD_GUARD

#include <physX/pvd/PxPvd.h>

#include <Awesome/Physics/Config.hpp>

namespace aw
{
	namespace ph
	{
		class Pvd
		{
			physx::PxPvd* m_handle = nullptr;

		public:
			Pvd() = default;
			inline Pvd(Pvd&& other);
			Pvd(const Pvd&) = delete;
			inline ~Pvd();

			inline physx::PxPvd* Get();
			inline const physx::PxPvd* Get() const;

			AWESOME_PHYSICS_API void Create();
			inline void Destroy();

			inline Pvd& operator=(Pvd&& rhs);
			Pvd& operator=(const Pvd&) = delete;
		};
	}
}

#include <Awesome/Physics/Control/Pvd.inl>

#endif // GUARD