#pragma once

#ifndef AWESOME_EDITOR_LAYOUTS_ACTOR_LAYOUT_GUARD
#define AWESOME_EDITOR_LAYOUTS_ACTOR_LAYOUT_GUARD

#include <Awesome/SDK/Scene/Actor.hpp>

#include <Editor/Layouts/BoolLayout.hpp>
#include <Editor/Layouts/TextLayout.hpp>

namespace ed
{
	class ActorLayout : public QFormLayout
	{
		TextLayout* m_actName = nullptr;
		BoolLayout* m_actIsActive = nullptr;

	protected:
		void Update(aw::sdk::Actor* actor);

	public:
		ActorLayout(aw::sdk::Actor* actor, QWidget* parent = nullptr);
	};
}

#endif // GUARD