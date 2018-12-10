namespace text2d
{
	extern const luwra::MemberMap Fields;

	static void ImportLibrary(lua_State* L)
	{
		//Prerequisites
		component::ImportLibrary(L);

		luwra::StateWrapper state(L);

		state.registerUserType<Text2D>(Fields);
	}
}