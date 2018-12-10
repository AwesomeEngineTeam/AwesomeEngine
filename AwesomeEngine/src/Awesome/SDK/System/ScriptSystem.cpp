#include <Awesome/SDK/System/ScriptSystem.hpp>

#include <Awesome/SDK/BaseApplication.hpp>

namespace aw
{
	namespace sdk
	{
		std::vector<Script*> ScriptSystem::s_scripts;
		std::vector<std::function<void()>> ScriptSystem::s_events;

		std::mutex ScriptSystem::s_mutex;
		std::mutex ScriptSystem::s_eventMutex;

		void ScriptSystem::GarbageCollect()
		{
			s_mutex.lock();
			for (auto it = s_scripts.begin(); it != s_scripts.end(); ++it)
			{
				if ((*it)->GarbageCollectionRequired())
				{
					(*it)->m_isUsed = false;

					(*it)->OnDestroy();
					s_scripts.erase(it);

					break; //Lazy
				}
			}
			s_mutex.unlock();
		}

		void ScriptSystem::Start(std::atomic_bool& shouldUpdate)
		{
			static std::function<void()> func = [&shouldUpdate]()
			{
				LOG_LOG("Script thread id: %u", std::this_thread::get_id());
				while (!BaseApplication::ShouldQuit())
				{
					if (shouldUpdate)
					{
						//static Chrono chrono;
						//chrono.Restart();

						Update(Time::DeltaTime());
						shouldUpdate = false;

						//LOG_LOG("Script System took %lfs to update", chrono.GetElapsedTime<Chrono::Seconds>());
					}
					else
					{
						GarbageCollect();
						std::this_thread::yield();
					}
				}
				LOG_LOG("Exitting Script thread")
			};

			std::thread thr(func);
			thr.detach();
		}
	}
}