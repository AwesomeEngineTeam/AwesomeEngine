#pragma once

#ifndef AWESOME_RENDERER_GUI_BENCHMARK_GUARD
#define AWESOME_RENDERER_GUI_BENCHMARK_GUARD

#include <Awesome/Renderer/GUI/GUIBase.hpp>

namespace aw
{
	class AWESOME_RENDERER_API GUIBenchmark : public GUIBase
	{
		float* m_frameTime = nullptr;

	protected:
		void Update() override;

	public:
		GUIBenchmark() = delete;
		GUIBenchmark(float* frameTime);
		GUIBenchmark(const GUIBenchmark& other) = delete;
		GUIBenchmark(GUIBenchmark&& other) = delete;
		~GUIBenchmark() = default;

		GUIBenchmark& operator=(const GUIBenchmark& rhs) = delete;
		GUIBenchmark& operator=(GUIBenchmark&& rhs) = delete;
	};
}

#endif //Guard