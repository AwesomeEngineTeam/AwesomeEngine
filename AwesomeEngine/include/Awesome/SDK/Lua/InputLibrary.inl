namespace input
{
    extern const luwra::MemberMap Functions;

    extern const luwra::MemberMap KeyCodes;
    extern const luwra::MemberMap MouseButtons;

    static void ImportLibrary(lua_State* L)
    {
        luwra::StateWrapper state(L);

        state["Input"] = Functions;
        state["KeyCode"] = KeyCodes;
        state["MouseButton"] = MouseButtons;
    }
}