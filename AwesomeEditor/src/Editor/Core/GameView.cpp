#include <Awesome/SDK.hpp>

#include <Editor/Core/EditorApplication.hpp>

namespace ed
{
	GameView::GameWindow::GameWindow(aw::RenderWindow* renderWindow, QWindow* parent) :
		QWindow(parent),
		m_renderWindow{ renderWindow }
	{
		setPosition({ 100000.0f, 100000.0f }); // set out of screen.
	}

	bool GameView::GameWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
	{
		if (m_renderWindow)
		{
			aw::RenderWindow& win = *m_renderWindow;

			MSG* msg = static_cast<MSG*>(message);

			if (msg->message == WM_INPUT)
				aw::Window::InputEvent(msg->wParam, msg->lParam);
			else if (msg->message == WM_SIZE)
				win->SizeEvent(msg->wParam, msg->lParam);
			else if (msg->message == WM_SYSKEYDOWN || msg->message == WM_KEYDOWN)
				aw::Window::KeyDownEvent(msg->wParam, msg->lParam);
			else if (msg->message == WM_SYSKEYUP || msg->message == WM_KEYUP)
				aw::Window::KeyUpEvent(msg->wParam, msg->lParam);
		}

		return QWindow::nativeEvent(eventType, message, result);
	}

	void GameView::GameWindow::mousePressEvent(QMouseEvent* event)
	{
		(*m_renderWindow)->SetFocused();

		QWindow::mousePressEvent(event);
	}

	void GameView::GameWindow::keyPressEvent(QKeyEvent* event)
	{
		if (event->modifiers() & Qt::Modifier::CTRL)
		{
			EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());

			if (event->modifiers() & Qt::Modifier::SHIFT)
			{
				if (event->key() == Qt::Key_Q)
					editor->SetTranslationTool();
				else if (event->key() == Qt::Key_W)
					editor->SetRotationTool();
				else if (event->key() == Qt::Key_E)
					editor->SetScaleTool();
				else if (event->key() == Qt::Key_A)
					editor->SetToolAbsolute();
				else if (event->key() == Qt::Key_S)
					editor->SetToolRelative();
				else if (event->key() == Qt::Key_P)
					editor->Stop();
			}
			else
			{
				if (event->key() == Qt::Key_P)
					editor->Play();
				else if (event->key() == Qt::Key_S)
					/*editor->SaveScene()*/;
			}

			event->accept();
		}
		else
			event->ignore();
	}

	GameView::GameView(Window& win) :
		QDockWidget("Scene", &win),
		gameWindow{ std::make_unique<aw::RenderWindow>() }
	{
		setObjectName("Game Scene");

		setFeatures(0); // 0 == !DockWidgetClosable && !DockWidgetMovable && !DockWidgetFloatable.

		GameWindow* temp = new GameWindow(gameWindow.get());
		gameWindow->Create(reinterpret_cast<HWND>(temp->winId()));
		//setWidget(QWidget::createWindowContainer(temp, this));

		// Trick to force Qt, without this, DockWidget borders are removed... Why...
		QWindow* qWin = QWindow::fromWinId(reinterpret_cast<WId>((*gameWindow)->GetHandle()));
		setWidget(QWidget::createWindowContainer(qWin, this));
	}

	GameView::~GameView()
	{
		if (gameWindow)
		{
			gameWindow->Quit();
			gameWindow.reset();
		}
	}

	void GameView::SetOnGame(bool b)
	{
		if(b)
			(*gameWindow)->SetFocused();
	}
}