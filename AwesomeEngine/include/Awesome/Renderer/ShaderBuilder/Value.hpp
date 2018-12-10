#pragma once

#ifndef AWESOME_RENDERER_SHADER_BUILDING_VALUE_GUARD
#define AWESOME_RENDERER_SHADER_BUILDING_VALUE_GUARD

#include <Awesome/Renderer/ShaderBuilder/Type.hpp>

#include <Awesome/Core/Debug.hpp>

#include <Awesome/Maths/Vector2.hpp>
#include <Awesome/Maths/Vector3.hpp>

namespace aw
{
	namespace sb
	{
		enum class VectorComponent : uint8
		{
			X = 0,
			Y = 1,
			Z = 2,
			W = 3,

			R = 0,
			G = 1,
			B = 2,
			A = 3,

			U = 0,
			V = 1
		};

		struct Value : IdWrapper
		{
			Value() = default;
			using IdWrapper::IdWrapper;

			inline Type GetType() const;

			inline Value ExtractFloat(VectorComponent) const;
			inline Value ExtractVector2f() const;
			inline Value ExtractVector3f() const;

			inline Value Shuffle(Value rhs, uint32 a, uint32 b, uint32 c, uint32 d) const;

			template <typename T>
			inline Value ToVector4f(T t) const;
			template <typename T>
			inline Value ToVector4f(T t1, T t2) const;
			template <typename T>
			inline Value ToVector4f(T t1, T t2, T t3) const;

			//inline Value ToMatrix3();

			inline Value X() const;
			inline Value Y() const;
			inline Value Z() const;
			inline Value W() const;
			inline Value XY() const;
			inline Value XYZ() const;

			inline Value Normalize() const;
			inline Value Dot(Value rhs) const;
			inline Value Cross(Value rhs) const;
			inline Value Distance(Value rhs) const;
			inline Value Length() const;
			inline Value Abs() const;
			inline Value Transpose() const;
			inline Value Inverse() const;
			inline Value Negate() const;
			inline static Value Reflect(Value incident, Value normal);
			inline static Value Refract(Value incident, Value normal, Value refractionCoef);
			inline static Value Pow(Value base, Value exp);
			inline static Value Sqrt(Value x);
			inline static Value Max(Value x, Value y);
			inline static Value Min(Value x, Value y);
			inline static Value Mix(Value x, Value y, Value a);
			inline static Value SmoothStep(Value x, Value y, Value a);
			inline static Value Select(Value condition, Value t, Value f);

			inline Value ImageSampleImplicitLod(Value texCoordinates, Type type = Type::MakeVector4f()) const;

			inline static Value MakeVectorf(uint32 numComp, Value);
			inline static Value MakeVector2f(Value);
			inline static Value MakeVector2f(Value, Value);
			inline static Value MakeVector3f(Value);
			inline static Value MakeVector3f(Value, Value, Value);
			inline static Value MakeVector4f(Value);
			inline static Value MakeVector4f(Value, Value, Value, Value);
			inline static Value MakeMatrix3f(Value, Value, Value);
			inline static Value MakeComposite(Type, Value);
			inline static Value MakeComposite(vk::ArrayProxy<const Value>);

			inline static Value MakeConstant(bool, bool specConstant = false);
			inline static Value MakeConstant(int32, bool specConstant = false);
			inline static Value MakeConstant(uint32, bool specConstant = false);
			inline static Value MakeConstant(int64, bool specConstant = false);
			inline static Value MakeConstant(uint64, bool specConstant = false);
			inline static Value MakeConstant(float, bool specConstant = false);
			inline static Value MakeConstant(double, bool specConstant = false);
			inline static Value MakeConstant(Vector2, bool specConstant = false);
			inline static Value MakeConstant(Vector3, bool specConstant = false);
			inline static Value MakeConstant(const std::array<float, 4>& v, bool specConstant = false);

			inline Value operator[](uint64) const;
			inline Value operator-() const;
			inline Value operator+(Value rhs) const;
			inline Value operator-(Value rhs) const;
			inline Value operator*(Value rhs) const;
			inline Value operator/(Value rhs) const;

			inline Value operator<(Value rhs) const;
			inline Value operator>(Value rhs) const;
			inline Value operator<=(Value rhs) const;
			inline Value operator>=(Value rhs) const;
		};

		inline Value operator""_c(unsigned long long ull)
		{
			return Value::MakeConstant(int32(ull));
		}

		inline Value operator""_ci(unsigned long long ull)
		{
			return Value::MakeConstant(int32(ull));
		}

		inline Value operator""_cui(unsigned long long ull)
		{
			return Value::MakeConstant(uint32(ull));
		}

		inline Value operator""_cil(unsigned long long ull)
		{
			return Value::MakeConstant(int64(ull));
		}

		inline Value operator""_cuil(unsigned long long ull)
		{
			return Value::MakeConstant(ull);
		}

		inline Value operator""_c(long double ld)
		{
			return Value::MakeConstant(float(ld));
		}

		inline Value operator""_cf(long double ld)
		{
			return Value::MakeConstant(float(ld));
		}

		inline Value operator""_cd(long double ld)
		{
			return Value::MakeConstant(double(ld));
		}
	}
}

#include <Awesome/Renderer/ShaderBuilder/Value.inl>

#endif //GUARD
