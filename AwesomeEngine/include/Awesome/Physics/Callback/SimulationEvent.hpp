#pragma once

#ifndef AWESOME_PHYSICS_SIMULATION_EVENT_GUARD
#define AWESOME_PHYSICS_SIMULATION_EVENT_GUARD

#include <physX/PxSimulationEventCallback.h>
#include <physX/extensions/PxDefaultSimulationFilterShader.h>


#include <Awesome/Core/Debug.hpp>
#include <Awesome/Physics/Config.hpp>

namespace aw
{
	namespace ph
	{

		class SimulationEvent : public physx::PxSimulationEventCallback
		{
		public:
			AWESOME_PHYSICS_API static std::function<void(void*, void*)> onCollisionCallback;
			AWESOME_PHYSICS_API static std::function<void(void*, void*)> onTriggerCallback;

			SimulationEvent() = default;
			~SimulationEvent() override = default;

			AWESOME_PHYSICS_API void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
			AWESOME_PHYSICS_API void onWake(physx::PxActor** actors, physx::PxU32 count) override;
			AWESOME_PHYSICS_API void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
			AWESOME_PHYSICS_API void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
			AWESOME_PHYSICS_API void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
			AWESOME_PHYSICS_API void onAdvance(const physx::PxRigidBody*const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;
		};

		AWESOME_PHYSICS_API physx::PxFilterFlags SampleFilterShader(
			physx::PxFilterObjectAttributes attributes0,
			physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes attributes1,
			physx::PxFilterData filterData1,
			physx::PxPairFlags& pairFlags,
			const void* constantBlock,
			physx::PxU32 constantBlockSize);
	}
}

#endif // GUARD