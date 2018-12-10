namespace aw
{
	namespace sdk
	{
		inline bool SoundSystem::Register(Component* component)
		{
			if (component == nullptr)
				return false;

			if (component->typeId == TypeInfo<SoundListener>::id)
			{
				component->m_isUsed = true;

				SoundListener* listener = reinterpret_cast<SoundListener*>(component);

				s_mutex.lock();
				s_listeners.push_back(listener);
				s_mutex.unlock();

				listener->m_id = s_count++;

				AK::SoundEngine::RegisterGameObj(listener->m_id);
				AK::SoundEngine::AddDefaultListener(listener->m_id);

				return true;
			}
			else if (component->typeId == TypeInfo<SoundEmitter>::id)
			{
				component->m_isUsed = true;

				SoundEmitter* emitter = reinterpret_cast<SoundEmitter*>(component);

				s_mutex.lock();
				s_emitters.push_back(emitter);
				s_mutex.unlock();

				emitter->m_id = s_count++;

				AK::SoundEngine::RegisterGameObj(emitter->m_id);

				return true;
			}

			return false;
		}

		//inline void SoundSystem::Unregister(Component* component)
		//{
		//	if (component == nullptr)
		//		return;

		//	s_gcMutex.lock();

		//	if (component->typeId == TypeInfo<SoundListener>::id)
		//		s_listenersToDestroy.push_back(reinterpret_cast<SoundListener*>(component));
		//	else if (component->typeId != TypeInfo<SoundEmitter>::id)
		//		s_emittersToDestroy.push_back(reinterpret_cast<SoundEmitter*>(component));

		//	s_gcMutex.unlock();

		//	//for (auto it = s_listeners.begin(); it != s_listeners.end(); it++)
		//	//{
		//	//	if (*it == component)
		//	//	{
		//	//		AK::SoundEngine::RemoveDefaultListener((*it)->m_id);
		//	//		AK::SoundEngine::UnregisterGameObj((*it)->m_id);

		//	//		s_listeners.erase(it);

		//	//		return;
		//	//	}
		//	//}

		//	//for (auto it = s_emitters.begin(); it != s_emitters.end(); it++)
		//	//{
		//	//	if (*it == component)
		//	//	{
		//	//		AK::SoundEngine::UnregisterGameObj((*it)->m_id);
		//	//		s_emitters.erase(it);

		//	//		return;
		//	//	}
		//	//}
		//}

		inline AkSoundPosition SoundSystem::GetPosition(Component* component)
		{
			AkSoundPosition soundTr;

			const UniformTransform& transform = component->GlobalTransform();

			Quaternion trRot = transform.GetRotation();
			Vector3 forward = trRot * Vector3::Forward();
			forward.Normalize();

			Vector3 up = trRot * Vector3::Up();
			up.Normalize();

			Vector3 trPos = transform.GetPosition();
			AkVector pos = { trPos.x, trPos.y, trPos.z };
			AkVector front = { forward.x, forward.y, forward.z };
			AkVector top = { up.x, up.y, up.z };

			soundTr.Set(pos, front, top);
			return soundTr;
		}

		inline void SoundSystem::Update()
		{
			GarbageCollect();

			for (size_t i = 0; i < s_listeners.size(); ++i)
				AK::SoundEngine::SetPosition(s_listeners[i]->m_id, GetPosition(s_listeners[i]));
			for (size_t i = 0; i < s_emitters.size(); ++i)
				AK::SoundEngine::SetPosition(s_emitters[i]->m_id, GetPosition(s_emitters[i]));

			sound::Update();
		}
	}
}