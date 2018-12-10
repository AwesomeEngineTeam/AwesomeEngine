#include <Awesome/SDK/System/PhysicalSystem.hpp>

#include <Awesome/SDK/Components/Render/Camera.hpp>
#include <Awesome/SDK/Time.hpp>

#include <Awesome/SDK/BaseApplication.hpp>

namespace aw
{
	namespace sdk
	{
		ph::Scene PhysicalSystem::s_scene;

		std::vector<PhysicalComponent*> PhysicalSystem::s_phComps;
		std::vector<PhysicalComponent*> PhysicalSystem::s_compsToInstantiate;

		std::mutex PhysicalSystem::s_instMutex;
		std::mutex PhysicalSystem::s_updateMutex;

		void PhysicalSystem::Start(const std::atomic_bool& shouldUpdate)
		{
			static std::function<void()> func = [&shouldUpdate]()
			{
				LOG_LOG("Physics thread id: %u", std::this_thread::get_id());
				
				float elapsedTime = 0;

				while (!BaseApplication::ShouldQuit())
				{
					float start = Time::GlobalTime();

					while ((shouldUpdate == false || (elapsedTime = Time::GlobalTime() - start) < s_fixedInterval) && !BaseApplication::ShouldQuit())
					{
						if (elapsedTime < 0.0f)
							start = Time::GlobalTime();

						InstantiatePendingComponents();
						GarbageCollect();

						std::this_thread::yield();
					}

					if (!BaseApplication::ShouldQuit())
						Update(elapsedTime);

					//LOG_LOG("Physics System took %lfs to update", elapsedTime);

				}
				LOG_LOG("Exitting Physics thread")
			};

			std::thread thr(func);
			thr.detach();
		}

		void PhysicalSystem::Update(float frameTime)
		{
			// Send Updated GlobalTr to ph::

			InstantiatePendingComponents();
			GarbageCollect();

			//std::unique_lock<std::mutex>(s_updateMutex);

			s_updateMutex.lock();

			for (uint32 index = 0u; index < s_phComps.size(); index++)
				s_phComps[index]->UpdateBuffering();

			s_scene.Simulate(frameTime);
			while (!s_scene.FetchResult(false))
					std::this_thread::yield();

			for (PhysicalComponent* body : s_phComps)
			{
				if (!body->IsActive())
					continue;

				UniformTransform currTr = body->GlobalTransform();
				currTr.scale = 1.0f; // remove scale for deltaTr.

				UniformTransform deltaTr = currTr.GetInversed() * body->GetUpdatedGlobalTransform();
				
				// Remove scale for deltaTr application.
				currTr = body->GetActor()->GetRootComponent()->GlobalTransform();
				float rootScale = currTr.scale;
				currTr.scale = 1.0f;

				currTr = currTr * deltaTr; // Apply deltaTr
				currTr.scale = rootScale; // reset Scale

				body->GetActor()->GetRootComponent()->SetGlobalTransform(currTr); // Set newTr
			}

			s_updateMutex.unlock();
		}

		void PhysicalSystem::GarbageCollect()
		{
			for (auto it = s_phComps.begin(); it != s_phComps.end(); ++it)
			{
				if ((*it)->GarbageCollectionRequired())
				{
					s_scene.RemoveActor(*(*it)->m_body);

					(*it)->m_isUsed = false;
					s_phComps.erase(it);

					return; //Lazy
				}
			}
		}

		void PhysicalSystem::InstantiatePendingComponents()
		{
			s_instMutex.lock();
			for(auto component : s_compsToInstantiate)
			{
				s_scene.AddActor(*(reinterpret_cast<PhysicalComponent*>(component)->m_body));
				s_phComps.push_back(reinterpret_cast<PhysicalComponent*>(component));

				//We need to do it at least once
				s_phComps.back()->UpdateBuffering();
			}

			s_compsToInstantiate.clear();
			s_instMutex.unlock();
		}

		AWESOME_SDK_API RaycastInfo PhysicalSystem::Raycast(const Vector3& origin, const Vector3& dir, float maxDist)
		{
			s_updateMutex.lock();

			auto res = s_scene.Raycast(origin, dir, maxDist);

			s_updateMutex.unlock();

			return res;

			//std::unique_lock<std::mutex>(s_updateMutex);
			//return s_scene.Raycast(origin, dir, maxDist);
		}

		AWESOME_SDK_API RaycastInfo PhysicalSystem::ScreenRaycast(Camera& cam, const Vector2& screenPos, float maxDist)
		{
			const Vector3& position = cam.GlobalPosition();
			Vector3 target = cam.ScreenToSpace(screenPos);

			//std::unique_lock<std::mutex>(s_updateMutex);

			s_updateMutex.lock();

			auto res = s_scene.Raycast(position, (target - position).GetNormalized(), maxDist);

			s_updateMutex.unlock();

			return res;

			//return s_scene.Raycast(position, (target - position).GetNormalized(), maxDist);
		}
	}
}