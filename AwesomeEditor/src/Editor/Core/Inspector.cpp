#include <Editor/Core/Window.hpp>

#include <Awesome/SDK.hpp>

namespace ed
{
	Inspector::Inspector(Window& win) : QDockWidget("Inspector", &win)
	{
		setObjectName("Inspector");
	}

	void Inspector::Inspect(aw::sdk::Component* comp)
	{
		if (!comp)
			return;

		Clear();

		if (onInspectComponent)
			onInspectComponent(comp);

		ComponentWidget* compWidget = nullptr;

		if (comp->typeId == aw::TypeInfo<aw::sdk::Component>::id)
			compWidget = new ComponentWidget(comp, true);
		else if (comp->typeId == aw::TypeInfo<aw::sdk::Camera>::id)
			compWidget = new CameraWidget(dynamic_cast<aw::sdk::Camera*>(comp));
		else if (comp->typeId == aw::TypeInfo<aw::sdk::MeshRenderer>::id)
			compWidget = new MeshRendererWidget(dynamic_cast<aw::sdk::MeshRenderer*>(comp));
		else if (comp->typeId == aw::TypeInfo<aw::sdk::PointLight>::id)
			compWidget = new LightWidget(dynamic_cast<aw::sdk::PointLight*>(comp), "Point Light");
		else if (comp->typeId == aw::TypeInfo<aw::sdk::DirectionalLight>::id)
			compWidget = new LightWidget(dynamic_cast<aw::sdk::DirectionalLight*>(comp), "Directional Light");
		else if (comp->typeId == aw::TypeInfo<aw::sdk::Rigidbody>::id)
			compWidget = new RigidbodyWidget(dynamic_cast<aw::sdk::Rigidbody*>(comp));
		else if (comp->typeId == aw::TypeInfo<aw::sdk::BoxCollider>::id)
			compWidget = new BoxColliderWidget(dynamic_cast<aw::sdk::BoxCollider*>(comp));
		else if (comp->typeId == aw::TypeInfo<aw::sdk::SphereCollider>::id)
			compWidget = new SphereColliderWidget(dynamic_cast<aw::sdk::SphereCollider*>(comp));
		else if (comp->typeId == aw::TypeInfo<aw::sdk::CapsuleCollider>::id)
			compWidget = new CapsuleColliderWidget(dynamic_cast<aw::sdk::CapsuleCollider*>(comp));
		else if (comp->typeId == aw::TypeInfo<aw::sdk::PlaneCollider>::id)
			compWidget = new PlaneColliderWidget(dynamic_cast<aw::sdk::PlaneCollider*>(comp));
		else if (comp->typeId == aw::TypeInfo<aw::sdk::Script>::id)
			compWidget = new ScriptWidget(dynamic_cast<aw::sdk::Script*>(comp));
		else if (comp->typeId == aw::TypeInfo<aw::sdk::SoundEmitter>::id)
			compWidget = new SoundEmitterWidget(dynamic_cast<aw::sdk::SoundEmitter*>(comp));
		else if (comp->typeId == aw::TypeInfo<aw::sdk::SoundListener>::id)
			compWidget = new SoundListenerWidget(dynamic_cast<aw::sdk::SoundListener*>(comp));
		else if (comp->typeId == aw::TypeInfo<aw::sdk::Text2D>::id)
			compWidget = new Text2DWidget(dynamic_cast<aw::sdk::Text2D*>(comp));
		else
		{
			LOG_LOG("Inspector: Component not implemented yet! Component ID: %i", comp->typeId);
			return;
		}

		setWidget(compWidget);
		connect(&m_timer, &QTimer::timeout, compWidget, &ComponentWidget::Update);

		m_timer.start(sRefreshTime);

		raise();
	}

	MaterialWidget* Inspector::Inspect(ed::Material& material)
	{
		Clear();

		MaterialWidget* widget = new MaterialWidget(material);

		setWidget(widget);

		raise();

		return widget;
	}

	void Inspector::SetOnGame(bool b)
	{
		setDisabled(b);
	}

	void Inspector::Clear()
	{
		if (widget())
		{
			if (onClearInspector)
				onClearInspector();

			m_timer.stop();
			disconnect(&m_timer, &QTimer::timeout, dynamic_cast<ComponentWidget*>(widget()), &ComponentWidget::Update);
			delete widget();
		}

		setWidget(nullptr);
	}
}