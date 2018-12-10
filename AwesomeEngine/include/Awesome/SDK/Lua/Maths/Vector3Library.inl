namespace vec3
{
    extern const luwra::MemberMap Functions;
    extern const luwra::MemberMap Fields;
    extern const luwra::MemberMap MetaMethods;

    static void ImportLibrary(lua_State* L)
    {
        //Prerequisites
        math_utils::ImportLibrary(L);

        luwra::StateWrapper state(L);

        state.registerUserType<Vector3(const float, const float, const float)>("Vec3", Fields, MetaMethods);
        state["Vector3"] = Functions;
    }
}