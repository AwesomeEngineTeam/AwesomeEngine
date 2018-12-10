namespace script
{
	extern const luwra::MemberMap Fields;
	extern const luwra::MemberMap MetaMethods;

	static void ImportLibrary(lua_State* L)
	{
		luwra::StateWrapper state(L);
		state.registerUserType<Script>(Fields, MetaMethods);
	}
}