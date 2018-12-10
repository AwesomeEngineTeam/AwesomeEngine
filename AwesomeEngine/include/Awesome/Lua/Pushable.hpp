#pragma once

#ifndef AWESOME_PUSHABLE_GUARD
#define AWESOME_PUSHABLE_GUARD

#if defined(interface)
#undef interface
#endif

#include <Awesome/Core/Reflexion/Reflect.hpp>
#include "luwra/types/pushable.hpp" 

namespace aw
{
	enum class PushableTypeID : size_t
	{
		Bool = 0,
		Int,
		Float,
		Ptr
	};


	struct LuaPushable
	{
		luwra::Pushable pushable;
		reflect::TypeDescriptor* pushableTypeDescriptor;
		const void* (*getValue)(const void*);

		inline LuaPushable();

		template <typename Type> 
		inline LuaPushable(Type&& value);

		inline LuaPushable(const LuaPushable& other) = delete;
		inline LuaPushable(LuaPushable&& other);

		inline bool operator<(const LuaPushable& rhs) const;
		inline LuaPushable& operator=(const LuaPushable& rhs) = delete;
		inline LuaPushable& operator=(LuaPushable&& rhs);
	};
}

template <>
struct luwra::Value<aw::LuaPushable>
{
	inline static void push(State* state, const aw::LuaPushable& value)
	{
		value.pushable.interface->push(state);
	}
};

#include <Awesome/Lua/Pushable.inl>

#endif // !GUARD