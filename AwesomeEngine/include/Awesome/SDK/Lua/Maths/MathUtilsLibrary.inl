namespace math_utils
{
    extern const luwra::MemberMap Table;
//    extern const luwra::MemberMap Constants;
//    extern const luwra::MemberMap MetaMethods;

    static void ImportLibrary(lua_State* L)
    {
        luwra::StateWrapper state(L);
        state["Maths"] = Table;
    }
}