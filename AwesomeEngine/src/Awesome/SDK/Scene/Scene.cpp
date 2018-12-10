#include <Awesome/SDK/Scene/Scene.hpp>

namespace aw
{
	namespace sdk
	{
#if WITH_EDITOR
		std::function<void(Scene*)> Scene::onSceneStart;
		std::function<void(Actor*, Actor*)> Scene::onCreateActor;
		std::function<void(Actor*)> Scene::onDestroyActor;
#endif

		void Scene::GarbageCollect()
		{
			m_root->GarbageCollect();
			//for (auto it = m_root->GetFirstChild(); it != m_root->GetLastChild(); ++it)
			//{
			//	if((*it)->GarbageCollect())
			//	{
			//		RemoveActorImmediate(*it);
			//		return; //Lazy
			//	}
			//}
		}
	}
}