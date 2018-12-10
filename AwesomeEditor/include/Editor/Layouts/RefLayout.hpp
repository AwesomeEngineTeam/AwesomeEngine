#pragma once

#ifndef AWESOME_EDITOR_LAYOUTS_REF_LAYOUT_GUARD
#define AWESOME_EDITOR_LAYOUTS_REF_LAYOUT_GUARD

#include <functional>

#include <QtGui/qevent.h>
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qformlayout.h>

#include <Editor/Layouts/BoolLayout.hpp>
#include <Editor/Layouts/IntLayout.hpp>
#include <Editor/Layouts/FloatLayout.hpp>
#include <Editor/Layouts/TextLayout.hpp>

#include <Awesome/SDK/Components/Component.hpp>

namespace ed
{
	class RefLayout : public QFormLayout
	{
	protected:
		QLineEdit* m_fieldName = nullptr;

	public:
		std::function<void(const std::string& name)> onFieldNameChange;

		RefLayout(const std::string& name, QWidget* parent = nullptr);

		std::string GetFieldName() const;

		QSize sizeHint() const override;
	};


	class BoolRefLayout : public RefLayout
	{
	public:
		BoolLayout* boolLayout = nullptr;

		BoolRefLayout(const std::string& name, bool isCheck = true, QWidget* parent = nullptr);
	};

	class IntRefLayout : public RefLayout
	{
	public:
		IntLayout* intLayout = nullptr;
		IntRefLayout(const std::string& name, int value = 0, QWidget* parent = nullptr);
	};

	class FloatRefLayout : public RefLayout
	{
	public:
		FloatLayout* floatLayout = nullptr;
		FloatRefLayout(const std::string& name, float value = 0.0f, QWidget* parent = nullptr);
	};

	class TextRefLayout : public RefLayout
	{
	public:
		TextLayout * textLayout = nullptr;
		TextRefLayout(const std::string& name, const std::string& value, QWidget* parent = nullptr);
	};

	class ComponentRefLayout : public RefLayout
	{
		aw::sdk::Component* m_comp;

		class ComponentLineEdit : public QLineEdit
		{
			ComponentRefLayout& m_compLayout;

		public:
			ComponentLineEdit(ComponentRefLayout& layout);

		protected:
			void contextMenuEvent(QContextMenuEvent* event) override;
		};

		ComponentLineEdit* m_compName = nullptr;

		void OnFieldNameChange();

	public:
		std::function<void(aw::sdk::Component* comp)> onComponentChange;

		ComponentRefLayout(const std::string& name, aw::sdk::Component* comp, QWidget* parent = nullptr);

		void Update();
		void SetComponentValue(aw::sdk::Component* comp);
	};
}

#endif // GUARD