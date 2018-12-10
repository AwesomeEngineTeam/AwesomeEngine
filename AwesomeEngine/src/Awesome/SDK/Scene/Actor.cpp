#include <Awesome/SDK/Scene/Actor.hpp>

#include <Awesome/SDK/Scene/SceneManager.hpp>

#include <Awesome/SDK/Components/Render/MeshRenderer.hpp>
#include <Awesome/SDK/Components/Lua/Script.hpp>
#include <Awesome/SDK/Components/Sound/SoundEmitter.hpp>

namespace aw
{
	namespace sdk
	{

#if WITH_EDITOR
		std::function<void(Actor* actor)> Actor::onAddComponent;
		std::function<void(Actor* actor)> Actor::onDestroyComponent;
#endif

		//bool Actor::GarbageCollectionRequired() const
		//{
		//	for (auto it = m_children.begin(); it != m_children.end(); ++it)
		//	{
		//		if ((*it)->GarbageCollectionRequired())
		//			return true;
		//	}

		//	return m_pendingGC;
		//}

		bool Actor::GarbageCollect()
		{
			//if (m_pendingGC == false)
			//	return false;

			if(m_root)
			{
				if (m_root->GarbageCollect())
				{
					delete m_root;
					m_root = nullptr;

				}
			}

			for (auto it = m_children.begin(); it != m_children.end(); ++it)
			{
				if ((*it)->GarbageCollect())
				{
					sdk::SceneManager::CurrentScene->RemoveActorImmediate(*it);
					break; //Lazy
				}
			}

			return m_pendingGC && m_children.empty() && m_root == nullptr;
		}

		void Actor::Destroy()
		{
			sdk::SceneManager::CurrentScene->RemoveActor(this);
		}

		bool Actor::RegisterComponentInScene(Component* comp)
		{
			return SceneManager::CurrentScene->RegisterComponent(comp);
		}

		//void Actor::UnregisterComponentInScene(Component* comp)
		//{
		//	SceneManager::CurrentScene->UnregisterComponent(comp);
		//}

		//void Actor::UnregisterRecur(Component* root)
		//{
		//	UnregisterComponentInScene(root);

		//	for (auto it = root->GetFirstChild(); it != root->GetLastChild(); it++)
		//		UnregisterRecur(*it);
		//}

		MeshRenderer* Actor::GetMesh()
		{
			return GetComponent<MeshRenderer>();
		}

		std::vector<MeshRenderer*> Actor::GetMeshes()
		{
			return GetComponents<MeshRenderer>();
		}

		Script* Actor::GetScript()
		{
			return GetComponent<Script>();
		}

		std::vector<Script*> Actor::GetScripts()
		{
			return GetComponents<Script>();
		}

		SoundEmitter* Actor::GetSoundEmitter()
		{
			return GetComponent<SoundEmitter>();
		}

		std::vector<SoundEmitter*> Actor::GetSoundEmitters()
		{
			return GetComponents<SoundEmitter>();
		}
	}

	namespace reflect
	{
		TypeDescriptor_ActorPtr::TypeDescriptor_ActorPtr() :
			TypeDescriptor{ "Actor*", sizeof(sdk::Actor*) },
			actorTypeDescriptor{ aw::reflect::TypeResolver<sdk::Actor>::Get() }
		{
		}

		void TypeDescriptor_ActorPtr::Dump(const void* obj, std::ostream& os) const
		{
			sdk::Actor* ptr = *(sdk::Actor**)(obj);

			if (ptr == nullptr)
				os << 'N';
			else
				actorTypeDescriptor->Dump(ptr, os);
		}

		void TypeDescriptor_ActorPtr::Undump(void* obj, std::istream& os) const {}

		template <>
		TypeDescriptor* GetPrimitiveDescriptor<sdk::Actor*>()
		{
			static TypeDescriptor_ActorPtr typeDesc;
			return &typeDesc;
		}
	}
}