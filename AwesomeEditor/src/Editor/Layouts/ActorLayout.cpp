#include <Editor/Core/EditorApplication.hpp>

namespace ed
{
	ActorLayout::ActorLayout(aw::sdk::Actor* actor, QWidget* parent) : QFormLayout(parent)
	{
		setSpacing(2);

		QLabel* title = new QLabel("Actor");
		title->setFont(QFont("Arial", 14, QFont::Bold));

		addRow(title);

		m_actName = new TextLayout("Name:", actor->name);
		m_actName->onValueChange = [actor](const std::string& name) { dynamic_cast<EditorApplication*>(QApplication::instance())->RenameActor(actor, name); };
		addRow(m_actName);

		m_actIsActive = new BoolLayout("Is Active:", actor->IsActive());
		m_actIsActive->onValueChange = [actor](bool b) { actor->SetActive(b); };
		addRow(m_actIsActive);
	}

	void ActorLayout::Update(aw::sdk::Actor* actor)
	{
		m_actName->Update(actor->name);
		m_actIsActive->Update(actor->IsActive());
	}
}