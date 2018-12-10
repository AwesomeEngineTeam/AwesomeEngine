#include <QtWidgets/qtoolbar.h>

#include <Editor/Core/EditorApplication.hpp>

namespace ed
{
	WindowBar::WindowBar(Window& win)
	{
		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());

		// Icons
		QIcon playIcon("assetsEditor/icons/playIcon.png");
		QIcon stopIcon("assetsEditor/icons/stopIcon.png");
		QIcon undoIcon("assetsEditor/icons/undoIcon.png");
		QIcon redoIcon("assetsEditor/icons/redoIcon.png");
		QIcon fullscreenIcon("assetsEditor/icons/fullscreenIcon.png");
		QIcon maximizeIcon("assetsEditor/icons/maximizeIcon.png");
		QIcon translationIcon("assetsEditor/icons/translationIcon.png");
		QIcon rotationIcon("assetsEditor/icons/rotationIcon.png");
		QIcon scaleIcon("assetsEditor/icons/scaleIcon.png");
		QIcon absoluteIcon("assetsEditor/icons/absoluteIcon.png");
		QIcon relativeIcon("assetsEditor/icons/relativeIcon.png");
		QIcon plusIcon("assetsEditor/icons/plusButtonIcon.png");
		QIcon saveIcon("assetsEditor/icons/saveIcon.png");
		
		// File Menu
		QMenu* fileMenu = win.menuBar()->addMenu("&File");

		QAction* newAct = new QAction(plusIcon, "&New", &win);
		newAct->setShortcut(QKeySequence::New);
		newAct->setStatusTip("Create a new Scene");
		QWidget::connect(newAct, &QAction::triggered, editor, &EditorApplication::NewScene);
		fileMenu->addAction(newAct);

		QAction* saveAct = new QAction(saveIcon, "&Save", &win);
		saveAct->setShortcut(QKeySequence::Save);
		saveAct->setStatusTip("Save Current Scene");
		QWidget::connect(saveAct, &QAction::triggered, editor, &EditorApplication::SaveScene);
		fileMenu->addAction(saveAct);
		saveAct->setEnabled(false); // Currently disable

		fileMenu->addSeparator();

		QAction* quitAct = new QAction(QIcon(), "&QuitApp", &win);
		quitAct->setShortcut(QKeySequence::Quit);
		quitAct->setStatusTip("Exit Application");
		QWidget::connect(quitAct, &QAction::triggered, editor, &EditorApplication::QuitApp);
		fileMenu->addAction(quitAct);

		// Edit Menu
		QMenu* editMenu = win.menuBar()->addMenu("&Edit");

		QAction* undoAct = new QAction(undoIcon, "&Undo", &win);
		undoAct->setShortcut(QKeySequence::Undo);
		undoAct->setStatusTip("Reverse your last action");
		//QWidget::connect(undoAct, &QAction::triggered, editor, &EditorApplication::Undo);
		editMenu->addAction(undoAct);
		undoAct->setEnabled(false); // Currently disable

		QAction* redoAct = new QAction(redoIcon, "&Redo", &win);
		redoAct->setShortcut(QKeySequence::Redo);
		redoAct->setStatusTip("Reverse your last Undo");
		//QWidget::connect(redoAct, &QAction::triggered, editor, &EditorApplication::Redo);
		editMenu->addAction(redoAct);
		redoAct->setEnabled(false); // Currently disable

		editMenu->addSeparator();

		QAction* playAct = new QAction(playIcon, "&Play", &win);
		playAct->setShortcut(QKeySequence("Ctrl+P"));
		playAct->setStatusTip("Launch Play Mode");
		QWidget::connect(playAct, &QAction::triggered, editor, &EditorApplication::Play);
		editMenu->addAction(playAct);

		QAction* stopAct = new QAction(stopIcon, "&Stop", &win);
		stopAct->setShortcut(QKeySequence("Ctrl+Shift+P"));
		stopAct->setStatusTip("Stop Play Mode");
		QWidget::connect(stopAct, &QAction::triggered, editor, &EditorApplication::Stop);
		editMenu->addAction(stopAct);

		// Tools Menu
		QMenu* toolsMenu = win.menuBar()->addMenu("&Tools");

		QAction* optAct = new QAction(QIcon(), "&Option", &win);
		optAct->setShortcut(QKeySequence("Ctrl+Shift+O"));
		optAct->setStatusTip("Option");
		//QWidget::connect(optAct, &QAction::triggered, editor, &EditorApplication::Option);
		toolsMenu->addAction(optAct);
		optAct->setEnabled(false); // Currently disable

		QAction* translationToolAct = new QAction(translationIcon, "&Set Translation Tool", &win);
		translationToolAct->setShortcut(QKeySequence("Ctrl+Shift+Q"));
		translationToolAct->setStatusTip("Set the current tool to Translation Tool");
		QWidget::connect(translationToolAct, &QAction::triggered, editor, &EditorApplication::SetTranslationTool);
		toolsMenu->addAction(translationToolAct);

		QAction* rotationToolAct = new QAction(rotationIcon, "&Set Rotation Tool", &win);
		rotationToolAct->setShortcut(QKeySequence("Ctrl+Shift+W"));
		rotationToolAct->setStatusTip("Set the current tool to Rotation Tool");
		QWidget::connect(rotationToolAct, &QAction::triggered, editor, &EditorApplication::SetRotationTool);
		toolsMenu->addAction(rotationToolAct);

