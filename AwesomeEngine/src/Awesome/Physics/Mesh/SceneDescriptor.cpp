#include <Awesome/Physics/Control/Context.hpp>
#include <Awesome/Physics/Descriptor/SceneDescriptor.hpp>

namespace aw
{
	namespace ph
	{
		SimulationEvent SceneDescriptor::s_simulationEventCallback;

		SceneDescriptor::SceneDescriptor(const Vector3& gravity, bool useGPU) : m_handle{ Context::instance.GetTolerancesScale() }
		{
			Create(gravity, useGPU);
		}

		void SceneDescriptor::Create(const Vector3& gravity, bool useGPU)
		{
			m_handle.gravity = reinterpret_cast<const physx::PxVec3&>(gravity);
			//m_handle.filterShader = physx::PxDefaultSimulationFilterShader;
			m_handle.filterShader = SampleFilterShader;
			m_handle.flags |= physx::PxSceneFlag::eENABLE_CCD;

			m_handle.simulationEventCallback = &s_simulationEventCallback;

			physx::PxCpuDispatcher* cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1); // Thread num = 1
			ASSERT(!cpuDispatcher, "Physcis: Scene::CpuDispatcher::Create failed!");
			m_handle.cpuDispatcher = cpuDispatcher;

			#if PX_WINDOWS && AWESOME_PHYSICS_USE_GPU
			if(useGPU)
				m_handle.gpuDispatcher = Context::gpuContext.Get()->getGpuDispatcher();
			#endif
		}
	}
}