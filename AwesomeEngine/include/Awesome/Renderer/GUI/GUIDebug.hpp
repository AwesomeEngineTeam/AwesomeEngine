#pragma once

#ifndef AWESOME_RENDERER_GUI_DEBUG_GUARD
#define AWESOME_RENDERER_GUI_DEBUG_GUARD

#include <Awesome/Core/Logger/Logger.hpp>

#include <Awesome/Renderer/GUI/GUIBase.hpp>

#include <sstream>

namespace aw
{
	class AWESOME_RENDERER_API GUIDebug : public GUIBase
	{
		//Output widget data
		bool m_outputWidgetActive = true;

		std::stringstream m_outputStream;

		std::vector<std::string> m_outputLines;
		size_t m_lastNbOutputLines = 0;

		//Memory widget data
		bool m_memoryWidgetActive = true;

		std::vector<int> m_selectedBlocks;
		int8 m_selectedPool = -1;

		void Update() override;

		void UpdateMemoryWidget();
		void UpdateOutputWidget();

	public:
		inline GUIDebug();
		GUIDebug(const GUIDebug& other) = delete;
		GUIDebug(GUIDebug&& other) = delete;
		~GUIDebug() = default;

		GUIDebug& operator=(const GUIDebug& rhs) = delete;
		GUIDebug& operator=(GUIDebug&& rhs) = delete;
	};
}

#include <Awesome/Renderer/GUI/GUIDebug.inl>

#endif //Guard