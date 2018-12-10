#include <Awesome/SDK/Lua/Scene/Components/ComponentLibrary.inl>
#include <Awesome/SDK/Lua/Scene/Components/MeshLibrary.inl>
#include <Awesome/SDK/Lua/Scene/Components/ScriptLibrary.inl>
#include <Awesome/SDK/Lua/Scene/Components/SoundEmitterLibrary.inl>
#include <Awesome/SDK/Lua/Scene/Components/Text2DLibrary.inl>

namespace comp
{
	static void ImportLibrary(lua_State* L)
	{
		component::ImportLibrary(L);
		mesh::ImportLibrary(L);
		script::ImportLibrary(L);
		sound_emitter::ImportLibrary(L);
		text2d::ImportLibrary(L);
	}
}