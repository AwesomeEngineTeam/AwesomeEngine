#pragma once

#ifndef AWESOME_RENDERER_SHADER_BUILDING_VARIABLE_GUARD
#define AWESOME_RENDERER_SHADER_BUILDING_VARIABLE_GUARD

#include <Awesome/Renderer/ShaderBuilder/Value.hpp>

#include <Awesome/Core/Debug.hpp>

namespace aw
{
	namespace sb
	{
		struct Variable : IdWrapper
		{
			Value value{ 0 };
			spv::StorageClass storageClass;

			Variable() = default;
			inline Variable(const Variable&);
			inline Variable(Variable&&) noexcept;

			inline Variable(spv::StorageClass, Type, const char* name = nullptr);
			inline Variable(spv::Id, spv::StorageClass);

			inline void Store(const Value&);
			inline Value Load() const;

			inline Value GetValue();

			inline Variable MakeAccessChain(const std::vector<Value>& offsets);
			inline static Variable CreateUniform(Type, int32 binding, int32 set = 0, const char* name = nullptr);
			inline static Variable CreateUniform(Type, const char* name = nullptr);
			inline static Variable CreateUniformConstant(Type, int32 binding, int32 set = 0, const char* name = nullptr);
			inline static Variable CreateUniformConstant(Type, const char* name = nullptr);

			inline Type GetType();
			inline Type GetVariableType() const;

			inline Value ExtractFloat(VectorComponent);
			inline Value ExtractVector2f();
			inline Value ExtractVector3f();

			inline Value Shuffle(Value rhs, uint32 a, uint32 b, uint32 c, uint32 d);

			template <typename T>
			inline Value ToVector4f(T t);
			template <typename T>
			inline Value ToVector4f(T t1, T t2);
			template <typename T>
			inline Value ToVector4f(T t1, T t2, T t3);

			inline Value X();
			inline Value Y();
			inline Value Z();
			inline Value W();
			inline Value XY();
			inline Value XYZ();

			inline Value Normalize();
			inline Value Dot(Value);
			inline Value Cross(Value);
			inline Value Abs();
			inline Value Transpose();
			inline Value Inverse();
			inline Value Negate();

			inline Value ImageSampleImplicitLod(Value texCoordinates, Type = Type::MakeVector4f());

			inline operator Value();

			inline Value* operator->() { GetValue(); return &value; }

			inline Value operator-();
			inline Value operator+(Value);
			inline Value operator-(Value);
			inline Value operator*(Value);
			inline Value operator/(Value);

			inline Value operator<(Value);
			inline Value operator>(Value);

			inline Variable& operator++();
			inline Variable operator++(int);
			inline Variable& operator--();
			inline Variable operator--(int);

			inline Variable& operator+=(Value);
			inline Variable& operator-=(Value);
			inline Variable& operator*=(Value);

			inline Variable& operator=(Value);
			inline Variable& operator=(const Variable&);
			inline Variable& operator=(Variable&&);
		};
	}
}

#include <Awesome/Renderer/ShaderBuilder/Variable.inl>

#endif //GUARD
