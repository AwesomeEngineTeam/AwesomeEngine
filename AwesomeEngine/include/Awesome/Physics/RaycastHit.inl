namespace aw
{
	namespace ph
	{
		inline RaycastHit::RaycastHit(Rigidbody* _body, float dist, const Vector3& pos, const Vector3& norm) :
			body{ _body },
			distance{ dist },
			position{ pos },
			normal{ norm }
		{
		}

		inline RaycastHit::RaycastHit(RaycastHit&& other) :
			body{ std::move(other.body) },
			distance{ std::move(other.distance) },
			position{ std::move(other.position) },
			normal{ std::move(other.normal) }
		{
		}

		inline RaycastHit::RaycastHit(const RaycastHit& other) :
			body{ other.body },
			distance{ other.distance },
			position{ other.position },
			normal{ other.normal }
		{
		}

		inline RaycastHit& RaycastHit::operator=(RaycastHit&& rhs)
		{
			body = std::move(rhs.body);
			distance = std::move(rhs.distance);
			position = std::move(rhs.position);
			normal = std::move(rhs.normal);

			return *this;
		}

		inline RaycastHit& RaycastHit::operator=(const RaycastHit& rhs)
		{
			body = rhs.body;
			distance = rhs.distance;
			position = rhs.position;
			normal = rhs.normal;

			return *this;
		}
	}
}