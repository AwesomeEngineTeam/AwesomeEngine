#pragma once

#ifndef AWESOME_SDK_ACTOR_GUARD
#define AWESOME_SDK_ACTOR_GUARD

#include <Awesome/SDK/Config.hpp>
#include <Awesome/SDK/Components/Component.hpp>

#include <functional>

namespace aw
{
	namespace sdk
	{
		class Actor : public SceneNodeTemplate<Actor>
		{
		protected:
			Component* m_root = nullptr;

			std::atomic_bool m_pendingGC = false;

		public:
#if WITH_EDITOR
			AWESOME_SDK_API static std::function<void(Actor* actor)> onAddComponent;
			AWESOME_SDK_API static std::function<void(Actor* actor)> onDestroyComponent;
#endif

			inline explicit Actor(const std::string& name = defaultName);
			Actor(const Actor&) = delete;
			Actor(Actor&&) = delete;
			inline ~Actor() override;

			inline Component* GetRootComponent();
			inline const Component* GetRootComponent() const;

			template <typename T>
			inline T* GetComponent();
			template <typename T>
			inline std::vector<T*> GetComponents();

			class MeshRenderer* GetMesh();
			std::vector<class MeshRenderer*> GetMeshes();

			class Script* GetScript();
			std::vector<class Script*> GetScripts();

			class SoundEmitter* GetSoundEmitter();
			std::vector<class SoundEmitter*> GetSoundEmitters();

		private:
			template <typename T>
			inline Component* GetComponent(Component* root);
			template <typename T>
			inline void GetComponent(Component* root, std::vector<T*>& buff);

		public:
			inline Component* FindComponentById(uint32 id) const;
			inline Component* FindComponentWithName(const std::string& str) const;
			inline std::vector<Component*> FindComponentsWithName(const std::string& str) const;

			template <typename T>
			inline T* AddComponent(Component* parent = nullptr);
			template <typename T, typename... Args>
			inline T* AddComponent(Component* parent, Args&&... args);
			inline void RemoveComponent(Component* comp);

			//AWESOME_SDK_API bool GarbageCollectionRequired() const;
			AWESOME_SDK_API bool GarbageCollect();
			//AWESOME_SDK_API bool GarbageCollect();

			inline void MarkForGarbageCollection();

			AWESOME_SDK_API void Destroy();

		private:
			template <typename T>
			T * RegisterComponentInActor(T* newComp, Component* parent);
			bool RegisterComponentInScene(Component*);
			//void UnregisterComponentInScene(Component*);
			//void UnregisterRecur(Component* root);

		public:
			inline void SetLocalPosition(const Vector3& position);
			inline void SetLocalRotation(const Quaternion& rotation);
			inline void SetGlobalPosition(const Vector3& position);
			inline void SetGlobalRotation(const Quaternion& rotation);
			inline void SetScale(float scale);
			inline void LookAt(const Vector3& target, const Vector3& up = Vector3::Up());
			inline void Rotate_WorldSpace(const Quaternion& rotation);
			inline void Rotate_SelfSpace(const Quaternion& rotation);
			inline void Rotate_WorldSpace(const Vector3& eulerAngles);
			inline void Rotate_SelfSpace(const Vector3& eulerAngles);
			inline void RotateAround(const Vector3& point, const Vector3& axis, float angle);
			inline void Translate_WorldSpace(const Vector3& translationVector);
			inline void Translate_SelfSpace(const Vector3& translationVector);

			inline Actor& operator=(const Actor&) = delete;
			inline Actor& operator=(Actor&&) = delete;

			REFLECT_BEGIN(Actor)
				REFLECT_MEMBER_INHERITANCE(SceneNodeTemplate<Actor>)
				REFLECT_MEMBER(m_root)
			REFLECT_END()
		};
	}

	namespace reflect
	{
		struct TypeDescriptor_ActorPtr : TypeDescriptor
		{
			TypeDescriptor* actorTypeDescriptor;

			TypeDescriptor_ActorPtr();
			void Dump(const void* obj, std::ostream& os = std::cout) const override;
			void Undump(void* obj, std::istream& os = std::cin) const override;
		};
	}
}

#include <Awesome/SDK/Scene/Actor.inl>

#endif // GUARD