namespace aw
{
	namespace ph
	{
		inline void Context::Init()
		{
			foundation.Create(allocatorCallback, debugCallback);
			pvd.Create();
			instance.Create();
			cooking.Create();
			extensions.Create();
			gpuContext.Create();
		}

		inline void Context::UnInit()
		{
			gpuContext.Destroy();
			extensions.Destroy();
			cooking.Destroy();
			instance.Destroy();
			pvd.Destroy();
			foundation.Destroy();
		}
	}
}