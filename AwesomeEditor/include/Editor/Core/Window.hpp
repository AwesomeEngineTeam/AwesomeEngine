#pragma once

#ifndef AWESOME_EDITOR_CORE_WINDOW_GUARD
#define AWESOME_EDITOR_CORE_WINDOW_GUARD

#include <QtCore/qsettings.h>

#include <QtWidgets/qmainwindow.h>

#include <Editor/Core/Windowbar.hpp>

#include <Editor/Core/GameView.hpp>
#include <Editor/Core/ActorView.hpp>
#include <Editor/Core/FolderView.hpp>
#include <Editor/Core/AssetsView.hpp>

namespace ed
{
	class Window : public QMainWindow
	{
	public:
		WindowBar bar;

		GameView gameView;
		ActorView actorView;
		ComponentView compView;
		Inspector inspec;
		FolderView folderView;
		AssetsView assetsView;

		bool m_isMaximizeOnPlay = false;

		Window();
		~Window() = default;

		aw::RenderWindow& GetGameWindow();

		void SetOnGame(bool b = true);
		void SetMaximizeOnPlay(bool b = true);

		void PickComponent(aw::sdk::Component* comp);

		void RenameActor(aw::sdk::Actor* actor, const std::string& name);
		void RenameComponent(aw::sdk::Component* comp, const std::string& name);

		void SetOnInspectComponentCallback(const std::function<void(aw::sdk::Component*)>&);
		void SetOnClearInspectorCallback(const std::function<void()>&);

		void SwitchFullScreen();
		void ResetLayout();

		inline QSize sizeHint() const override;

		QString GetAssetName(aw::uint32, AssetsView::AssetsType) const;
		aw::uint32 GetAssetIndex(const QString&) const;

	protected:
		void closeEvent(QCloseEvent* event) override;
	};
}

#endif // GUARD