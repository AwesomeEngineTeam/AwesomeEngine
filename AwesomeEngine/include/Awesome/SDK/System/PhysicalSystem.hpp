#pragma once

#ifndef AWESOME_SDK_PHYSICAL_SYSTEM_GUARD
#define AWESOME_SDK_PHYSICAL_SYSTEM_GUARD

#include <Awesome/Physics/Control/Context.hpp>
#include <Awesome/Physics/Scene/Scene.hpp>

#include <Awesome/SDK/Scene/Actor.hpp>
#include <Awesome/SDK/Components/Physical/Rigidbody.hpp>
#include <Awesome/SDK/Components/Physical/Collider.hpp>

#include <Awesome/SDK/RaycastInfo.hpp>

namespace aw
{
	namespace sdk
	{
		class PhysicalSystem
		{
			static ph::Scene s_scene;

			static std::vector<PhysicalComponent*> s_phComps;
			static std::vector<PhysicalComponent*> s_compsToInstantiate;
			
			static std::mutex s_instMutex;
			static std::mutex s_updateMutex;

			static AWESOME_SDK_API void GarbageCollect();
			static AWESOME_SDK_API void InstantiatePendingComponents();

		public:
			static constexpr const float s_fixedInterval = 0.016f;

			PhysicalSystem() = delete;
			inline ~PhysicalSystem();

			static inline const ph::Scene& GetPhysicalScene();

			static inline void Init();
			static inline void UnInit();

			static AWESOME_SDK_API void Start(const std::atomic_bool& shouldUpdate);

			static inline bool Register(Component* component);
			//static inline void Unregister(Component* component);
			static AWESOME_SDK_API void Update(float frameTime);

			static AWESOME_SDK_API RaycastInfo Raycast(const Vector3& origin, const Vector3& dir, float maxDist);
			static AWESOME_SDK_API RaycastInfo ScreenRaycast(class Camera& cam, const Vector2& screenPos, float maxDist);
		};
	}
}

#include <Awesome/SDK/System/PhysicalSystem.inl>

#endif // GUARD