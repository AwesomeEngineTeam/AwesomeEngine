#include <Awesome/SDK/Lua/Libraries.hpp>

#define WRAP_SCENE(field) LUWRA_MEMBER(Scene, field)

namespace aw
{
	namespace sdk
	{
		namespace lua
		{
			namespace scene
			{
				const luwra::MemberMap Functions
				{
					{ "FindActorWithName", LUWRA_WRAP(SceneManager::FindActorWithName) },
					{ "FindActorsWithName", LUWRA_WRAP(SceneManager::FindActorsWithName) },
					{ "RemoveActor", LUWRA_WRAP(SceneManager::RemoveActor) }
				};

				const luwra::MemberMap Fields
				{
					//LUWRA_MEMBER_OVERLOAD(Scene, CreateActor, Actor*(Scene::*)(Actor*, const std::string&)),
					//WRAP_SCENE(RemoveActor)
				};
			}
		}
	}
}

#undef WRAP_SCENE