#include <Awesome/SDK/Scene/SceneManager.hpp>
#include <fstream>

namespace aw
{
	namespace sdk
	{
		std::vector<std::string> SceneManager::s_scenes;
		uint32 SceneManager::s_currentSceneId = ~0u;
		std::unique_ptr<Scene> SceneManager::CurrentScene;

		void SceneManager::CreateScene(const std::string& sceneName)
		{
			AddScene(sceneName);
			std::ofstream saveFile;
			std::string fileName = SCENE_PATH + sceneName + SCENE_EXT;

			CreateDirectory(SCENE_PATH, nullptr);

			saveFile.open(fileName);
			saveFile.close();

			if (CurrentScene.get() == nullptr)
				CurrentScene = std::make_unique<Scene>(sceneName);
		}

		void SceneManager::SaveScene()
		{
			if (CurrentScene.get() == nullptr || s_currentSceneId == ~0u)
				return;

			std::ofstream saveFile;
			std::string fileName = SCENE_PATH + CurrentScene->Name() + SCENE_EXT;

			CreateDirectory(SCENE_PATH, nullptr);

			saveFile.open(fileName);

			reflect::TypeDescriptor* typeDesc = reflect::TypeResolver<sdk::Scene>::Get();
			typeDesc->Dump(CurrentScene.get(), saveFile);

			saveFile.close();
		}

		std::unique_ptr<Scene> SceneManager::LoadFromFile(const std::string& sceneFile)
		{
			// TODO : DO
			return std::move(CurrentScene);
		}
	}
}