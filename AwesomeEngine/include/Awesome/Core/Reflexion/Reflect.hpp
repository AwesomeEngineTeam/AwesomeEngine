#pragma once

#ifndef AWESOME_REFLECT_GUARD
#define AWESOME_REFLECT_GUARD

#include <Awesome/Core/Config.hpp>

#include <memory>
#include <iostream>
#include <sstream>

#include <list>
#include <map>
#include <vector>
#include <string>
#include <atomic>

namespace aw
{
	AWESOME_CORE_API std::stringstream CutString(std::string& s, size_t pos, size_t count = std::string::npos);

	namespace reflect
	{
		struct TypeDescriptor
		{
			const char* name;
			size_t size;

			inline TypeDescriptor(const char* name, size_t size);
			virtual ~TypeDescriptor() = default;

			inline virtual std::string GetFullName() const;
			virtual void Dump(const void* obj, std::ostream& os = std::cout) const = 0;
			virtual void Undump(void* obj, std::istream& is = std::cin) const = 0;
		};

		template <typename T>
		TypeDescriptor* GetPrimitiveDescriptor();

		struct DefaultResolver
		{
			template <typename T> static char Func(decltype(&T::Reflection));
			template <typename T> static int Func(...);

			template <typename T>
			struct IsReflected
			{
				enum { value = (sizeof(Func<T>(nullptr)) == sizeof(char)) };
			};

			template <typename T>
			inline static TypeDescriptor* Get()
			{
				if constexpr (IsReflected<T>::value)
					return &T::Reflection;
				else
					return GetPrimitiveDescriptor<T>();
			}
		};

		template <typename T>
		struct TypeResolver
		{
			using ValueType = std::remove_reference_t<std::remove_cv_t<T>>;

			inline static TypeDescriptor* Get()
			{
				return DefaultResolver::Get<ValueType>();
			}

			inline static ValueType FromDump(std::istream& is = std::cin)
			{
				ValueType val;
				TypeDescriptor* td = Get();

				td->Undump(&val, is);

				return val;
			}
		};

		struct TypeDescriptor_Struct : public TypeDescriptor
		{
			struct Member
			{
				const char* name;
				size_t offset;
				TypeDescriptor* type;
			};

			std::vector<Member> members;

			inline TypeDescriptor_Struct(void(*init)(TypeDescriptor_Struct*));
			inline TypeDescriptor_Struct(const char* name, size_t size, const std::initializer_list<Member>& init);

			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};

#define REFLECT_BEGIN(type) \
		friend struct aw::reflect::DefaultResolver; \
		inline static void initReflection(aw::reflect::TypeDescriptor_Struct* typeDesc) \
		{ \
			using T = type; \
			typeDesc->name = #type; \
			typeDesc->size = sizeof(T);

#define REFLECT_MEMBER_INHERITANCE(BASE) \
			typeDesc->members = BASE::Reflection.members;

#define REFLECT_MEMBER(name) \
			typeDesc->members.push_back({#name, offsetof(T, name), aw::reflect::TypeResolver<decltype(T::name)>::Get()});

#define REFLECT_END() \
		} \
		inline static aw::reflect::TypeDescriptor_Struct Reflection{ initReflection }; 

#define REFLECT_ENUM(Enum) template <> \
		aw::reflect::TypeDescriptor* aw::reflect::GetPrimitiveDescriptor<Enum>() \
		{ \
			return aw::reflect::TypeResolver<std::underlying_type<Enum>::type>::Get(); \
		}

#define REFLECT_ENUM_EXPORT(Enum, Export) template <> \
		Export aw::reflect::TypeDescriptor* aw::reflect::GetPrimitiveDescriptor<Enum>() \
		{ \
			return aw::reflect::TypeResolver<std::underlying_type<Enum>::type>::Get(); \
		}
	}
}

#include <Awesome/Core/Reflexion/Reflect.inl>
#include <Awesome/Core/Reflexion/Specializations.inl>

#endif // !AWESOME_REFLECT_GUARD
