#pragma once

#ifndef AWESOME_PHYSICS_SCENE_GUARD
#define AWESOME_PHYSICS_SCENE_GUARD

#include <physX/PxScene.h>

#include <Awesome/Physics/Scene/Actor.hpp>
#include <Awesome/Physics/Descriptor/SceneDescriptor.hpp>
#include <Awesome/Physics/RaycastHit.hpp>

namespace aw
{
	namespace ph
	{
		class Context;

		class Scene
		{
			physx::PxScene* m_handle = nullptr;

		public:
			Scene() = default;
			inline Scene(const SceneDescriptor& sceneDesc);
			inline Scene(Scene&& other);
			Scene(const Scene&) = delete;
			inline ~Scene();

			AWESOME_PHYSICS_API void Create(const SceneDescriptor& sceneDesc = SceneDescriptor());
			inline void Destroy();

			inline void AddActor(Actor& actor);
			inline void RemoveActor(Actor& actor);

			inline void Simulate(float elapsedTime);
			inline bool FetchResult(bool block = false);

			inline physx::PxScene& GetHandle();

			AWESOME_PHYSICS_API RaycastHit Raycast(const Vector3& origin, const Vector3& dir, float maxDist) const;

			inline Scene& operator=(Scene&& rhs);
			Scene& operator=(const Scene&) = delete;
		};
	}
}

#include <Awesome/Physics/Scene/Scene.inl>

#endif // GUARD