#define COMPONENT_FIELDS(Type) \
	LUWRA_MEMBER_OVERLOAD(Type, GetActor, Actor*(Component::*)()), \
	\
	LUWRA_MEMBER(Type, GetLocalPosition), \
	LUWRA_MEMBER(Type, GetLocalRotation), \
	LUWRA_MEMBER(Type, GetLocalTransform), \
	\
	LUWRA_MEMBER(Type, GlobalPosition), \
	LUWRA_MEMBER(Type, GlobalRotation), \
	LUWRA_MEMBER(Type, GlobalTransform), \
	\
	LUWRA_MEMBER(Type, IsActive), \
	LUWRA_MEMBER(Type, SetActive), \
	\
	LUWRA_MEMBER(Type, LookAt), \
	\
	LUWRA_MEMBER_OVERLOAD(Type, GetParent, Component*(GraphNodeTemplate<Component>::*)()), \
	LUWRA_MEMBER_OVERLOAD(Type, FindChildWithName, Component*(NamedNodeTemplate<Component>::*)(const std::string&) const), \
	LUWRA_MEMBER(Type, name), \
	\
	LUWRA_MEMBER(Type, SetLocalTransform), \
	LUWRA_MEMBER(Type, SetLocalPosition), \
	LUWRA_MEMBER(Type, SetLocalRotation), \
	\
	{ "SetGlobalTransform", LUWRA_WRAP_MEMBER(Type, _SetGlobalTransform) }, \
	{ "SetGlobalPosition", LUWRA_WRAP_MEMBER(Type, _SetGlobalPosition) }, \
	{ "SetGlobalRotation", LUWRA_WRAP_MEMBER(Type, _SetGlobalRotation) }, \
	\
	LUWRA_MEMBER(Type, SetScale), \
	\
	{ "Rotate_SelfSpaceV", LUWRA_WRAP_MEMBER_OVERLOAD(Type, Rotate_SelfSpace, void(Component::*)(const Vector3&)) }, \
	{ "Rotate_WorldSpaceV", LUWRA_WRAP_MEMBER_OVERLOAD(Type, Rotate_WorldSpace, void(Component::*)(const Vector3&)) }, \
	{ "Rotate_SelfSpaceQ", LUWRA_WRAP_MEMBER_OVERLOAD(Type, Rotate_SelfSpace, void(Component::*)(const Quaternion&)) }, \
	{ "Rotate_WorldSpaceQ", LUWRA_WRAP_MEMBER_OVERLOAD(Type, Rotate_WorldSpace, void(Component::*)(const Quaternion&)) }, \
	LUWRA_MEMBER(Type, RotateAround), \
	\
	{ "Translate_WorldSpace", LUWRA_WRAP_MEMBER(Type, _Translate_WorldSpace) }, \
	{ "Translate_SelfSpace", LUWRA_WRAP_MEMBER(Type, _Translate_SelfSpace) }, \
	{ "Destroy", LUWRA_WRAP_MEMBER(Type, Destroy) }

namespace component
{
	//extern const luwra::MemberMap Functions;
	extern const luwra::MemberMap Fields;
	extern const luwra::MemberMap MetaMethods;

	static void ImportLibrary(lua_State* L)
	{
		//Prerequisites
		actor::ImportLibrary(L);

		luwra::StateWrapper state(L);

		state.registerUserType<Component>(Fields, MetaMethods);
	}
}
