namespace aw
{
	namespace sdk
	{
		inline Skybox::Skybox(Actor* actor) :
			MeshRenderer(actor, uint32(ComponentID::Skybox), "Unnamed Skybox")
		{
		}
	}
}