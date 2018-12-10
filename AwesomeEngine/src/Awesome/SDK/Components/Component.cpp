#include <Awesome/SDK/Components/Component.hpp>

#include <Awesome/SDK/Scene/Actor.hpp>

#include <Awesome/SDK/Components/Lua/Script.hpp>

#include <Awesome/SDK/Components/Physical/Rigidbody.hpp>
#include <Awesome/SDK/Components/Physical/BoxCollider.hpp>
#include <Awesome/SDK/Components/Physical/PlaneCollider.hpp>
#include <Awesome/SDK/Components/Physical/SphereCollider.hpp>
#include <Awesome/SDK/Components/Physical/CapsuleCollider.hpp>
#include <Awesome/SDK/Components/Physical/HeightFieldCollider.hpp>

#include <Awesome/SDK/Components/Render/Camera.hpp>
#include <Awesome/SDK/Components/Render/Text2D.hpp>
#include <Awesome/SDK/Components/Render/MeshRenderer.hpp>
#include <Awesome/SDK/Components/Render/PointLight.hpp>
#include <Awesome/SDK/Components/Render/DirectionalLight.hpp>
#include <Awesome/SDK/Components/Render/Billboard.hpp>
#include <Awesome/SDK/Components/Render/Skybox.hpp>

#include <Awesome/SDK/Components/Sound/SoundEmitter.hpp>
#include <Awesome/SDK/Components/Sound/SoundListener.hpp>

namespace aw
{
	namespace sdk
	{
		Component::Component(Actor* actor, uint32 _typeId, const std::string& name, const UniformTransform& localTransform) :
			SceneNodeTemplate(name, true),
			typeId{ _typeId },
			m_actor{ actor },
			m_localTransform{ localTransform }
		{
			m_trMutex.lock();
			CalculateGlobalTransform();
			m_trMutex.unlock();
		}

		//bool Component::GarbageCollect()
		//{
		//	if (m_isUsed == true)
		//		return false;

		//	for (auto it = m_children.begin(); it != m_children.end(); ++it)
		//	{
		//		if (!(*it)->GarbageCollectable())
		//			return false;
		//	}

		//	return true;
		//}

		bool Component::GarbageCollect()
		{
			//if (m_pendingGC == false)
			//	return false;

			for (auto it = m_children.begin(); it != m_children.end(); ++it)
			{
				if ((*it)->GarbageCollect())
				{
					RemoveChild(*it);
					break; //Lazy
				}
			}

			return !m_isUsed && m_children.empty();
		}

		bool Component::GarbageCollectionRequired() const
		{
			return m_pendingGC;
		}

		void Component::Destroy()
		{
			m_actor->RemoveComponent(this);
		}

		reflect::TypeDescriptor* Component::ComponentTypeDescriptor()
		{
			return TypeIDToTypeDescriptor(typeId);
		}

		bool Component::IsActive() const
		{
			return SceneNodeTemplate::IsActive() && m_actor && m_actor->IsActive();
		}

		void Component::SetChildHierarchyDirty()
		{
			if (m_parent == nullptr)
			{
				if (m_actor)
				{
					const Actor& actor = *m_actor;

					std::list<Actor*>::const_iterator endChild = actor.GetLastChild();

					for (std::list<Actor*>::const_iterator node = actor.GetFirstChild(); node != endChild; node++)
					{
						Component* root = (*node)->GetRootComponent();

						if (root)
							root->SetHierarchyDirty();
					}
				}
			}

			for (Component* node : m_children)
				node->SetHierarchyDirty();
		}

		void Component::SetHierarchyDirty()
		{
			if (m_isHierarchyDirty)
				return;

			m_isHierarchyDirty = true;

			for (Component* node : m_children)
				node->SetHierarchyDirty();
		}

		UniformTransform Component::GetHierarchyGlobalTransform()
		{
			if (m_parent)
			{
				if (m_parent->m_isHierarchyDirty)
					m_parent->CalculateGlobalTransform();

				return m_parent->m_globalTransform;
			}

			if (m_actor && m_actor->GetParent() && m_actor->GetParent()->GetRootComponent())
			{
				Component* root = m_actor->GetParent()->GetRootComponent();

				if (root->m_isHierarchyDirty)
					root->CalculateGlobalTransform();

				return root->m_globalTransform;
			}

			return UniformTransform::Identity();
		}

		void Component::CalculateGlobalTransform()
		{
			#if defined(WITH_EDITOR)
			if (IsActive() || isImplicit)
			#else
			if (IsActive())
			#endif
			{
				m_globalTransform = GetHierarchyGlobalTransform() * m_localTransform;
				m_isHierarchyDirty = false;
			}
		}

