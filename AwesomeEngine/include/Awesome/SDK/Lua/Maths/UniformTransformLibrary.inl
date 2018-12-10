namespace utr
{
    extern const luwra::MemberMap Functions;
    extern const luwra::MemberMap Fields;
    extern const luwra::MemberMap MetaMethods;

    static void ImportLibrary(lua_State* L)
    {
        //Prerequisites
        rot::ImportLibrary(L);

        luwra::StateWrapper state(L);

        state.registerUserType<UniformTransform(const Vector3&, const Quaternion&, float)>("UTransform", Fields, MetaMethods);
        state["UniformTransform"] = Functions;
    }
}