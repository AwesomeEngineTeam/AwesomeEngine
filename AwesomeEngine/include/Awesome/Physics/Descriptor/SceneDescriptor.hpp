#pragma once

#ifndef AWESOME_PHYSICS_SCENE_DESCRIPTOR_GUARD
#define AWESOME_PHYSICS_SCENE_DESCRIPTOR_GUARD

#include <physX/PxSceneDesc.h>

#include <Awesome/Physics/Callback/SimulationEvent.hpp>

#include <Awesome/Maths/Vector3.hpp>

#include <Awesome/Physics/Config.hpp>

namespace aw
{
	namespace ph
	{
		class SceneDescriptor
		{
			physx::PxSceneDesc m_handle;

			AWESOME_PHYSICS_API static SimulationEvent s_simulationEventCallback;

			AWESOME_PHYSICS_API void Create(const Vector3& gravity, bool useGPU);
			inline void Destroy();

		public:
			AWESOME_PHYSICS_API SceneDescriptor(const Vector3& gravity = Vector3(0.0f, -9.81f, 0.0f), bool useGPU = true);
			inline SceneDescriptor(SceneDescriptor&& other);
			inline SceneDescriptor(const SceneDescriptor& other);
			inline ~SceneDescriptor();

			inline physx::PxSceneDesc& Get();
			inline const physx::PxSceneDesc& Get() const;

			inline bool IsValid() const;

			inline SceneDescriptor& operator=(SceneDescriptor&& rhs);
			inline SceneDescriptor& operator=(const SceneDescriptor& rhs);
		};
	}
}

#include <Awesome/Physics/Descriptor/SceneDescriptor.inl>

#endif // GUARD