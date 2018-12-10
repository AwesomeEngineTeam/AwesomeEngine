#pragma once

#ifndef AWESOME_EDITOR_WIDGETS_SCRIPT_WIDGET_GUARD
#define AWESOME_EDITOR_WIDGETS_SCRIPT_WIDGET_GUARD

#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qinputdialog.h>

#include <Editor/Widgets/DropWidget.hpp>
#include <Editor/Layouts/DropDownMenu.hpp>
#include <Editor/Widgets/ComponentWidget.hpp>
#include <Editor/Layouts/RefLayout.hpp>

namespace ed
{
	class ScriptWidget : public ComponentWidget
	{
		class PopupMenu : public QWidget
		{
			QInputDialog* m_input = nullptr;

		public:
			std::string type;
			std::string fieldName;
			bool isRejected = false;

			PopupMenu(QWidget* parent = nullptr);

		protected:
			void closeEvent(QCloseEvent* event);
		};

		DropWidget* m_drop = nullptr;
		QPushButton* m_addBinding = nullptr;

		void CreateBinding(const std::string& type, const std::string& fieldName);
		void RemoveBinding(RefLayout* layout);

	public:
		ScriptWidget(class aw::sdk::Script* comp, QWidget* parent = nullptr);
		
		void Update() override;

		void AddBindingLayout(RefLayout* layout);
	};
}

#endif // GUARD