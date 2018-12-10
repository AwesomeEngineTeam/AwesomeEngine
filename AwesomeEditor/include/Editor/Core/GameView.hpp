#pragma once

#ifndef AWESOME_EDITOR_CORE_GAME_VIEW_GUARD
#define AWESOME_EDITOR_CORE_GAME_VIEW_GUARD

#include <QtGui/qwindow.h>
#include <QtWidgets/qdockwidget.h>

#include <Awesome/Renderer/Control/RenderWindow.hpp>
#include <Awesome/Windowing.hpp>

namespace aw
{
	namespace sdk
	{
		class Camera;
	}
}

namespace ed
{
	class Window;

	class GameView : public QDockWidget
	{
		class GameWindow : public QWindow
		{
			aw::RenderWindow* m_renderWindow{ nullptr };

		public:
			GameWindow(aw::RenderWindow* renderWindow, QWindow* parent = nullptr);

		protected:
			bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

			void mousePressEvent(QMouseEvent* event) override;
			void keyPressEvent(QKeyEvent* event) override;
		};

	public:
		std::unique_ptr<aw::RenderWindow> gameWindow = nullptr;

		GameView(Window& win);
		~GameView();

		void SetOnGame(bool b = true);
	};
}

#endif // GUARD