#pragma once

#ifndef AWESOME_PHYSICS_RIGIDBODY_STATIC_GUARD
#define AWESOME_PHYSICS_RIGIDBODY_STATIC_GUARD

#include <physX/PxRigidStatic.h>

#include <Awesome/Physics/Body/Rigidbody.hpp>

namespace aw
{
	namespace ph
	{
		class RigidbodyStatic : public Rigidbody
		{
		public:
			RigidbodyStatic() = default;
			inline RigidbodyStatic(const TransformBase& tr, void* userData);
			inline RigidbodyStatic(const TransformBase& tr, const Geometry& geo, const Material& mat, void* userData);
			inline RigidbodyStatic(RigidbodyStatic&& other);
			RigidbodyStatic(const RigidbodyStatic&) = delete;
			~RigidbodyStatic() = default;

			AWESOME_PHYSICS_API void Create(const TransformBase& tr, void* userData) override;

			inline RigidbodyStatic& operator=(RigidbodyStatic&& rhs);
			RigidbodyStatic& operator=(const RigidbodyStatic& rhs) = delete;
		};
	}
}

#include <Awesome/Physics/Body/RigidbodyStatic.inl>

#endif // GUARD