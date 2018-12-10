namespace time
{
	extern const luwra::MemberMap Functions;

	static void ImportLibrary(lua_State* L)
	{
		luwra::StateWrapper state(L);
		state["Time"] = Functions;
	}
}