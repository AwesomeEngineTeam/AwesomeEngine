namespace ed
{
	template <typename T>
	aw::sdk::MeshRenderer* EditorApplication::CreateGizmo(aw::sdk::Actor* actor, aw::sdk::Component* parent)
	{
		return nullptr;
	}

	inline Window& EditorApplication::GetWindow()
	{
		ASSERT(m_editWin == nullptr, "m_editWin is nullptr");

		return *m_editWin;
	}

	inline void EditorApplication::SetTranslationTool()
	{
		m_currentTool = LandmarkTool::Translation;
		m_currentToolInt = static_cast<int>(m_currentTool);
	}

	inline void EditorApplication::SetRotationTool()
	{
		m_currentTool = LandmarkTool::Rotation;
		m_currentToolInt = static_cast<int>(m_currentTool);
	}

	inline void EditorApplication::SetScaleTool()
	{
		m_currentTool = LandmarkTool::Scale;
		m_currentToolInt = static_cast<int>(m_currentTool);
	}

	inline void EditorApplication::SetToolAbsolute()
	{
		m_landmarkControlAbsolute = true;
	}

	inline void EditorApplication::SetToolRelative()
	{
		m_landmarkControlAbsolute = false;
	}

	/*template <>
	inline aw::sdk::MeshRenderer* EditorApplication::CreateGizmo<aw::sdk::PlaneCollider>(aw::sdk::Actor* actor, aw::sdk::Component* parent)
	{
		if (!actor)
			return nullptr;

		auto debugPlane = actor->AddComponent<aw::sdk::MeshRenderer>(parent);
		ASSERT(!debugPlane, "debugPlane is nullptr");
		debugPlane->SetMesh(static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::DebugQuad));
		debugPlane->SetMaterial(m_debugMaterialIndex);
		debugPlane->isImplicit = true;

		return debugPlane;
	}

	template <>
	inline aw::sdk::MeshRenderer* EditorApplication::CreateGizmo<aw::sdk::BoxCollider>(aw::sdk::Actor* actor, aw::sdk::Component* parent)
	{
		if (!actor)
			return nullptr;

		auto debugBox = actor->AddComponent<aw::sdk::MeshRenderer>(parent);
		ASSERT(!debugBox, "debugBox is nullptr");
		debugBox->SetMesh(static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::DebugCube));
		debugBox->SetMaterial(m_debugMaterialIndex);
		debugBox->isImplicit = true;

		return debugBox;
	}

	template <>
	inline aw::sdk::MeshRenderer* EditorApplication::CreateGizmo<aw::sdk::SphereCollider>(aw::sdk::Actor* actor, aw::sdk::Component* parent)
	{
		if (!actor)
			return nullptr;

		auto debugSphere = actor->AddComponent<aw::sdk::MeshRenderer>(parent);
		ASSERT(!debugSphere, "debugSphere is nullptr");
		debugSphere->SetMesh(static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::DebugSphere));
		debugSphere->SetMaterial(m_debugMaterialIndex);
		debugSphere->isImplicit = true;

		return debugSphere;
	}

	template <>
	inline aw::sdk::MeshRenderer* EditorApplication::CreateGizmo<aw::sdk::CapsuleCollider>(aw::sdk::Actor* actor, aw::sdk::Component* parent)
	{
		if (!actor)
			return nullptr;

		auto debugCapsule = actor->AddComponent<aw::sdk::MeshRenderer>(parent);
		ASSERT(!debugCapsule, "debugCapsule is nullptr");
		debugCapsule->SetMesh(static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::DebugCapsule));
		debugCapsule->SetMaterial(m_debugMaterialIndex);
		debugCapsule->isImplicit = true;

		return debugCapsule;
	}//*/

	/*template <>
	inline aw::sdk::PointLight* EditorApplication::InstantiateComponent<aw::sdk::PointLight>(aw::sdk::Actor* actor, aw::sdk::Component* parent)
	{
		if (actor)
		{
			aw::sdk::PointLight* comp = actor->AddComponent<aw::sdk::PointLight>(parent);

			if (comp != nullptr)
			{
				auto gizmo = actor->AddComponent<aw::sdk::Billboard>(comp);
				ASSERT(!gizmo, "gizmo is nullptr");
				gizmo->SetScale(0.5f);
				gizmo->SetLocalRotation(aw::Quaternion(3.14159f, aw::Vector3{ 1.0f, 0.0f, 0.0f }));
				gizmo->SetMesh(m_quadMeshIndex);
				gizmo->SetMaterial(m_lightGizmoMaterialIndex);
				gizmo->isImplicit = true;
			}

			return comp;
		}

		return nullptr;
	}*/

	template <>
	inline aw::sdk::Camera* EditorApplication::InstantiateComponent<aw::sdk::Camera>(aw::sdk::Actor* actor, aw::sdk::Component* parent)
	{
		if (actor)
		{
			aw::sdk::Camera* comp = actor->AddComponent<aw::sdk::Camera>(parent);
			aw::Vector2 dim = aw::InputManager::GetFocusedWindowDimensions();
			comp->SetWidth(dim.x);
			comp->SetHeight(dim.y);
			return comp;
		}

		return nullptr;
	}

	template <typename T>
	T* EditorApplication::InstantiateComponent(aw::sdk::Actor* actor, aw::sdk::Component* parent)
	{
		if (actor)
			return actor->AddComponent<T>(parent);

		return nullptr;
	}
}