		QAction* scaleToolAct = new QAction(scaleIcon, "&Set Scale Tool", &win);
		scaleToolAct->setShortcut(QKeySequence("Ctrl+Shift+E"));
		scaleToolAct->setStatusTip("Set the current tool to Scale Tool");
		QWidget::connect(scaleToolAct, &QAction::triggered, editor, &EditorApplication::SetScaleTool);
		toolsMenu->addAction(scaleToolAct);

		QAction* toolAbsoluteAct = new QAction(absoluteIcon, "&Set Tool absolute", &win);
		scaleToolAct->setShortcut(QKeySequence("Ctrl+Shift+A"));
		toolAbsoluteAct->setStatusTip("Set the tool to be absolute");
		QWidget::connect(toolAbsoluteAct, &QAction::triggered, editor, &EditorApplication::SetToolAbsolute);
		toolsMenu->addAction(toolAbsoluteAct);

		QAction* toolRelativeAct = new QAction(relativeIcon, "&Set Tool relative", &win);
		scaleToolAct->setShortcut(QKeySequence("Ctrl+Shift+S"));
		toolRelativeAct->setStatusTip("Set the tool to be relative");
		QWidget::connect(toolRelativeAct, &QAction::triggered, editor, &EditorApplication::SetToolRelative);
		toolsMenu->addAction(toolRelativeAct);

		// View Menu
		QMenu* viewMenu = win.menuBar()->addMenu("&View");

		QAction* fullScreenAct = new QAction(fullscreenIcon, "&FullScren", &win);
		fullScreenAct->setShortcut(QKeySequence::FullScreen);
		fullScreenAct->setStatusTip("Set win FullScreen");
		QWidget::connect(fullScreenAct, &QAction::triggered, &win, &Window::SwitchFullScreen);
		viewMenu->addAction(fullScreenAct);

		QAction* resetLayoutAct = new QAction(QIcon(), "&Reset Layout", &win);
		resetLayoutAct->setShortcut(QKeySequence("Ctrl+Shift+Y"));
		resetLayoutAct->setStatusTip("Reset win layout");
		QWidget::connect(resetLayoutAct, &QAction::triggered, &win, &Window::ResetLayout);
		viewMenu->addAction(resetLayoutAct);
		resetLayoutAct->setEnabled(false);

		// Help Menu
		QMenu* helpMenu = win.menuBar()->addMenu("&Help");
		helpMenu->setEnabled(false); // Currently disable

		// ToolBar
		QToolBar* toolbar = win.addToolBar("ToolBar");
		toolbar->setObjectName("ToolBar");
		toolbar->setMovable(false);

		// Tools
		QAction* translButton = toolbar->addAction("&Translation Tool");
		translButton->setIcon(translationIcon);
		QWidget::connect(translButton, &QAction::triggered, editor, &EditorApplication::SetTranslationTool);

		QAction* rotButton = toolbar->addAction("&Rotation Tool");
		rotButton->setIcon(rotationIcon);
		QWidget::connect(rotButton, &QAction::triggered, editor, &EditorApplication::SetRotationTool);

		QAction* scaleButton = toolbar->addAction("&Scale Tool");
		scaleButton->setIcon(scaleIcon);
		QWidget::connect(scaleButton, &QAction::triggered, editor, &EditorApplication::SetScaleTool);

		QAction* absoluteToolButton = toolbar->addAction("&Absolute Tools");
		absoluteToolButton->setIcon(absoluteIcon);
		QWidget::connect(absoluteToolButton, &QAction::triggered, editor, &EditorApplication::SetToolAbsolute);

		QAction* relativeToolButton = toolbar->addAction("&Relative Tools");
		relativeToolButton->setIcon(relativeIcon);
		QWidget::connect(relativeToolButton, &QAction::triggered, editor, &EditorApplication::SetToolRelative);

		// Left Space
		QWidget* empty1 = new QWidget();
		empty1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
		toolbar->addWidget(empty1);

		// Actions
		QAction* playButton = toolbar->addAction("&Play");
		playButton->setIcon(playIcon);
		QWidget::connect(playButton, &QAction::triggered, editor, &EditorApplication::Play);

		QAction* stopButton = toolbar->addAction("&Stop");
		stopButton->setIcon(stopIcon);
		QWidget::connect(stopButton, &QAction::triggered, editor, &EditorApplication::Stop);

		m_maxPlayButton = toolbar->addAction("&Maximize on Play");
		m_maxPlayButton->setIcon(maximizeIcon);
		m_maxPlayButton->setCheckable(true);
		QWidget::connect(m_maxPlayButton, &QAction::triggered, &win, &Window::SetMaximizeOnPlay);

		// Right Space
		QWidget* empty2 = new QWidget();
		empty2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
		toolbar->addWidget(empty2);

		// Scene
		QAction* saveButton = toolbar->addAction("&Save");
		saveButton->setIcon(saveIcon);
		QWidget::connect(saveButton, &QAction::triggered, editor, &EditorApplication::SaveScene);

		QAction* newButton = toolbar->addAction("&New");
		newButton->setIcon(plusIcon);
		QWidget::connect(newButton, &QAction::triggered, editor, &EditorApplication::NewScene);
	}

	void WindowBar::SetOnGame(bool b)
	{
		m_maxPlayButton->setDisabled(b);
	}
}