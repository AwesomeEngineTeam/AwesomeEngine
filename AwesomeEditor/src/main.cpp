#include <Awesome/SDK.hpp>

#include <Editor/Core/EditorApplication.hpp>

using namespace ed;

int main(int argc, char **argv)
{
	try
	{
		aw::sdk::EngineRRID engine;

		EditorApplication app(argc, argv);
		app.Launch();
	}
	catch (const std::function<void()>& catchFunc)
	{
		catchFunc();
		system("pause");

		return -1;
	}

	return 0;
}
