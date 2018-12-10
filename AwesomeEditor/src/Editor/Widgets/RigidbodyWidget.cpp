#include <QtWidgets/qcombobox.h>

#include <Editor/Widgets/RigidbodyWidget.hpp>

namespace ed
{
	RigidbodyWidget::RigidbodyWidget(aw::sdk::Rigidbody* comp, QWidget* parent) : ComponentWidget(comp, parent)
	{
		PhysicalComponentLayout* phLayout = new PhysicalComponentLayout(comp, "Rigidbody", this);

		m_shapeSelect = new DropDownMenu("Shape:");
		m_shapeSelect->AddItem("Box");
		m_shapeSelect->AddItem("Sphere");
		m_shapeSelect->AddItem("Capsule");

		phLayout->addRow(m_shapeSelect);

		if (comp->geometryType == aw::ph::GeometryType::Sphere || comp->geometryType == aw::ph::GeometryType::Capsule)
		{
			m_shapeSelect->SetCurrentItem("Sphere"); // Sphere or Capsule
			m_radLayout = new FloatLayout("Radius:", comp->radius, 0.001f);
			m_radLayout->onValueChange = [comp](float rad) { comp->radius = rad; };
			phLayout->addRow(m_radLayout);

			int i = 0;
			QFormLayout::ItemRole role;

			phLayout->getLayoutPosition(m_radLayout, &i, &role);
		}
		
		if (comp->geometryType == aw::ph::GeometryType::Capsule)
		{
			m_shapeSelect->SetCurrentItem("Capsule"); // Only Capsule
			m_hHeightLayout = new FloatLayout("Half Height:", comp->halfHeight, 0.001f);
			m_hHeightLayout->onValueChange = [comp](float hHeig) { comp->halfHeight = hHeig; };
			phLayout->addRow(m_hHeightLayout);
		}

		if (comp->geometryType == aw::ph::GeometryType::Box)
		{
			m_shapeSelect->SetCurrentItem("Box"); // Only Box
			m_hScaleLayout = new Vector3Layout("Half Scale:", comp->halfScale, 0.001f);
			m_hScaleLayout->onValueChange = [comp](const aw::Vector3& hSc) { comp->halfScale = hSc; };
			phLayout->addRow(m_hScaleLayout);
		}

		m_shapeSelect->onValueChange = [comp, phLayout, this](const std::string& name)
		{
			if (comp->geometryType == aw::ph::GeometryType::Sphere && name == "Capsule") // Sphere to Capsule
			{
				comp->SetGeometry(aw::ph::GeometryType::Capsule);
				m_radLayout->Update(comp->radius);

				m_hHeightLayout = new FloatLayout("Half Height:", comp->halfHeight, 0.001f);
				m_hHeightLayout->onValueChange = [comp](float hHeig) { comp->halfHeight = hHeig; };
				phLayout->insertRow(14, m_hHeightLayout);
				return;
			}
			else if(comp->geometryType == aw::ph::GeometryType::Capsule && name == "Sphere") // Capsule to Sphere
			{
				comp->SetGeometry(aw::ph::GeometryType::Sphere);
				m_radLayout->Update(comp->radius);

				phLayout->removeRow(m_hHeightLayout);
				m_hScaleLayout = nullptr;
				return;
			}

			if (m_radLayout)
			{
				phLayout->removeRow(m_radLayout);
				m_radLayout = nullptr;

				if (m_hHeightLayout)
				{
					phLayout->removeRow(m_hHeightLayout);
					m_hHeightLayout = nullptr;
				}
			}
			else if (m_hScaleLayout)
			{
				phLayout->removeRow(m_hScaleLayout);
				m_hScaleLayout = nullptr;
			}

			if (name == "Box")
			{
				comp->SetGeometry(aw::ph::GeometryType::Box);
				m_hScaleLayout = new Vector3Layout("Half Scale:", comp->halfScale, 0.001f);
				m_hScaleLayout->onValueChange = [comp](const aw::Vector3& hSc) { comp->halfScale = hSc; };
				phLayout->insertRow(13, m_hScaleLayout);
			}
			else
			{
				if (name == "Sphere")
				{
					comp->SetGeometry(aw::ph::GeometryType::Sphere);

					m_radLayout = new FloatLayout("Radius:", comp->radius, 0.001f);
					m_radLayout->onValueChange = [comp](float rad) { comp->radius = rad; };
					phLayout->insertRow(13, m_radLayout);
				}
				else
				{
					comp->SetGeometry(aw::ph::GeometryType::Capsule);

					m_radLayout = new FloatLayout("Radius:", comp->radius, 0.001f);
					m_radLayout->onValueChange = [comp](float rad) { comp->radius = rad; };
					phLayout->insertRow(13, m_radLayout);

					m_hHeightLayout = new FloatLayout("Half Height:", comp->halfHeight, 0.001f);
					m_hHeightLayout->onValueChange = [comp](float hHeig) { comp->halfHeight = hHeig; };
					phLayout->insertRow(14, m_hHeightLayout);
				}
			}
		};

		m_detectionSelect = new DropDownMenu("Collision Detection:");
		m_detectionSelect->AddItem("Discreet");
		m_detectionSelect->AddItem("Continuous");

		if (comp->collisionDetectionMode == aw::ph::CollisionDetection::Discreet)
			m_detectionSelect->SetCurrentItem("Discreet");

		m_detectionSelect->onValueChange = [comp, this](const std::string& name) {
			if (name == "Discreet")
				comp->collisionDetectionMode = aw::ph::CollisionDetection::Discreet;
			else if (name == "Continuous")
				comp->collisionDetectionMode = aw::ph::CollisionDetection::Continuous;
		};

		m_mass = new FloatLayout("Mass:", comp->mass);
		m_mass->onValueChange = [comp](float f) { comp->mass = f; };

		m_lockAxis = new UIntLayout("Lock Axis:", comp->lockAxisMask);
		m_lockAxis->onValueChange = [comp](aw::uint32 mask) { comp->lockAxisMask = mask; };

		m_gravKin[0] = new BoolLayout("Is Gravity:", comp->useGravity);
		m_gravKin[0]->onValueChange = [comp](bool b) { comp->useGravity = b; };

		m_gravKin[1] = new BoolLayout("Is Kinematic:", comp->IsKinematic());
		m_gravKin[1]->onValueChange = [comp](bool b) { comp->SetKinematic(b); };

		m_vels[0] = new Vector3Layout("Linear Velocity:", comp->linearVelocity);
		m_vels[0]->onValueChange = [comp](const aw::Vector3& f) { comp->linearVelocity = f; };

		m_vels[1] = new Vector3Layout("Angular Velocity:", comp->angularVelocity);
		m_vels[1]->onValueChange = [comp](const aw::Vector3& f) { comp->angularVelocity = f; };

		phLayout->addRow(m_detectionSelect);

		phLayout->addRow(m_mass);
		phLayout->addRow(m_lockAxis);
		phLayout->addRow(m_gravKin[0]);
		phLayout->addRow(m_gravKin[1]);
		phLayout->addRow(m_vels[0]);
		phLayout->addRow(m_vels[1]);

		setLayout(phLayout);
	}

