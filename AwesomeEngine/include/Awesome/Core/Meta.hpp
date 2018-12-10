#pragma once

#ifndef AWESOME_CORE_META_GUARD
#define AWESOME_CORE_META_GUARD

#include <utility>
#include <typeindex>

#define REGISTER_TYPE(T) \
    namespace meta \
    { \
        static const std::type_index T##_index = typeid(T); \
        template<> struct TypeChar<T> { static constexpr const std::type_index& value = T##_index; }; \
        template<> struct CharType<T##_index> { typedef T type; }; \
    } \

namespace aw
{
    namespace meta
    {
        template<typename T> struct TypeChar {};
        template<const std::type_index& t> struct CharType { };

        struct FieldDescriptor
        {
            size_t offset;
            const char* name;
            const std::type_index& type;
        };

        template<typename T>
        struct ClassDescriptor;

        template<typename T>
        struct Field
        {
            const char* name;
            T value;
        };

        template<typename T>
        static Field<T> MakeField(const char* name, T value)
        {
            return { name, value };
        }
    }

    using String = char*;

    REGISTER_TYPE(int)
    REGISTER_TYPE(double)
    REGISTER_TYPE(float)
    REGISTER_TYPE(String)
    REGISTER_TYPE(void);
}

#define REGISTER_CLASS_BEGIN(type, numFields) \
    REGISTER_TYPE(type) \
    namespace meta \
    { \
        template<> \
        struct ClassDescriptor<type> \
        { \
            using T = type; \
     	    static constexpr auto nbFields = numFields; \
			static constexpr auto seq = std::make_index_sequence<nbFields>(); \
            \
            template<typename F, size_t... I> \
            static void VisitImpl(F& f, void* obj, std::index_sequence<I...>) \
            { \
                char* ptr = reinterpret_cast<char*>(obj); \
                \
				 static constexpr FieldDescriptor fields[numFields]{

#define REGISTER_FIELD(field) \
                    { offsetof(T, field), #field, TypeChar<decltype(T::field)>::value },

#define REGISTER_CLASS_END \
                }; \
                \
                using swallow = int[]; \
                (void)swallow{0, (void(f(MakeField(fields[I].name, *reinterpret_cast<typename CharType<fields[I].type>::type*>(ptr + fields[I].offset)))), 0)...}; \
            } \
            \
            template<typename F> \
            static void Visit(F& f, void* obj) \
            { \
                VisitImpl(f, obj, seq); \
            } \
        }; \
    }

//marouflage for MSVC
//using swallow = int[]; \
//(void)swallow{0, (void(f(MakeField(fields[I].name, *reinterpret_cast<typename CharType<fields[I].type>::type*>(ptr + fields[I].offset)))), 0)...}; \

//Normal way for the rest of the entire universe
//(static_cast<void>(std::invoke(f, MakeField(fields[I].name, *reinterpret_cast<typename CharType<fields[I].type>::type*>(ptr + fields[I].offset)))), ...); \

#endif //AWESOME_CORE_META_GUARD