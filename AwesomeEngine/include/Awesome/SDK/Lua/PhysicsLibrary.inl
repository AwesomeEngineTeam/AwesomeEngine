namespace physics
{
	extern const luwra::MemberMap Functions;
	extern const luwra::MemberMap RaycastFields;

	extern const luwra::MemberMap RigidbodyFields;

	extern const luwra::MemberMap ForceMode;

	static void ImportLibrary(lua_State* L)
	{
		math::ImportLibrary(L);

		luwra::StateWrapper state(L);

		state.registerUserType<RaycastInfo()>("HitInfo", RaycastFields);
		state.registerUserType<Rigidbody>(RigidbodyFields);

		state["Physics"] = Functions;
		state["ForceMode"] = ForceMode;
	}
}