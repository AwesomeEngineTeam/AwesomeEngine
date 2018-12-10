namespace aw
{
	namespace sdk
	{
		inline Light::Light(Actor* actor, uint32 typeId, const std::string& name) :
			Component(actor, typeId, name)
		{
		}

		inline Light::Light(Actor* actor) :
			Light(actor, uint32(ComponentID::Light), "Unnamed Light")
		{
		}
	}
}