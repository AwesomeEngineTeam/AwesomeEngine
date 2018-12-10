#pragma once

#ifndef AWESOME_MATHS_TRANSFORM_GUARD
#define AWESOME_MATHS_TRANSFORM_GUARD

#include <Awesome/Maths/TransformBase.hpp>

namespace aw
{
	struct Matrix4;

	struct Transform : TransformBase
	{
		Vector3 scale{ 1.0f, 1.0f, 1.0f };

		Transform() = default;
		inline Transform(const Transform& other);
		inline explicit Transform(const Vector3& position, const Quaternion& rotation = Quaternion::Identity(), const Vector3& scale = Vector3::One());
		~Transform() = default;

		AWESOME_MATHS_API Matrix4 ToMatrix() const;
		inline std::string ToString() const;

		inline static const Transform& Identity();

		inline Transform& operator=(const Transform& other);

		inline bool operator==(const Transform& rhs);
		inline bool operator!=(const Transform& rhs);

		REFLECT_BEGIN(Transform)
			REFLECT_MEMBER_INHERITANCE(TransformBase)
			REFLECT_MEMBER(scale)
		REFLECT_END()
	};
}

#include <Awesome/Maths/Transform.inl>

#endif //GUARD

// { 
// 	x1 = (m1((m22-1) m5-m4 m6)+m2(m11 m6-m6-m4 m5) +m3(m4^2+m22+m11(1-m22)-1)) /(m00(m6^2+m11(-m6^2-m33+m22(m33-1)+1)+2m4 m5 m6+(1-m22) m5^2 +(1-m33) m4^2+m33+m22(1-m33)-1) +m11(m6^2+m33+m22(1-m33)-1)+m1^2(m6^2+m33+m22(1-m33)-1)-m6^2 +m3(m1((2m22-2) m5-2m4 m6)+m2(2m11 m6-2m6-2m4 m5)) +m1 m2((2m33-2) m4-2m5 m6)-2m4 m5 m6 +m2^2(m5^2+m33+m11(1-m33)-1)+(m22-1) m5^2 +m3^2(m4^2+m22+m11(1-m22)-1)+(m33-1) m4^2-m33+m22(m33-1)+1), 
// 	x2 = -(m00((m22-1) m5-m4 m6)+m4 m6+m1 m2 m6+(1-m22) m5-m2^2 m5 +m3(m2 m4+m1(1-m22))) /(m00(m6^2+m11(-m6^2-m33+m22(m33-1)+1)+2m4 m5 m6+(1-m22) m5^2 +(1-m33) m4^2+m33+m22(1-m33)-1) +m11(m6^2+m33+m22(1-m33)-1)+m1^2(m6^2+m33+m22(1-m33)-1)-m6^2 +m3(m1((2m22-2) m5-2m4 m6)+m2(2m11 m6-2m6-2m4 m5)) +m1 m2((2m33-2) m4-2m5 m6)-2m4 m5 m6 +m2^2(m5^2+m33+m11(1-m33)-1)+(m22-1) m5^2 +m3^2(m4^2+m22+m11(1-m22)-1)+(m33-1) m4^2-m33+m22(m33-1)+1), 
// 	x3 = (m00(-m11 m6+m6+m4 m5)+m11 m6+m1^2 m6-m6-m4 m5-m1 m2 m5 +m3((m11-1) m2-m1 m4)) /(m00(m6^2+m11(-m6^2-m33+m22(m33-1)+1)+2m4 m5 m6+(1-m22) m5^2 +(1-m33) m4^2+m33+m22(1-m33)-1) +m11(m6^2+m33+m22(1-m33)-1)+m1^2(m6^2+m33+m22(1-m33)-1)-m6^2 +m3(m1((2m22-2) m5-2m4 m6)+m2(2m11 m6-2m6-2m4 m5)) +m1 m2((2m33-2) m4-2m5 m6)-2m4 m5 m6 +m2^2(m5^2+m33+m11(1-m33)-1)+(m22-1) m5^2 +m3^2(m4^2+m22+m11(1-m22)-1)+(m33-1) m4^2-m33+m22(m33-1)+1), 
// 	x4 = -(m00(m4^2+m22+m11(1-m22)-1)-m4^2-2m1 m2 m4-m22+m11(m22-1) +m1^2(m22-1)+(m11-1) m2^2+1) /(m00(m6^2+m11(-m6^2-m33+m22(m33-1)+1)+2m4 m5 m6+(1-m22) m5^2 +(1-m33) m4^2+m33+m22(1-m33)-1) +m11(m6^2+m33+m22(1-m33)-1)+m1^2(m6^2+m33+m22(1-m33)-1)-m6^2 +m3(m1((2m22-2) m5-2m4 m6)+m2(2m11 m6-2m6-2m4 m5)) +m1 m2((2m33-2) m4-2m5 m6)-2m4 m5 m6 +m2^2(m5^2+m33+m11(1-m33)-1)+(m22-1) m5^2 +m3^2(m4^2+m22+m11(1-m22)-1)+(m33-1) m4^2-m33+m22(m33-1)+1) 
// }