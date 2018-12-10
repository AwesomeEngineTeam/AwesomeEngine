#pragma once

#ifndef AWESOME_SDK_COMPONENT_GUARD
#define AWESOME_SDK_COMPONENT_GUARD

#include <Awesome/Core/TypeInfo.hpp>
#include <Awesome/Core/Graph/SceneNodeTemplate.hpp>

#include <Awesome/Maths/Matrix3.hpp>
#include <Awesome/Maths/UniformTransform.hpp>

#include <Awesome/SDK/Config.hpp>

namespace aw
{
	namespace sdk
	{
		template<uint16 mainID, uint32 specID = 0u>
		constexpr uint32 MakeCompID()
		{
			static_assert(mainID <= 0xFFF, "mainID too big");
			static_assert(specID <= 0xFFFFF, "specID too big");

			uint32 id = mainID;

			id <<= 20;
			id |= specID;

			return id;
		}

		enum class ComponentID : uint32
		{
			Component = MakeCompID<0>(),
			Camera = MakeCompID<1>(),
			MeshRenderer = MakeCompID<2>(),
			Light = MakeCompID<3>(),
			PointLight = MakeCompID<3, 1>(),
			DirectionalLight = MakeCompID<3, 2>(),
			Rigidbody = MakeCompID<4>(),
			Collider = MakeCompID<5>(),
			PlaneCollider = MakeCompID<5, 1>(),
			BoxCollider = MakeCompID<5, 2>(),
			SphereCollider = MakeCompID<5, 3>(),
			CapsuleCollider = MakeCompID<5, 4>(),
			MeshCollider = MakeCompID<5, 5>(),
			HeightField = MakeCompID<5, 6>(),
			Script = MakeCompID<6>(),
			SoundListener = MakeCompID<7>(),
			SoundEmitter = MakeCompID<8>(),
			Text2D = MakeCompID<9>(),
			Billboard = MakeCompID<10>(),
			Skybox = MakeCompID<11>()
		};

		inline const char* TypeIDToString(ComponentID id)
		{
			switch(id)
			{
				case ComponentID::Component:
					return "Component";
				case ComponentID::Camera:
					return "Camera";
				case ComponentID::MeshRenderer:
					return "MeshRenderer";
				case ComponentID::Light:
					return "Light";
				case ComponentID::PointLight:
					return "PointLight";
				case ComponentID::DirectionalLight:
					return "DirectionalLight";
				case ComponentID::Rigidbody:
					return "Rigidbody";
				case ComponentID::Collider:
					return "Collider";
				case ComponentID::PlaneCollider:
					return "PlaneCollider";
				case ComponentID::BoxCollider:
					return "BoxCollider";
				case ComponentID::SphereCollider:
					return "SphereCollider";
				case ComponentID::CapsuleCollider:
					return "CapsuleCollider";
				case ComponentID::MeshCollider:
					return "MeshCollider";
				case ComponentID::HeightField:
					return "HeightField";
				case ComponentID::Skybox:
					return "Skybox";
				case ComponentID::Script:
					return "Script";
				case ComponentID::SoundListener:
					return "SoundListener";
				case ComponentID::SoundEmitter:
					return "SoundEmitter";
				case ComponentID::Text2D:
					return "Text2D";
				case ComponentID::Billboard:
					return "Billboard";
			}
		}

		inline const char* TypeIDToString(uint32 id)
		{
			return TypeIDToString(static_cast<ComponentID>(id));
		}

		inline bool InheritsOf(uint32 child, uint32 base)
		{
			return (child & 0xFFF00000) == (base & 0xFFF00000);
		}

		class Actor;

		class Component : public SceneNodeTemplate<Component>
		{
			friend class Actor;

			AWESOME_SDK_API bool GarbageCollect();
			inline void MarkForGarbageCollection();

		protected:
			mutable std::mutex m_trMutex;

		public:
			const uint32 typeId;

			#if defined(WITH_EDITOR)
			bool isImplicit{ false };
			#endif

		//protected:
			Actor* m_actor = nullptr;
			UniformTransform m_localTransform;
			UniformTransform m_globalTransform;

