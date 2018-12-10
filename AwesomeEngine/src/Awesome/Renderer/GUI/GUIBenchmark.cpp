#include <Awesome/Renderer/GUI/GUIBenchmark.hpp>

#include <imgui/imgui.h>

namespace aw
{
	GUIBenchmark::GUIBenchmark(float* frameTime) : m_frameTime{frameTime}
	{
	}

	void GUIBenchmark::Update()
	{
		bool b = true;
		ImGui::Begin("Performances", &b, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("FrameTime: %fms", *m_frameTime);
		ImGui::End();
	}
}