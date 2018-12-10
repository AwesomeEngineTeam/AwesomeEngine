#pragma once

#ifndef AWESOME_MATHS_UNIFORM_TRANSFORM_GUARD
#define AWESOME_MATHS_UNIFORM_TRANSFORM_GUARD

#include <Awesome/Maths/TransformBase.hpp>

namespace aw
{
	struct Matrix4;

	struct UniformTransform : TransformBase
	{
		std::atomic<float> scale{ 1.0f };

		UniformTransform() = default;
		inline explicit UniformTransform(const Vector3& position, const Quaternion& rotation = Quaternion::Identity(), float scale = 1.0f);
		inline UniformTransform(const UniformTransform& other) : TransformBase(other), scale{ other.scale.load() }
		{
		}
		~UniformTransform() = default;

		inline UniformTransform GetInversed() const;

		AWESOME_MATHS_API Matrix4 ToMatrix() const;
		inline std::string ToString() const;

		inline UniformTransform& Inverse();

		inline static const UniformTransform& Identity();

		inline UniformTransform operator*(const UniformTransform& rhs) const;
		inline Vector3 operator*(const Vector3& rhs) const;
		inline Quaternion operator*(const Quaternion& rhs) const;

		inline UniformTransform& operator=(const UniformTransform& other)
		{
			position = other.GetPosition();
			rotation = other.GetRotation();
			scale = other.scale.load();

			return *this;
		}

		inline bool operator==(const UniformTransform& rhs);
		inline bool operator!=(const UniformTransform& rhs);

		REFLECT_BEGIN(UniformTransform)
			REFLECT_MEMBER_INHERITANCE(TransformBase)
			REFLECT_MEMBER(scale)
		REFLECT_END()
	};
}

#include <Awesome/Maths/UniformTransform.inl>

#endif //GUARD