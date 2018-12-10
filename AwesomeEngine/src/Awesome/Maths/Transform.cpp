#include <Awesome/Maths/Transform.hpp>
#include <Awesome/Maths/Matrix4.hpp>

namespace aw 
{
	Matrix4 Transform::ToMatrix() const
	{
		float xx = rotation.x * rotation.x;
		float yy = rotation.y * rotation.y;
		float zz = rotation.z * rotation.z;
		float wx = rotation.w * rotation.x;
		float wy = rotation.w * rotation.y;
		float wz = rotation.w * rotation.z;
		float xy = rotation.x * rotation.y;
		float xz = rotation.x * rotation.z;
		float yz = rotation.y * rotation.z;

		return {
			scale.x * (1.0f - 2.0f * (yy + zz)),	scale.y * (2.0f * (xy - wz)),			scale.z * (2.0f * (xz + wy)),			position.x,
			scale.x * (2.0f * (xy + wz)),			scale.y * (1.0f - 2.0f * (xx + zz)),	scale.z * (2.0f * (yz - wx)),			position.y,
			scale.x * (2.0f * (xz - wy)),			scale.y * (2.0f * (yz + wx)),			scale.z * (1.0f - 2.0f * (xx + yy)),	position.z,
			0.0f,									0.0f,									0.0f,									1.0f
		};
	}
}