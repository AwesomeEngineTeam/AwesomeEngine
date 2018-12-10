namespace aw
{
	inline Transform::Transform(const Transform& other) :
		TransformBase(other.GetPosition(), other.GetRotation()),
		scale{ scale }
	{
	}

	inline Transform::Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale) :
		TransformBase(position, rotation),
		scale{ scale }
	{
	}

	inline std::string Transform::ToString() const
	{
		return "[Transform]\n  |_ Position : x = " +
			std::to_string(position.x) + "\ty = " + std::to_string(position.y) + "\tz = " + std::to_string(position.z) + "\n  |_ Rotation : w = " + 
			std::to_string(rotation.w) + "\tx = " + std::to_string(rotation.x) + "\ty = " + std::to_string(rotation.y) + "\tz = " + std::to_string(rotation.z) + "\n  |_ Scale    : x = " +
			std::to_string(scale.x) + "\ty = " + std::to_string(scale.y) + "\tz = " + std::to_string(scale.z);
	}

	inline const Transform& Transform::Identity()
	{
		static const Transform awTransformIdentity;
		return awTransformIdentity;
	}

	inline Transform& Transform::operator=(const Transform& other)
	{
		SetPosition(other.GetPosition());
		SetRotation(other.GetRotation());
		scale = other.scale;

		return *this;
	}

	inline bool Transform::operator==(const Transform& rhs)
	{
		return (position == rhs.position) && (rotation == rhs.rotation) && (scale == rhs.scale);
	}

	inline bool Transform::operator!=(const Transform& rhs)
	{
		return !this->operator==(rhs);
	}
}