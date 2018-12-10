namespace aw
{
	/*inline TransformBase::Rotation::Rotation(float w, float x, float y, float z) :
		Quaternion(w, x, y, z)
	{
	}*/

	inline TransformBase::Rotation::Rotation(const Quaternion& quaternion) :
		Quaternion(FromQuaternion(quaternion))
	{
	}

	inline TransformBase::Rotation::Rotation(Quaternion&& quaternion) :
		Quaternion(FromQuaternion(quaternion))
	{
	}

	inline Quaternion TransformBase::Rotation::FromQuaternion(const Quaternion& quaternion)
	{
		if (quaternion == Quaternion::Zero())
			return Quaternion::Identity();
		else
			return quaternion.GetNormalized();
	}

	inline Quaternion TransformBase::Rotation::GetConjugated() const
	{
		return { w, -x, -y, -z };
	}

	inline Quaternion TransformBase::Rotation::GetInversed() const
	{
		return GetConjugated();
	}

	inline Quaternion& TransformBase::Rotation::Conjugate()
	{
		x = -x;
		y = -y;
		z = -z;

		return *this;
	}

	inline Quaternion& TransformBase::Rotation::Inverse()
	{
		return Conjugate();
	}

	inline TransformBase::Rotation& TransformBase::Rotation::operator=(const Quaternion& quaternion)
	{
		Quaternion q = FromQuaternion(quaternion);

		w = q.w.load();
		x = q.x.load();
		y = q.y.load();
		z = q.z.load();

		return *this;
	}

	inline TransformBase::Rotation& TransformBase::Rotation::operator=(Quaternion&& quaternion)
	{
		Quaternion q = FromQuaternion(quaternion);

		w = q.w.load();
		x = q.x.load();
		y = q.y.load();
		z = q.z.load();

		return *this;
	}
}