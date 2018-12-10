#include <Awesome/SDK/System/SoundSystem.hpp>

#include <Awesome/SDK/BaseApplication.hpp>

namespace aw
{
	namespace sdk
	{
		decltype(SoundSystem::s_count) SoundSystem::s_count = 0;

		std::vector<SoundListener*>  SoundSystem::s_listeners;
		std::vector<SoundEmitter*>  SoundSystem::s_emitters;

		std::mutex SoundSystem::s_mutex;

		void SoundSystem::GarbageCollect()
		{
			s_mutex.lock();
			for (auto it = s_listeners.begin(); it != s_listeners.end(); ++it)
			{
				if (!(*it)->GarbageCollectionRequired())
					continue;

				AK::SoundEngine::RemoveDefaultListener((*it)->m_id);
				AK::SoundEngine::UnregisterGameObj((*it)->m_id);

				(*it)->m_isUsed = false;

				s_listeners.erase(it);

				break; //Lazy
			}

			for (auto it = s_emitters.begin(); it != s_emitters.end(); ++it)
			{
				if (!(*it)->GarbageCollectionRequired())
					continue;

				AK::SoundEngine::UnregisterGameObj((*it)->m_id);

				(*it)->m_isUsed = false;
				s_emitters.erase(it);

				break; //Lazy
			}
			s_mutex.unlock();
		}

		void SoundSystem::Start(std::atomic_bool& shouldUpdate)
		{
			static std::function<void()> func = [&shouldUpdate]()
			{
				LOG_LOG("Sound thread id: %u", std::this_thread::get_id());
				while (!BaseApplication::ShouldQuit())
				{
					if (shouldUpdate)
					{
						//static Chrono chrono;
						//chrono.Restart();

						Update();
						shouldUpdate = false;

						//LOG_LOG("Sound System took %lfs to update", chrono.GetElapsedTime<Chrono::Seconds>());
					}
					else
					{
						GarbageCollect();
						std::this_thread::yield();
					}
				}
				LOG_LOG("Exitting Sound thread")
			};

			std::thread thr(func);
			thr.detach();
		}
	}
}