		const std::string& Component::GetComponentTypeName() const
		{
			if (typeId == static_cast<uint32>(ComponentID::MeshRenderer))
			{
				static std::string str3 = "MeshRenderer";
				return str3;
			}
			else if (typeId == static_cast<uint32>(ComponentID::BoxCollider))
			{
				static std::string str7 = "BoxCollider";
				return str7;
			}
			else if (typeId == static_cast<uint32>(ComponentID::SphereCollider))
			{
				static std::string str8 = "SphereCollider";
				return str8;
			}
			else if (typeId == static_cast<uint32>(ComponentID::Script))
			{
				static std::string str13 = "Script";
				return str13;
			}
			else if (typeId == static_cast<uint32>(ComponentID::Camera))
			{
				static std::string str2 = "Camera";
				return str2;
			}
			else if (typeId == static_cast<uint32>(ComponentID::PointLight))
			{
				static std::string str4 = "PointLight";
				return str4;
			}
			if (typeId == static_cast<uint32>(ComponentID::Component))
			{
				static std::string str1 = "Component";
				return str1;
			}
			else if (typeId == static_cast<uint32>(ComponentID::Rigidbody))
			{
				static std::string str6 = "Rigidbody";
				return str6;
			}
			else if (typeId == static_cast<uint32>(ComponentID::CapsuleCollider))
			{
				static std::string str9 = "CapsuleCollider";
				return str9;
			}
			else if (typeId == static_cast<uint32>(ComponentID::Text2D))
			{
				static std::string str15 = "Text2D";
				return str15;
			}
			else if (typeId == static_cast<uint32>(ComponentID::SoundEmitter))
			{
				static std::string str14 = "SoundEmitter";
				return str14;
			}
			else if (typeId == static_cast<uint32>(ComponentID::SoundListener))
			{
				static std::string str15 = "SoundListener";
				return str15;
			}
			else if (typeId == static_cast<uint32>(ComponentID::PlaneCollider))
			{
				static std::string str13 = "PlaneCollider";
				return str13;
			}
			else if (typeId == static_cast<uint32>(ComponentID::Skybox))
			{
				static std::string str12 = "Skybox";
				return str12;
			}
			else if (typeId == static_cast<uint32>(ComponentID::DirectionalLight))
			{
				static std::string str5 = "DirectionalLight";
				return str5;
			}
			else if (typeId == static_cast<uint32>(ComponentID::MeshCollider))
			{
				static std::string str10 = "MeshCollider";
				return str10;
			}
			else if (typeId == static_cast<uint32>(ComponentID::HeightField))
			{
				static std::string str11 = "HeightField";
				return str11;
			}
			else
			{
				static std::string str00 = "Unknown";
				return str00;
			}
		}

		reflect::TypeDescriptor* Component::TypeIDToTypeDescriptor(uint32 id)
		{
			switch (id)
			{
			case TypeInfo<aw::sdk::Component>::id:
				return aw::reflect::TypeResolver<aw::sdk::Component>::Get();
			case TypeInfo<aw::sdk::Camera>::id:
				return aw::reflect::TypeResolver<aw::sdk::Camera>::Get();
			case TypeInfo<aw::sdk::MeshRenderer>::id:
				return aw::reflect::TypeResolver<aw::sdk::MeshRenderer>::Get();
			case TypeInfo<aw::sdk::PointLight>::id:
				return aw::reflect::TypeResolver<aw::sdk::PointLight>::Get();
			case TypeInfo<aw::sdk::DirectionalLight>::id:
				return aw::reflect::TypeResolver<aw::sdk::DirectionalLight>::Get();
			case TypeInfo<aw::sdk::Rigidbody>::id:
				return aw::reflect::TypeResolver<aw::sdk::Rigidbody>::Get();
			case TypeInfo<aw::sdk::PlaneCollider>::id:
				return aw::reflect::TypeResolver<aw::sdk::PlaneCollider>::Get();
			case TypeInfo<aw::sdk::BoxCollider>::id:
				return aw::reflect::TypeResolver<aw::sdk::BoxCollider>::Get();
			case TypeInfo<aw::sdk::SphereCollider>::id:
				return aw::reflect::TypeResolver<aw::sdk::SphereCollider>::Get();
			case TypeInfo<aw::sdk::CapsuleCollider>::id:
				return aw::reflect::TypeResolver<aw::sdk::CapsuleCollider>::Get();
			//case TypeInfo<aw::sdk::MeshCollider>::id:
				//return aw::reflect::TypeResolver<aw::sdk::MeshCollider>::Get();
			//case TypeInfo<aw::sdk::HeightField>::id:
				//return aw::reflect::TypeResolver<aw::sdk::HeightField>::Get();
			case TypeInfo<aw::sdk::Skybox>::id:
				return nullptr;
				//return aw::reflect::TypeResolver<aw::sdk::Skybox>::Get();
			case TypeInfo<aw::sdk::Script>::id:
				return aw::reflect::TypeResolver<aw::sdk::Script>::Get();
			case TypeInfo<aw::sdk::SoundListener>::id:
				return aw::reflect::TypeResolver<aw::sdk::SoundListener>::Get();
			case TypeInfo<aw::sdk::SoundEmitter>::id:
				return aw::reflect::TypeResolver<aw::sdk::SoundEmitter>::Get();
			case TypeInfo<aw::sdk::Text2D>::id:
				return aw::reflect::TypeResolver<aw::sdk::Text2D>::Get();
			}
		}
	}

	namespace reflect
	{
		TypeDescriptor_ComponenentPtr::TypeDescriptor_ComponenentPtr() : 
			TypeDescriptor{ "Component*", sizeof(sdk::Component*) }
		{
		}

		void TypeDescriptor_ComponenentPtr::Dump(const void* obj, std::ostream& os) const
		{
			sdk::Component* ptr = *(sdk::Component**)(obj);

			if (ptr == nullptr)
				os << 'N';
			else
			{
				TypeDescriptor* componentTypeDescriptor = ptr->ComponentTypeDescriptor();

				if (componentTypeDescriptor == nullptr)
					return;

				componentTypeDescriptor->Dump(ptr, os);
			}
		}

		void TypeDescriptor_ComponenentPtr::Undump(void* obj, std::istream& os) const {}

		template <>
		TypeDescriptor* GetPrimitiveDescriptor<sdk::Component*>()
		{
			static TypeDescriptor_ComponenentPtr typeDesc;
			return &typeDesc;
		}
	}
}