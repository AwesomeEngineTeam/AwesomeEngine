#define WRAP_VEC2(field) LUWRA_MEMBER(Vector2, field)

namespace vec2
{
    extern const luwra::MemberMap Functions;
    extern const luwra::MemberMap Fields;
    extern const luwra::MemberMap MetaMethods;

    static void ImportLibrary(lua_State* L)
    {
        luwra::StateWrapper state(L);

        state.registerUserType<Vector2(float, float)>("Vec2", Fields, MetaMethods);
        state["Vector2"] = Functions;
    }
}

#undef WRAP_VEC2