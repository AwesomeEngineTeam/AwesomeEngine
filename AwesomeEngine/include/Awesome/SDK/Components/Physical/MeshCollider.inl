namespace aw
{
	namespace sdk
	{
		inline MeshCollider::MeshCollider(Actor* actor, const std::vector<Vector3>& vertices, const std::vector<uint32>& indices) :
			Collider(actor, uint32(ComponentID::MeshCollider), ph::ConvexMeshGeometry(std::make_shared<ph::ConvexMesh>(ph::ConvexMeshDescriptor(vertices, indices))), "Unnamed MeshRenderer")
		{
		}
	}
}