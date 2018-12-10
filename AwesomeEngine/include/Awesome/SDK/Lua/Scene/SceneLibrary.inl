namespace scene
{
	extern const luwra::MemberMap Functions;
	extern const luwra::MemberMap Fields;

	static void ImportLibrary(lua_State* L)
	{
		luwra::StateWrapper state(L);

		state.registerUserType<Scene>(Fields);
		state["Scene"] = Functions;
	}
}