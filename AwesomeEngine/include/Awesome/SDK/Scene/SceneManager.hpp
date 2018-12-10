#pragma once

#ifndef AWESOME_SDK_SCENE_MANAGER_GUARD
#define AWESOME_SDK_SCENE_MANAGER_GUARD

#include <functional>

#include <Awesome/SDK/Scene/Scene.hpp>

#define SCENE_PATH "assets/scenes/"
#define SCENE_EXT ".awscene"

namespace aw
{
	namespace sdk
	{
		class SceneManager
		{
			static std::vector<std::string> s_scenes;
			static uint32 s_currentSceneId;

		public:
			static std::unique_ptr<Scene> CurrentScene;

			SceneManager() = delete;
			~SceneManager() = delete;

			static void CreateScene(const std::string& sceneName);
			static void SaveScene();
			static inline void LoadScene(const std::string& sceneName);
			static inline void LoadScene(uint32 id);
			static inline void LoadNextScene();

		private:
			static inline int AddScene(const std::string& sceneName);
			static std::unique_ptr<Scene> LoadFromFile(const std::string& sceneFile);

		public:

			static inline void Start();

			// -----------------------------------------------------------------------------------------
			//Remove ? (Needed for lua for static access)
			static inline Actor* FindActorById(uint32 id);
			static inline Actor* FindActorWithName(const std::string& name);
			static inline std::vector<Actor*> FindActorsWithName(const std::string& name);

			static inline void RemoveActor(Actor* actor);
			// -----------------------------------------------------------------------------------------
		};
	}
}

#include <Awesome/SDK/Scene/SceneManager.inl>

#endif //GUARD