#include <Awesome/Core/Initializer.hpp>

namespace aw
{
	template<typename... Args>
	int Initializer<Args...>::s_refCount = 0;

	template<typename... Args>
	inline Initializer<Args...>::Initializer()
	{
		InitClass<Args...>();
	}
	
	template<typename... Args>
	inline Initializer<Args...>::~Initializer()
	{
		UninitClass<Args...>();
	}

	template<typename... Args>
	template<typename FirstClass, typename... OtherClass>
	inline void Initializer<Args...>::InitClass()
	{
		if (Initializer<FirstClass>::s_refCount++ == 0)
			FirstClass::Init();

		if constexpr(sizeof...(OtherClass) > 0)
			InitClass<OtherClass...>();
	}

	template<typename... Args>
	template<typename FirstClass, typename... OtherClass>
	inline void Initializer<Args...>::UninitClass()
	{
		if (--Initializer<FirstClass>::s_refCount == 0)
			FirstClass::Uninit();

		if constexpr(sizeof...(OtherClass) > 0)
			UninitClass<OtherClass...>();
	}
}