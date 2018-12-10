#include <Awesome/SDK/ThreadManager.hpp>

namespace aw
{
	namespace sdk
	{
		ThreadQueue<7> ThreadManager::s_threadQueue{};

		void ThreadManager::JoinAll()
		{
			s_threadQueue.JoinAll();
		}
	}
}