			std::atomic_bool m_isHierarchyDirty = true;


			std::atomic_bool m_isUsed = false;
			std::atomic_bool m_pendingGC = false;

			Component(Actor* actor, uint32 typeId, const std::string& name, const UniformTransform& localTransform = UniformTransform::Identity());

		public:
			inline explicit Component(Actor* actor);
			//Component() = default;
			Component(const Component&) = delete;
			Component(Component&&) = delete;
			~Component() override = default;

			reflect::TypeDescriptor* ComponentTypeDescriptor();

			AWESOME_SDK_API bool GarbageCollectionRequired() const;
			AWESOME_SDK_API void Destroy();

			bool IsActive() const override;
			inline bool IsDirty() const;
			inline Actor* GetActor();
			inline const Actor* GetActor() const;
			inline Vector3 GetLocalPosition() const;
			inline Quaternion GetLocalRotation() const;
			inline UniformTransform GetLocalTransform() const;

			inline UniformTransform GlobalTransform();
			inline Vector3 GlobalPosition();
			inline Quaternion GlobalRotation();
			inline float Scale();

			const std::string& GetComponentTypeName() const;

		protected:
			void SetChildHierarchyDirty();
			void SetHierarchyDirty();
			UniformTransform GetHierarchyGlobalTransform();
			virtual void CalculateGlobalTransform();

		public:
			inline virtual void SetLocalTransform(const UniformTransform& transform);
			inline virtual void SetLocalPosition(const Vector3& position);
			inline virtual void SetLocalRotation(const Quaternion& rotation);
			inline virtual void SetGlobalPosition(const Vector3& position);
			inline virtual void SetGlobalRotation(const Quaternion& rotation);
			inline virtual void SetGlobalTransform(const UniformTransform& transform);
			inline virtual void SetScale(float scale);
			inline virtual void LookAt(const Vector3& target, const Vector3& up = Vector3::Up());
			inline virtual void Rotate_WorldSpace(const Quaternion& rotation);
			inline virtual void Rotate_SelfSpace(const Quaternion& rotation);
			inline virtual void Rotate_WorldSpace(const Vector3& eulerAngles);
			inline virtual void Rotate_SelfSpace(const Vector3& eulerAngles);
			inline virtual void RotateAround(const Vector3& point, const Vector3& axis, float angle);
			inline virtual void Translate_WorldSpace(const Vector3& translationVector);
			inline virtual void Translate_SelfSpace(const Vector3& translationVector);

			//To fix clang mangling bug
			inline void _Translate_WorldSpace(const Vector3& translationVector);
			inline void _Translate_SelfSpace(const Vector3& translationVector);
			inline void _SetGlobalPosition(const Vector3& position);
			inline void _SetGlobalRotation(const Quaternion& rotation);
			inline void _SetGlobalTransform(const UniformTransform& transform);

			inline Component& operator=(const Component&) = delete;
			inline Component& operator=(Component&&) = delete;

			static reflect::TypeDescriptor* TypeIDToTypeDescriptor(uint32 id);

			REFLECT_BEGIN(Component)
				REFLECT_MEMBER_INHERITANCE(SceneNodeTemplate<Component>)
				REFLECT_MEMBER(typeId)
				REFLECT_MEMBER(m_localTransform)
				REFLECT_MEMBER(m_globalTransform)
				REFLECT_MEMBER(m_isHierarchyDirty)
			REFLECT_END()
		};

	}

	namespace reflect
	{
		struct TypeDescriptor_ComponenentPtr : TypeDescriptor
		{
			TypeDescriptor_ComponenentPtr();
			void Dump(const void* obj, std::ostream& os = std::cout) const override;
			void Undump(void* obj, std::istream& os = std::cin) const override;
		};
	}

	template <>
	class TypeInfo<sdk::Component>
	{
	public:
		static const uint32 id = static_cast<uint32>(sdk::ComponentID::Component);
	};
}

#include <Awesome/SDK/Components/Component.inl>

#endif // GUARD
