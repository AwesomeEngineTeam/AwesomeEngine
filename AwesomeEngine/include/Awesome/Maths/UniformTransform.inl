namespace aw
{
	inline UniformTransform::UniformTransform(const Vector3& position, const Quaternion& rotation, float scale) :
		TransformBase(position, rotation),
		scale{ scale }
	{
	}

	inline UniformTransform UniformTransform::GetInversed() const
	{
		UniformTransform inverse;

		Rotation rot = GetRotation();
		Vector3 pos = GetPosition();

		inverse.rotation = rot.GetInversed();
		inverse.scale = 1.0f / scale;
		inverse.position = -(inverse.scale * inverse.rotation * pos);

		return inverse;
	}

	inline std::string UniformTransform::ToString() const
	{
		Rotation rot = GetRotation();
		Vector3 pos = GetPosition();

		return "[Transform]\n  |_ Position : x = " +
			   std::to_string(pos.x) + "\ty = " + std::to_string(pos.y) + "\tz = " + std::to_string(pos.z) + "\n  |_ Rotation : w = " +
			   std::to_string(rot.w) + "\tx = " + std::to_string(rot.x) + "\ty = " + std::to_string(rot.y) + "\tz = " + std::to_string(rot.z) + "\n  |_ Scale    : x = " +
			   std::to_string(scale) + "\ty = " + std::to_string(scale) + "\tz = " + std::to_string(scale);
	}

	inline UniformTransform& UniformTransform::Inverse()
	{
		m_rotMutex.lock();
		rotation.Inverse();
		m_rotMutex.unlock();

		scale = 1.0f / scale;

		m_rotMutex.lock();
		position = -(scale * rotation * position);
		m_rotMutex.unlock();

		return *this;
	}

	inline const UniformTransform& UniformTransform::Identity()
	{
		static const UniformTransform awUniformTransformIdentity;
		return awUniformTransformIdentity;
	}

	inline UniformTransform UniformTransform::operator*(const UniformTransform& rhs) const
	{
		UniformTransform result;

		Rotation rot = GetRotation();
		Vector3 pos = GetPosition();

		result.position = rot * (scale * rhs.GetPosition()) + pos;
		result.rotation = rot * rhs.GetRotation();
		result.scale = scale * rhs.scale;

		return result;
	}

	inline Vector3 UniformTransform::operator*(const Vector3& rhs) const
	{
		return GetRotation() * (scale * rhs) + GetPosition();
	}

	inline Quaternion UniformTransform::operator*(const Quaternion& rhs) const
	{
		return GetRotation() * rhs;
	}

	inline bool UniformTransform::operator==(const UniformTransform& rhs)
	{
		return (GetPosition() == rhs.GetPosition()) && (GetRotation() == rhs.GetRotation()) && NearlyEqual(scale, rhs.scale);
	}

	inline bool UniformTransform::operator!=(const UniformTransform& rhs)
	{
		return !this->operator==(rhs);
	}
}