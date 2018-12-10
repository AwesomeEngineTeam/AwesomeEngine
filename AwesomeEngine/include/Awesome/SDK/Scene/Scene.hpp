#pragma once

#ifndef AWESOME_SDK_SCENE_GUARD
#define AWESOME_SDK_SCENE_GUARD

#include <functional>

#include <Awesome/SDK/Time.hpp>

//#include <Awesome/SDK/RaycastInfo.hpp>

//#include <Awesome/SDK/Scene/Actor.hpp>

#include <Awesome/SDK/System/RenderSystem.hpp>
#include <Awesome/SDK/System/PhysicalSystem.hpp>
#include <Awesome/SDK/System/ScriptSystem.hpp>
#include <Awesome/SDK/System/EmptySystem.hpp>
#include <Awesome/SDK/System/SoundSystem.hpp>

#include <Awesome/SDK/ThreadManager.hpp>

//#define MAX_POINT_LIGHTS_COUNT (10)

namespace aw
{
	namespace sdk
	{
		class Scene
		{
			std::string m_name{"New Scene"};
			std::unique_ptr<Actor> m_root = std::make_unique<Actor>("Root");


		public:
			inline void RemoveActorImmediate(Actor* actor);

#if WITH_EDITOR
			AWESOME_SDK_API static std::function<void(Scene*)> onSceneStart;
			AWESOME_SDK_API static std::function<void(Actor*, Actor*)> onCreateActor;
			AWESOME_SDK_API static std::function<void(Actor*)> onDestroyActor;
#endif

			//Scene(BufferManager::BufferMemoryPool* pool) : m_renderSystem{MAX_MODELS_COUNT, MAX_POINT_LIGHTS_COUNT, pool} { m_renderSystem.Init(); };
			Scene() = default;
			inline Scene(const std::string& n);
			Scene(const Scene&) = delete;
			Scene(Scene&&) = delete;
			inline ~Scene();

			inline const std::string& Name() const { return m_name; }

			inline std::list<Actor*>::iterator GetFirstActor();
			inline std::list<Actor*>::iterator GetLastActor();

			template<typename T = Actor>
			inline T* CreateActor(Actor* parent = nullptr);

			template<typename T = Actor, typename... Args>
			inline T* CreateActor(Actor* parent, Args&&... args);

			inline void RemoveActor(Actor* actor);

			inline Actor* FindActorById(uint32 id);
			inline Actor* FindActorWithName(const std::string& name);
			inline std::vector<Actor*> FindActorsWithName(const std::string& name);

			AWESOME_SDK_API void GarbageCollect();

			inline bool RegisterComponent(Component* comp);
			//inline void UnregisterComponent(Component* comp);

			inline Scene& operator=(Scene&&) = delete;
			inline Scene& operator=(const Scene&) = delete;

			REFLECT_BEGIN(Scene)
				REFLECT_MEMBER(m_root)
			REFLECT_END()
		};
	}
}

#include <Awesome/SDK/Scene/Scene.inl>

#endif // GUARD