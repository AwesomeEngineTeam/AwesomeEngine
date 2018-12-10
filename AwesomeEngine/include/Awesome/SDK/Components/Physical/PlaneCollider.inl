namespace aw
{
	namespace sdk
	{
		inline PlaneCollider::PlaneCollider(Actor* actor) :
			Collider(actor, uint32(ComponentID::PlaneCollider), ph::PlaneGeometry(), "Unnamed PlaneCollider")
		{
		}
	}
}