namespace rot
{
    extern const luwra::MemberMap Fields;
    extern const luwra::MemberMap MetaMethods;

    static void ImportLibrary(lua_State* L)
    {
        //Prerequisites
        quat::ImportLibrary(L);

        luwra::StateWrapper state(L);
        state.registerUserType<TransformBase::Rotation(const Quaternion&)>("Rotation", Fields, MetaMethods);
    }
}