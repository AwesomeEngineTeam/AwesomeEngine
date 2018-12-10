namespace quat
{
    extern const luwra::MemberMap Functions;
    extern const luwra::MemberMap Fields;
    extern const luwra::MemberMap MetaMethods;

    static void ImportLibrary(lua_State* L)
    {
        //Prerequisites
        vec3::ImportLibrary(L);

        luwra::StateWrapper state(L);

        state.registerUserType<Quaternion(const float, const float, const float, const float)>("Quat", Fields, MetaMethods);
        state["Quaternion"] = Functions;
    }
}