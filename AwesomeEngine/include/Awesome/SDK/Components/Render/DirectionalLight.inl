namespace aw
{
	namespace sdk
	{
		inline DirectionalLight::DirectionalLight(Actor* actor) :
			Light(actor, uint32(ComponentID::DirectionalLight), "Unnamed DirectionalLight")
		{
		}
	}
}