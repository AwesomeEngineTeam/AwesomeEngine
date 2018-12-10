#pragma once

#ifndef AWESOME_EDITOR_CORE_INSPECTOR_GUARD
#define AWESOME_EDITOR_CORE_INSPECTOR_GUARD

#include <QtWidgets/qdockwidget.h>
#include <QtCore/qtimer.h>

#include <Awesome/SDK/Components/Component.hpp>

#include <Editor/Core/Material.hpp>

#include <Editor/Widgets/CameraWidget.hpp>
#include <Editor/Widgets/LightWidget.hpp>
#include <Editor/Widgets/MeshRendererWidget.hpp>
#include <Editor/Widgets/MaterialWidget.hpp>
#include <Editor/Widgets/BoxColliderWidget.hpp>
#include <Editor/Widgets/SphereColliderWidget.hpp>
#include <Editor/Widgets/CapsuleColliderWidget.hpp>
#include <Editor/Widgets/PlaneColliderWidget.hpp>
#include <Editor/Widgets/RigidbodyWidget.hpp>
#include <Editor/Widgets/SoundEmitterWidget.hpp>
#include <Editor/Widgets/SoundListenerWidget.hpp>
#include <Editor/Widgets/Text2DWidget.hpp>
#include <Editor/Widgets/ScriptWidget.hpp>

namespace ed
{
	class Window;

	class Inspector : public QDockWidget
	{
		QTimer m_timer;

		static constexpr int sRefreshTime = 100; // ms

	public:
		std::function<void(aw::sdk::Component*)> onInspectComponent;
		std::function<void()> onClearInspector;

		Inspector(Window& win);
		~Inspector() = default;

		void Inspect(aw::sdk::Component* comp);
		MaterialWidget* Inspect(ed::Material& material);

		void SetOnGame(bool b = true);

		void Clear();
	};
}

#endif // GUARD