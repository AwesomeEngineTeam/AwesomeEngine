#define LUA_ENUM(Type) \
	namespace luwra \
	{ \
		template<> \
		struct Value<Type> \
		{ \
			\
			static inline Type read(lua_State *state, int n) \
			{ \
				auto value = Value<int>::read(state, n); \
			\
				return Type(value); \
			} \
			\
			\
			static inline size_t push(lua_State *state, Type value) \
			{ \
				lua_pushinteger(state, static_cast<int>(value)); \
				return 1; \
			} \
		}; \
	}

LUA_ENUM(aw::KeyCode)
LUA_ENUM(aw::MouseButton)
LUA_ENUM(aw::Severity)
LUA_ENUM(aw::ph::ForceMode)
LUA_ENUM(aw::sdk::SoundEmitter::ActionType)
LUA_ENUM(aw::sdk::SoundEmitter::InterpMode)
LUA_ENUM(AKRESULT)

LUWRA_DEF_REGISTRY_NAME(aw::Vector2, "Vector2")
LUWRA_DEF_REGISTRY_NAME(aw::Vector3, "Vector3")
LUWRA_DEF_REGISTRY_NAME(aw::Quaternion, "Quaternion")
LUWRA_DEF_REGISTRY_NAME(aw::TransformBase::Rotation, "Rotation")
LUWRA_DEF_REGISTRY_NAME(aw::UniformTransform, "UniformTransform")

LUWRA_DEF_REGISTRY_NAME(aw::sdk::RaycastInfo, "RaycastInfo")

LUWRA_DEF_REGISTRY_NAME(aw::sdk::Actor, "Actor")
LUWRA_DEF_REGISTRY_NAME(aw::sdk::Scene, "Scene")

LUWRA_DEF_REGISTRY_NAME(aw::sdk::Component, aw::sdk::TypeIDToString(aw::TypeInfo<aw::sdk::Component>::id))
LUWRA_DEF_REGISTRY_NAME(aw::sdk::MeshRenderer, aw::sdk::TypeIDToString(aw::TypeInfo<aw::sdk::MeshRenderer>::id))
LUWRA_DEF_REGISTRY_NAME(aw::sdk::Script, aw::sdk::TypeIDToString(aw::TypeInfo<aw::sdk::Script>::id))
LUWRA_DEF_REGISTRY_NAME(aw::sdk::SoundEmitter, aw::sdk::TypeIDToString(aw::TypeInfo<aw::sdk::SoundEmitter>::id))
LUWRA_DEF_REGISTRY_NAME(aw::sdk::Text2D, aw::sdk::TypeIDToString(aw::TypeInfo<aw::sdk::Text2D>::id))
LUWRA_DEF_REGISTRY_NAME(aw::sdk::Rigidbody, aw::sdk::TypeIDToString(aw::TypeInfo<aw::sdk::Rigidbody>::id))

namespace luwra
{
	template <>
	struct Value<aw::MousePos>
	{

		static inline aw::MousePos read(lua_State* state, int n)
		{
			auto mousePos = Value<aw::Vector2>::read(state, n);

			return { static_cast<int>(mousePos.x), static_cast<int>(mousePos.y) };
		}


		static inline size_t push(lua_State* state, aw::MousePos val)
		{
			Value<aw::Vector2>::push(state, static_cast<float>(val.x), static_cast<float>(val.y));

			return 1;
		}
	};

	template <>
	struct Value<std::atomic<float>>
	{

		static inline float read(lua_State* state, int n)
		{
			return Value<float>::read(state, n);
		}


		static inline size_t push(lua_State* state, const std::atomic<float>& val)
		{
			Value<float>::push(state, val.load());
			return 1;
		}
	};

	template <>
	struct Value<std::atomic_bool>
	{

		static inline float read(lua_State* state, int n)
		{
			return Value<bool>::read(state, n);
		}


		static inline size_t push(lua_State* state, const std::atomic_bool& val)
		{
			Value<bool>::push(state, val.load());
			return 1;
		}
	};

	template <>
	struct Value<std::atomic_uint8_t>
	{

		static inline float read(lua_State* state, int n)
		{
			return Value<uint8_t>::read(state, n);
		}


		static inline size_t push(lua_State* state, const std::atomic_uint8_t& val)
		{
			Value<uint8_t>::push(state, val.load());
			return 1;
		}
	};
}

#undef LUA_ENUM