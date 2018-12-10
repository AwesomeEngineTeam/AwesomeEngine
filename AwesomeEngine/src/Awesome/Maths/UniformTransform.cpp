#include <Awesome/Maths/UniformTransform.hpp>
#include <Awesome/Maths/Matrix4.hpp>

namespace aw
{
	Matrix4 UniformTransform::ToMatrix() const
	{
		Rotation rot = GetRotation();
		Vector3 pos = GetPosition();

		float xx = rot.x * rot.x;
		float yy = rot.y * rot.y;
		float zz = rot.z * rot.z;
		float wx = rot.w * rot.x;
		float wy = rot.w * rot.y;
		float wz = rot.w * rot.z;
		float xy = rot.x * rot.y;
		float xz = rot.x * rot.z;
		float yz = rot.y * rot.z;

		return {
			scale * (1.0f - 2.0f * (yy + zz)),	scale * (2.0f * (xy - wz)),			scale * (2.0f * (xz + wy)),			pos.x,
			scale * (2.0f * (xy + wz)),			scale * (1.0f - 2.0f * (xx + zz)),	scale * (2.0f * (yz - wx)),			pos.y,
			scale * (2.0f * (xz - wy)),			scale * (2.0f * (yz + wx)),			scale * (1.0f - 2.0f * (xx + yy)),	pos.z,
			0.0f,								0.0f,							0.0f,									1.0f
		};
	}
}