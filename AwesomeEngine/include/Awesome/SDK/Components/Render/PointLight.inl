namespace aw
{
	namespace sdk
	{
		/*inline PointLight::PointLight(Actor* actor, uint32 typeId, const std::string& name) :
			Component(actor, uint32(ComponentID::PointLight), "unnamed point light")
		{
		}

		inline PointLight::PointLight(Actor* actor) :
			PointLight(actor, uint32(ComponentID::PointLight), "unnamed point light")
		{
		}*/

		inline PointLight::PointLight(Actor* actor) :
			Light(actor, uint32(ComponentID::PointLight), "Unnamed PointLight")
		{
		}
	}
}