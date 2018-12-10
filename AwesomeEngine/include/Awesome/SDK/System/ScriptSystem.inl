namespace aw
{
	namespace sdk
	{
		inline void ScriptSystem::ProcessEvents()
		{
			s_eventMutex.lock();
			for (size_t i = 0; i < s_events.size(); ++i)
				s_events[i]();

			s_events.clear();
			s_eventMutex.unlock();
		}

		inline bool ScriptSystem::Register(Component* component)
		{
			if (component == nullptr)
				return false;

			if (component->typeId == TypeInfo<Script>::id)
			{
				component->m_isUsed = true;

				s_mutex.lock();
				s_scripts.push_back(reinterpret_cast<Script*>(component));
				s_mutex.unlock();

				return true;
			}

			return false;
		}

		//inline void ScriptSystem::Unregister(Component* component)
		//{
		//	if (component == nullptr || component->typeId != TypeInfo<Script>::id)
		//		return;

		//	//for (auto it = s_scripts.begin(); it != s_scripts.end(); it++)
		//	//{
		//	//	if (*it == component)
		//	//	{
		//	//		s_scripts.erase(it);

		//	//		return;
		//	//	}
		//	//}

		//	s_gcMutex.lock();
		//	s_scriptsToDestroy.push_back(reinterpret_cast<Script*>(component));
		//	s_gcMutex.unlock();
		//}

		inline void ScriptSystem::OnSceneStart()
		{
			for (int i = 0; i < s_scripts.size(); ++i)
				s_scripts[i]->Run();

			for (int i = 0; i < s_scripts.size(); ++i)
				s_scripts[i]->Start();
		}

		inline void ScriptSystem::Update(float elapsedTime)
		{
			GarbageCollect();

			for (int i = 0; i < s_scripts.size(); ++i)
			{
				if(s_scripts[i]->IsActive())
					s_scripts[i]->Update(elapsedTime);
			}

			ProcessEvents();
		}

		inline void ScriptSystem::AddEvent(const std::function<void()>& event)
		{
			s_eventMutex.lock();
			s_events.emplace_back(event);
			s_eventMutex.unlock();
		}
	}
}