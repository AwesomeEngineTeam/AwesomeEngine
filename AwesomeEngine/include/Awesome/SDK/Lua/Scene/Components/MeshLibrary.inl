namespace mesh
{
	//extern const luwra::MemberMap Functions;
	extern const luwra::MemberMap Fields;
	extern const luwra::MemberMap MetaMethods;

	static void ImportLibrary(lua_State* L)
	{
		//Prerequisites
		component::ImportLibrary(L);

		luwra::StateWrapper state(L);

		state.registerUserType<MeshRenderer>(Fields, MetaMethods);
	}
}