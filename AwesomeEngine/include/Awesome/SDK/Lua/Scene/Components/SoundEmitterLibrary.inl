namespace sound_emitter
{
	extern const luwra::MemberMap Fields;

	extern const luwra::MemberMap ActionType;
	extern const luwra::MemberMap InterpMode;
	
	extern const luwra::MemberMap SoundMap;

	static void ImportLibrary(lua_State* L)
	{
		luwra::StateWrapper state(L);
		state.registerUserType<SoundEmitter>(Fields);

		state["Sound"] = SoundMap;
	}
}