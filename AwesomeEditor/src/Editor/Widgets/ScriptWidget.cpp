#include <Editor/Widgets/ScriptWidget.hpp>

#include <Awesome/SDK/Components/Lua/Script.hpp>

namespace ed
{
	ScriptWidget::PopupMenu::PopupMenu(QWidget* parent) : QWidget(parent)
	{
		QFormLayout* layout = new QFormLayout;

		type = "Boolean";

		DropDownMenu* menu = new DropDownMenu("Type:");
		menu->AddItem("Boolean"); menu->AddItem("Integer"); menu->AddItem("Floating-Point"); menu->AddItem("String"); menu->AddItem("Component");
		menu->onValueChange = [this](const std::string& str) { type = str; };

		m_input = new QInputDialog;
		m_input->setLabelText("Enter a name");

		layout->addItem(menu);
		layout->addWidget(m_input);

		setLayout(layout);

		show();
		isRejected = !m_input->exec();

		fieldName = m_input->textValue().toStdString();
	}

	void ScriptWidget::PopupMenu::closeEvent(QCloseEvent* event)
	{
		m_input->reject();
		isRejected = true;
		event->accept();
	}

	ScriptWidget::ScriptWidget(aw::sdk::Script* comp, QWidget* parent) : ComponentWidget(comp, parent)
	{
		ComponentLayout* layout = new ComponentLayout(comp, "Script", this);

		m_drop = new DropWidget;
		m_drop->AddExtension(".lua");

		QString objName = comp->GetPath().c_str();
		int index = objName.indexOf("assets/");

		if (index != -1)
			objName = objName.remove(0, index + 7); // 7 == "assets/" size

		m_drop->setText(objName);

		m_drop->onValueChange = [comp](const std::string& str) {
			comp->SetPath("assets/" + str);
		};

		layout->addRow("Script:", m_drop);

		comp->SetBindingsLayout(this);

		m_addBinding = new QPushButton;
		m_addBinding->setIcon(QIcon("assetsEditor/icons/plusButtonIcon.png"));
		connect(m_addBinding, &QPushButton::released, this, [this]() {
			PopupMenu menu;
			if(!menu.isRejected)
				CreateBinding(menu.type, menu.fieldName);
		});

		layout->addWidget(m_addBinding);

		setLayout(layout);
	}

	void ScriptWidget::Update()
	{
		aw::sdk::Script* comp = dynamic_cast<aw::sdk::Script*>(m_comp);

		ComponentLayout* layout = dynamic_cast<ComponentLayout*>(this->layout());
		layout->Update(comp);

		QString objName = comp->GetPath().c_str();
		int index = objName.indexOf("assets/");

		if (index != -1)
			objName = objName.remove(0, index + 7); // 7 == "assets/" size

		m_drop->setText(objName);

		// TODO Update Bindings for Editor
		//for (int i = 8; i < layout->rowCount() - 1; i++) // 8 == 1st Binding row. -1 because of '+' button.
		//{
		//	layout->itemAt(i, QFormLayout::ItemRole::FieldRole);
		//}
	}

	void ScriptWidget::AddBindingLayout(RefLayout* reflayout)
	{
		QPushButton* removeButton = new QPushButton;
		removeButton->setIcon(QIcon("assetsEditor/icons/minusButtonIcon.png"));

		connect(removeButton, &QPushButton::released, this, [this, reflayout]() {
			RemoveBinding(reflayout);
		});

		dynamic_cast<ComponentLayout*>(this->layout())->addRow(removeButton, reflayout);
	}

	void ScriptWidget::RemoveBinding(RefLayout* reflayout)
	{
		ComponentLayout* layout = dynamic_cast<ComponentLayout*>(this->layout());
		
		dynamic_cast<aw::sdk::Script*>(m_comp)->RemoveBinding(reflayout->GetFieldName());

		layout->removeRow(reflayout); //  Wtf... Please QT...
	}

	void ScriptWidget::CreateBinding(const std::string& type, const std::string& fieldName)
	{
		aw::sdk::Script* comp = dynamic_cast<aw::sdk::Script*>(m_comp);

		if (type == "Boolean")
			comp->AddOrSetBinding(fieldName, false);
		else if (type == "Integer")
			comp->AddOrSetBinding(fieldName, 0);
		else if(type == "Floating-Point")
			comp->AddOrSetBinding(fieldName, 0.0f);
		else if (type == "String")
			comp->AddOrSetBinding(fieldName, std::string());
		else if (type == "Component")
			comp->AddOrSetBinding(fieldName, (aw::sdk::Component*)nullptr);

		ComponentLayout* layout = dynamic_cast<ComponentLayout*>(this->layout());

		layout->takeRow(m_addBinding);

		comp->SetBindingLayout(this, fieldName);

		layout->addWidget(m_addBinding);
	}
}