	void RigidbodyWidget::Update()
	{
		aw::sdk::Rigidbody* comp = dynamic_cast<aw::sdk::Rigidbody*>(m_comp);

		dynamic_cast<PhysicalComponentLayout*>(layout())->Update(comp);

		int index = 0;

		if (comp->geometryType == aw::ph::GeometryType::Box)
		{
			m_shapeSelect->SetCurrentItem("Box");
			m_hScaleLayout->Update(comp->halfScale);
		}
		else
		{
			m_radLayout->Update(comp->radius);
			m_shapeSelect->SetCurrentItem("Sphere");

			if (comp->geometryType == aw::ph::GeometryType::Capsule)
			{
				m_hHeightLayout->Update(comp->halfHeight);
				m_shapeSelect->SetCurrentItem("Capsule");
			}
		}

		if (comp->collisionDetectionMode == aw::ph::CollisionDetection::Discreet)
			m_detectionSelect->SetCurrentItem("Discreet");
		else if (comp->collisionDetectionMode == aw::ph::CollisionDetection::Continuous)
			m_detectionSelect->SetCurrentItem("Continuous");

		m_mass->Update(comp->mass);
		m_lockAxis->Update(comp->lockAxisMask);
		m_gravKin[0]->Update(comp->useGravity);
		m_gravKin[1]->Update(comp->IsKinematic());

		m_vels[0]->Update(comp->linearVelocity);
		m_vels[1]->Update(comp->angularVelocity);
	}
}