#include <Editor/Layouts/RefLayout.hpp>
#include <Editor/Core/EditorApplication.hpp>

namespace ed
{
	RefLayout::RefLayout(const std::string& name, QWidget* parent) : QFormLayout(parent)
	{
		setSpacing(2);

		m_fieldName = new QLineEdit;
		m_fieldName->setText(name.c_str());
	}

	std::string RefLayout::GetFieldName() const
	{
		return m_fieldName->text().toStdString();
	}

	QSize RefLayout::sizeHint() const
	{
		return QSize(100, 20);
	}


	BoolRefLayout::BoolRefLayout(const std::string& name, bool isCheck, QWidget* parent) : RefLayout(name, parent)
	{
		boolLayout = new BoolLayout(m_fieldName, isCheck);
		addItem(boolLayout);
	}

	IntRefLayout::IntRefLayout(const std::string& name, int value, QWidget* parent) : RefLayout(name, parent)
	{
		intLayout = new IntLayout(m_fieldName, value);
		addItem(intLayout);
	}

	FloatRefLayout::FloatRefLayout(const std::string& name, float value, QWidget* parent) : RefLayout(name, parent)
	{
		floatLayout = new FloatLayout(m_fieldName, value);
		addItem(floatLayout);
	}

	TextRefLayout::TextRefLayout(const std::string& name, const std::string& value, QWidget* parent) : RefLayout(name, parent)
	{
		textLayout = new TextLayout(m_fieldName, value);
		addItem(textLayout);
	}

	ComponentRefLayout::ComponentLineEdit::ComponentLineEdit(ComponentRefLayout& layout) : m_compLayout{ layout }
	{
	}

	void ComponentRefLayout::ComponentLineEdit::contextMenuEvent(QContextMenuEvent* event)
	{
		event->accept();
		QMenu* menu = new QMenu();
		QAction* act = menu->addAction("&Paste");

		QWidget::connect(act, &QAction::triggered, this, [this]() {
			m_compLayout.SetComponentValue(dynamic_cast<EditorApplication*>(QApplication::instance())->copiedComponent);
		});

		menu->exec(event->globalPos());
		delete menu;
	}

	ComponentRefLayout::ComponentRefLayout(const std::string& name, aw::sdk::Component* comp, QWidget* parent) :
		RefLayout(name, parent),
		m_comp{ comp }
	{
		m_compName = new ComponentLineEdit(*this);
		m_compName->setText(m_comp ? m_comp->name.c_str() : "New Binding");

		m_compName->setReadOnly(true);

		connect(m_fieldName, &QLineEdit::editingFinished, this, &ComponentRefLayout::OnFieldNameChange);

		addRow(m_fieldName, m_compName);
	}

	void ComponentRefLayout::Update()
	{
		m_compName->setText(m_comp ? m_comp->name.c_str() : "");
	}

	void ComponentRefLayout::SetComponentValue(aw::sdk::Component* comp)
	{
		m_comp = comp;
		Update();

		if (onComponentChange)
			onComponentChange(comp);
	}

	void ComponentRefLayout::OnFieldNameChange()
	{
		if (onFieldNameChange)
			onFieldNameChange(m_fieldName->text().toUtf8().data());
	}
}