#pragma once

#ifndef AWESOME_EDITOR_CORE_WINDOW_BAR_GUARD
#define AWESOME_EDITOR_CORE_WINDOW_BAR_GUARD

#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qmenu.h>

namespace ed
{
	class Window;

	class WindowBar
	{
		QAction* m_maxPlayButton = nullptr;

	public:
		WindowBar(Window& win);

		void SetOnGame(bool b);
	};
}

#endif // GUARD