#pragma once

#ifndef AWESOME_PHYSICS_ACTOR_GUARD
#define AWESOME_PHYSICS_ACTOR_GUARD

#include <physX/PxActor.h>

namespace aw
{
	namespace ph
	{
		class Actor
		{
		public:
			Actor() = default;
			virtual ~Actor() = default;

			inline virtual physx::PxActor* GetAsActor() = 0;
			inline virtual const physx::PxActor* GetAsActor() const = 0;
		};
	}
}

#endif // GUARD