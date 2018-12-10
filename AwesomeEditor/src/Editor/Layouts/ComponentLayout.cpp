#include <Editor/Core/EditorApplication.hpp>

namespace ed
{
	ComponentLayout::ComponentLayout(aw::sdk::Component* comp, const QString& typeName, QWidget* parent) : ActorLayout(comp->GetActor(), parent)
	{
		setSpacing(2);

		QLabel* title = new QLabel(typeName);
		title->setFont(QFont("Arial", 14, QFont::Bold));

		addRow(title);

		m_compName = new TextLayout("Name:", comp->name);
		m_compName->onValueChange = [comp](const std::string& name) { dynamic_cast<EditorApplication*>(QApplication::instance())->RenameComponent(comp, name); };
		addRow(m_compName);

		m_compIsActive = new BoolLayout("Is Active:", comp->IsActive());
		m_compIsActive->onValueChange = [comp](bool b) { comp->SetActive(b); };
		addRow(m_compIsActive);

		m_trLayout = new UniformTransformLayout(comp->GetLocalTransform());
		m_trLayout->onValueChange = [comp](const aw::UniformTransform& tr) { comp->SetLocalTransform(tr); };
		addRow(m_trLayout);
	}

	void ComponentLayout::Update(aw::sdk::Component* comp)
	{
		ActorLayout::Update(comp->GetActor());

		m_compName->Update(comp->name);
		m_compIsActive->Update(comp->IsActive());
		m_trLayout->Update(comp->GetLocalTransform());
	}
}