#include <Awesome/Physics/Callback/SimulationEvent.hpp>
#include <Awesome/Physics/Body/Rigidbody.hpp>

namespace aw
{
	namespace ph
	{
		std::function<void(void*, void*)> SimulationEvent::onCollisionCallback;
		std::function<void(void*, void*)> SimulationEvent::onTriggerCallback;

		void SimulationEvent::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
		{
			//LOG_WARNING("Physics: SimulationEvent::onConstraintBreak Not implemented!");
		}
		
		void SimulationEvent::onWake(physx::PxActor** actors, physx::PxU32 count)
		{
			//LOG_WARNING("Physics: SimulationEvent::onWake Not implemented!");
		}
		
		void SimulationEvent::onSleep(physx::PxActor** actors, physx::PxU32 count)
		{
			//LOG_WARNING("Physics: SimulationEvent::onSleep Not implemented!");
		}
		
		void SimulationEvent::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
		{
			for (uint32 i = 0u; i < nbPairs; i += 2)
			{
				// ignore pairs when shapes have been deleted
				if (pairs[i].events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
				{
					aw::sdk::PhysicalComponent* comp1 = reinterpret_cast<Rigidbody*>(pairHeader.actors[i]->userData)->GetUserData<aw::sdk::PhysicalComponent>();
					aw::sdk::PhysicalComponent* comp2 = reinterpret_cast<Rigidbody*>(pairHeader.actors[i + 1]->userData)->GetUserData<aw::sdk::PhysicalComponent>();

					if(onCollisionCallback)
						onCollisionCallback(comp1, comp2);
				}
			}
		}
		
		void SimulationEvent::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
		{
			for (uint32 i = 0u; i < count; i++)
			{
				// ignore pairs when shapes have been deleted
				if (pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
					continue;

				aw::sdk::PhysicalComponent* comp1 = reinterpret_cast<Rigidbody*>(pairs[i].triggerActor->userData)->GetUserData<aw::sdk::PhysicalComponent>();
				aw::sdk::PhysicalComponent* comp2 = reinterpret_cast<Rigidbody*>(pairs[i].otherActor->userData)->GetUserData<aw::sdk::PhysicalComponent>();

				if (onTriggerCallback)
					onTriggerCallback(comp1, comp2);
			}
		}
		
		void SimulationEvent::onAdvance(const physx::PxRigidBody*const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
		{
			//LOG_WARNING("Physics: SimulationEvent::onAdvance Not implemented!");
		}

		physx::PxFilterFlags SampleFilterShader(
			physx::PxFilterObjectAttributes attributes0,
			physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes attributes1,
			physx::PxFilterData filterData1,
			physx::PxPairFlags& pairFlags,
			const void* constantBlock,
			physx::PxU32 constantBlockSize)
		{


			// l: Layer / m: Mask
			// if (l1 & m2) && (l2 & m1) == collision !
			if (!((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))) // if no check
				return physx::PxFilterFlag::eKILL;

			// enable Discret or continuous Dynamic collision detection
			pairFlags = physx::PxPairFlag::eSOLVE_CONTACT | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT | physx::PxPairFlag::eDETECT_CCD_CONTACT;

			// let triggers through
			if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
				pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
			else // generate contacts for all that were not filtered above
				pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;

			return physx::PxFilterFlag::eDEFAULT;
		}
	}
}