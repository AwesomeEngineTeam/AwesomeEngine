#include <Editor/Core/EditorApplication.hpp>

namespace ed
{
	Window::Window() :
		bar(*this),
		gameView(*this),
		actorView(*this),
		compView(*this),
		inspec(*this),
		folderView(*this),
		assetsView(*this)
	{
		setWindowState(windowState().setFlag(Qt::WindowMaximized, true));
		setWindowIcon(QIcon("assetsEditor/icons/AwesomeLogo.png"));

		setCentralWidget(&gameView);

		addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, &inspec);

		addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, &actorView);
		addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, &compView);

		addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, &folderView);
		addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, &assetsView);

		resizeDocks({ &compView, &inspec, &assetsView }, { 250, 250, 100 }, Qt::Horizontal);
		resizeDocks({ &compView, &assetsView }, { 115, 215 }, Qt::Vertical);

		setDockNestingEnabled(true);

		QSettings settings("Awesome", "Editor");
		settings.setValue("geometry", saveGeometry());
		settings.setValue("state", saveState());
	}

	aw::RenderWindow& Window::GetGameWindow()
	{
		return *gameView.gameWindow.get();
	}

	void Window::SetOnGame(bool b)
	{
		bar.SetOnGame(b);
		gameView.SetOnGame(b);

		if (m_isMaximizeOnPlay)
		{
			if (b)
			{
				compView.hide();
				actorView.hide();
				folderView.hide();
				assetsView.hide();
				inspec.hide();
			}
			else
			{
				compView.show();
				actorView.show();
				folderView.show();
				assetsView.show();
				inspec.show();
			}
		}
		else
		{
			compView.SetOnGame(b);
			actorView.SetOnGame(b);

			folderView.setDisabled(b);
			assetsView.setDisabled(b);
			inspec.SetOnGame(b);
		}
	}

	void Window::SetMaximizeOnPlay(bool b)
	{
		m_isMaximizeOnPlay = b;
	}

	void Window::PickComponent(aw::sdk::Component* comp)
	{
		actorView.SetSelectedActor(comp->GetActor());
		compView.SetSelectedActor(comp->GetActor(), comp);
	}

	void Window::RenameActor(aw::sdk::Actor* actor, const std::string& name)
	{
		actorView.RenameActor(actor, name);
	}

	void Window::RenameComponent(aw::sdk::Component* comp, const std::string& name)
	{
		compView.RenameComponent(comp, name);
	}

	void Window::SetOnInspectComponentCallback(const std::function<void(aw::sdk::Component*)>& callback)
	{
		inspec.onInspectComponent = callback;
	}

	void Window::SetOnClearInspectorCallback(const std::function<void()>& callback)
	{
		inspec.onClearInspector = callback;
	}

	void Window::SwitchFullScreen()
	{
		setWindowState(windowState().setFlag(Qt::WindowFullScreen, !isFullScreen()));
	}

	void Window::ResetLayout()
	{
		setCentralWidget(&compView);

		QSettings settings("Awesome", "Editor");
		restoreGeometry(settings.value("geometry").toByteArray());
		restoreState(settings.value("state").toByteArray());
	}

	QSize Window::sizeHint() const
	{
		return QSize(800, 600);
	}

	QString Window::GetAssetName(aw::uint32 index, AssetsView::AssetsType type) const
	{
		return assetsView.GetAssetName(index, type);
	}

	aw::uint32 Window::GetAssetIndex(const QString& str) const
	{
		return assetsView.GetAssetIndex(str);
	}

	void Window::closeEvent(QCloseEvent* event)
	{
		dynamic_cast<EditorApplication*>(QApplication::instance())->Quit();

		event->accept();
	}
}