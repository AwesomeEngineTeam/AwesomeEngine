namespace aw
{
	namespace sdk
	{
		inline EngineRRID::EngineRRID()
		{
			Context::Init();

			Window::Init();

			ph::Context::Init();

			aw::ph::SimulationEvent::onCollisionCallback = PhysicalComponent::CollisionCallback;
			aw::ph::SimulationEvent::onTriggerCallback = PhysicalComponent::TriggerCallback;

			sound::Init();
		}

		inline EngineRRID::~EngineRRID()
		{
			sound::UnInit();

			ph::Context::UnInit();

			Window::UnInit();

			Context::UnInit();
		}
	}
}