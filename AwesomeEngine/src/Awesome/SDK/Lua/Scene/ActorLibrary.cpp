#include <Awesome/SDK/Lua/Libraries.hpp>

#define WRAP_ACTOR(field) LUWRA_MEMBER(Actor, field)

namespace aw
{
	namespace sdk
	{
		namespace lua
		{
			namespace actor
			{
				const luwra::MemberMap Fields
				{
					LUWRA_MEMBER_OVERLOAD(Actor, Rotate_SelfSpace, void(Actor::*)(const Vector3&)),
					LUWRA_MEMBER_OVERLOAD(Actor, Rotate_WorldSpace, void(Actor::*)(const Vector3&)),
					WRAP_ACTOR(Translate_WorldSpace),
					WRAP_ACTOR(Translate_SelfSpace),
					WRAP_ACTOR(GetMesh),
					WRAP_ACTOR(GetMeshes),
					WRAP_ACTOR(GetScript),
					WRAP_ACTOR(GetScripts),
					WRAP_ACTOR(GetSoundEmitter),
					WRAP_ACTOR(GetSoundEmitters),
					LUWRA_MEMBER_OVERLOAD(Actor, GetRootComponent, Component*(Actor::*)()),
					LUWRA_MEMBER_OVERLOAD(Actor, GetParent, Actor*(GraphNodeTemplate<Actor>::*)()),
					WRAP_ACTOR(FindComponentWithName),
					WRAP_ACTOR(SetGlobalPosition),
					WRAP_ACTOR(SetGlobalRotation),
					WRAP_ACTOR(RemoveComponent),
					WRAP_ACTOR(Destroy),
					WRAP_ACTOR(name),
					WRAP_ACTOR(SetActive),
					WRAP_ACTOR(IsActive)
				};

				const luwra::MemberMap MetaMethods
				{

				};
			}
		}
	}
}

#undef WRAP_ACTOR