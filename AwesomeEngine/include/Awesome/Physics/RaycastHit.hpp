#pragma once

#ifndef AWEOME_PHYSICS_RAYCAST_HIT_GUARD
#define AWEOME_PHYSICS_RAYCAST_HIT_GUARD

#include <Awesome/Physics/Body/Rigidbody.hpp>

namespace aw
{
	namespace ph
	{
		class RaycastHit
		{
		public:
			Rigidbody* body = nullptr;

			float distance = 0.0f;

			Vector3 position;
			Vector3 normal;

			RaycastHit() = default;
			inline RaycastHit(Rigidbody* _body, float dist, const Vector3& pos, const Vector3& norm);
			inline RaycastHit(RaycastHit&& other);
			inline RaycastHit(const RaycastHit& other);
			~RaycastHit() = default;

			inline RaycastHit& operator=(RaycastHit&& rhs);
			inline RaycastHit& operator=(const RaycastHit& rhs);
		};
	}
}

#include <Awesome/Physics/RaycastHit.inl>

#endif // GUARD