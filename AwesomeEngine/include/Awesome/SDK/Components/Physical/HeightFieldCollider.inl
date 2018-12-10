namespace aw
{
	namespace sdk
	{
		inline HeightFieldCollider::HeightFieldCollider(Actor* actor, Vector3 scale) :
			Collider(actor, uint32(ComponentID::HeightField), ph::HeightFieldGeometry(std::make_shared<ph::HeightField>(ph::HeightFieldDescriptor()), scale), "Unnamed HeightField Collider")
		{
			ASSERT(true, "HeightFieldCollider not implemented yet!");
		}
	}
}