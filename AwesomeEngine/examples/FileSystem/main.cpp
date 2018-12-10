#include <Awesome/Core.hpp>

using namespace aw;

int main()
{
	try
	{
		LOG_LOG("Test");
	}
	catch (const std::function<void()>& catchFunc)
	{
		catchFunc();
		system("pause");

		return -1;
	}

	//system("pause");

	return 0;
}