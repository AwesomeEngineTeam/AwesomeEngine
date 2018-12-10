#pragma once

#ifndef AWESOME_EDITOR_LAYOUTS_COMPONENT_LAYOUT_GUARD
#define AWESOME_EDITOR_LAYOUTS_COMPONENT_LAYOUT_GUARD

#include <Editor/Layouts/ActorLayout.hpp>
#include <Editor/Layouts/UniformTransformLayout.hpp>

namespace ed
{
	class ComponentLayout : public ActorLayout
	{
		TextLayout* m_compName = nullptr;
		BoolLayout* m_compIsActive = nullptr;
		UniformTransformLayout* m_trLayout = nullptr;

	public:
		ComponentLayout(aw::sdk::Component* comp, const QString& typeName, QWidget* parent = nullptr);

		void Update(aw::sdk::Component* comp);
	};
}

#endif // GUARD