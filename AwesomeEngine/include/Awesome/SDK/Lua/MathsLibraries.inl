#include <Awesome/SDK/Lua/Maths/MathUtilsLibrary.inl>

#include <Awesome/SDK/Lua/Maths/Vector2Library.inl>
#include <Awesome/SDK/Lua/Maths/Vector3Library.inl>

#include <Awesome/SDK/Lua/Maths/QuaternionLibrary.inl>

#include <Awesome/SDK/Lua/Maths/RotationLibrary.inl>
#include <Awesome/SDK/Lua/Maths/UniformTransformLibrary.inl>

namespace math
{
    static void ImportLibrary(lua_State* L)
    {
        vec2::ImportLibrary(L);
        utr::ImportLibrary(L);
    }
}