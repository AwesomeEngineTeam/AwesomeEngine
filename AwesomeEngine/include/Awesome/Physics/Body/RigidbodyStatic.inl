namespace aw
{
	namespace ph
	{
		inline RigidbodyStatic::RigidbodyStatic(const TransformBase& tr, void* userData)
		{
			Create(tr, userData);
		}

		inline RigidbodyStatic::RigidbodyStatic(const TransformBase& tr, const Geometry& geo, const Material& mat, void* userData)
		{
			Create(tr, userData);
			CreateShape(geo, mat);
		}

		inline RigidbodyStatic::RigidbodyStatic(RigidbodyStatic&& other) : Rigidbody(std::move(other))
		{
		}

		inline RigidbodyStatic& RigidbodyStatic::operator=(RigidbodyStatic&& rhs)
		{
			Rigidbody::operator=(std::move(rhs));
		
			return *this;
		}
	}
}