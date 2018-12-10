#pragma once

#ifndef AWESOME_SDK_LIBRARIES_GUARD
#define AWESOME_SDK_LIBRARIES_GUARD

#include <Awesome/SDK/Config.hpp>

#include <Awesome/Lua/LuaHelper.hpp>

#include <Awesome/Maths/Vector2.hpp>
#include <Awesome/Maths/Vector3.hpp>

#include <Awesome/Maths/Quaternion.hpp>
#include <Awesome/Maths/UniformTransform.hpp>

#include <Awesome/Windowing/Input/KeyCodes.hpp>
#include <Awesome/Windowing/Input/InputManager.hpp>

#include <Awesome/SDK/Scene/Scene.hpp>
#include <Awesome/SDK/Scene/SceneManager.hpp>

#include <Awesome/SDK/Components/Lua/Script.hpp>
#include <Awesome/SDK/Components/Render/MeshRenderer.hpp>

#include "luwra.hpp"

#define LUA_END_METATABLE LUA_HIDE_METATABLE_REG, LUA_NULL_REG

#include <Awesome/SDK/Lua/LuaTypes.inl>

namespace aw
{
	namespace sdk
	{
	    namespace lua
        {
			#include <Awesome/SDK/Lua/TimeLibrary.inl>
            #include <Awesome/SDK/Lua/InputLibrary.inl>

            #include <Awesome/SDK/Lua/MathsLibraries.inl>
			#include <Awesome/SDK/Lua/SceneLibraries.inl>
			#include <Awesome/SDK/Lua/PhysicsLibrary.inl>

            #include <Awesome/SDK/Lua/BaseLibrary.inl>

        }
	}
}

#endif //Guard