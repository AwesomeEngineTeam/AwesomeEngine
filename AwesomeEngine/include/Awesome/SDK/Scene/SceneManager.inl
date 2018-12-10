namespace aw
{
	namespace sdk
	{
		inline int SceneManager::AddScene(const std::string& sceneName)
		{
			uint32 i = 0;
			for (; i < s_scenes.size(); ++i)
			{
				if (s_scenes[i] == sceneName)
					return i;
			}

			s_scenes.emplace_back(sceneName);
			return i;
		}

		inline void SceneManager::LoadScene(const std::string& sceneName)
		{
			LOG_LOG(sceneName.c_str());
			s_currentSceneId = AddScene(sceneName);
			std::string fileName = SCENE_PATH + std::string(sceneName) + SCENE_EXT;
			CurrentScene = LoadFromFile(fileName.c_str());
		}

		inline void SceneManager::LoadScene(uint32 id)
		{
			s_currentSceneId = id;
			std::string fileName = SCENE_PATH + std::string(s_scenes[id]) + SCENE_EXT;
			CurrentScene = LoadFromFile(fileName.c_str());
		}

		inline void SceneManager::LoadNextScene()
		{
			if (++s_currentSceneId < s_scenes.size())
				LoadScene(s_currentSceneId);
			else
			{
				s_currentSceneId = ~0u;
				LOG_WARNING("LoadNextScene called but the actual scene is the last scene");
			}
		}

		inline void SceneManager::Start()
		{
			if (s_scenes.empty())
			{
				LOG_WARNING("No Scene added to SceneManager.");
				return;
			}

			s_currentSceneId = 0;

		#if WITH_EDITOR
			CurrentScene.get()->onSceneStart(CurrentScene.get());
		#endif
		}

		inline Actor* SceneManager::FindActorById(uint32 id)
		{
			if (CurrentScene != nullptr)
				return CurrentScene->FindActorById(id);

			LOG_WARNING("Current scene is undefined. Could not find actor %u", id);
			return nullptr;
		}

		inline Actor* SceneManager::FindActorWithName(const std::string& name)
		{
			if (CurrentScene != nullptr)
				return CurrentScene->FindActorWithName(name);

			LOG_WARNING("Current scene is undefined. Could not find actor %s", name.c_str());
			return nullptr;
		}

		inline std::vector<Actor*> SceneManager::FindActorsWithName(const std::string& name)
		{
			if (CurrentScene != nullptr)
				return CurrentScene->FindActorsWithName(name);

			LOG_WARNING("Current scene is undefined. Could not find actor %s", name.c_str());
			return std::vector<Actor*>();
		}

		inline void SceneManager::RemoveActor(Actor* actor)
		{
			if (CurrentScene != nullptr)
				CurrentScene->RemoveActor(actor);
			else
				LOG_WARNING("Current scene is undefined. Could not remove actor %s", actor->name.c_str());
		}
	}
}
