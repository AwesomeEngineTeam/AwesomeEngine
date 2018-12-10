namespace aw
{
	namespace sdk
	{
		inline RaycastInfo::RaycastInfo(const aw::ph::RaycastHit& hit) :
			body{ hit.body ? hit.body->GetPhysicalComponent()->GetActor() : nullptr },
			comp{ hit.body ? hit.body->GetPhysicalComponent() : nullptr },
			distance{ hit.distance },
			position{ hit.position },
			normal{ hit.normal }
		{
		}

		inline RaycastInfo::RaycastInfo(RaycastInfo&& other) :
			body{ std::move(other.body) },
			comp{ std::move(other.comp) },
			distance{ std::move(other.distance) },
			position{ std::move(other.position) },
			normal{ std::move(other.normal) }
		{
		}

		inline RaycastInfo::RaycastInfo(const RaycastInfo& other) :
			body{ other.body },
			comp{ other.comp },
			distance{ other.distance },
			position{ other.position },
			normal{ other.normal }
		{
		}

		inline RaycastInfo& RaycastInfo::operator=(RaycastInfo&& rhs)
		{
			body = std::move(rhs.body);
			comp = std::move(rhs.comp);
			distance = std::move(rhs.distance);
			position = std::move(rhs.position);
			normal = std::move(rhs.normal);

			return *this;
		}

		inline RaycastInfo& RaycastInfo::operator=(const RaycastInfo& rhs)
		{
			body = rhs.body;
			comp = rhs.comp;
			distance = rhs.distance;
			position = rhs.position;
			normal = rhs.normal;

			return *this;
		}
	}
}