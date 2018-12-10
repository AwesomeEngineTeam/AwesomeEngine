#pragma once

#ifndef AWESOME_EDITOR_CORE_EDITOR_APPLICATION_GUARD
#define AWESOME_EDITOR_CORE_EDITOR_APPLICATION_GUARD

#include <QtWidgets/qapplication.h>
#include <QtWidgets/qsplashscreen.h>

#include <Awesome/SDK.hpp>

#include <Editor/Core/Window.hpp>

namespace ed
{
	class EditorApplication : public aw::sdk::BaseApplication, public QApplication
	{
		enum class LandmarkTool : aw::uint8
		{
			Translation = 0,
			Rotation,
			Scale
		};

		LandmarkTool m_currentTool{ LandmarkTool::Translation };
		std::atomic_int m_currentToolInt = 0;
		std::atomic_bool m_landmarkControlAbsolute = true;

		QSplashScreen m_splash;

		Window* m_editWin = nullptr;
		std::atomic_bool m_isPlaying = false;
		std::unique_ptr<aw::sdk::Scene>& m_scene;

		aw::RenderingResources m_renderingResources{};

		aw::sdk::Camera* m_cameraComponent = nullptr;
		aw::sdk::Camera m_editorCamera{ nullptr };

		aw::sdk::Component* m_selectedComponent{ nullptr };

		float m_editorCameraRotationSpeed{ 2.0f };
		float m_editorCameraTranslationSpeed{ 10.0f };

		float m_translationSnap{ 0.5f };
		float m_rotationSnap{ M_PI / 8.0f };
		float m_scaleSnap{ 0.5f };

	public:
		aw::sdk::Component* copiedComponent = nullptr;

		EditorApplication(int argc, char **argv);
		~EditorApplication() = default;

		void Play();
		void Stop();
		void QuitApp();

		template <typename T>
		T* InstantiateComponent(aw::sdk::Actor* actor, aw::sdk::Component* parent = nullptr);
		aw::sdk::Actor* InstantiateActor(const std::string& name = "Default", aw::sdk::Actor* parent = nullptr);
		aw::sdk::Actor* InstantiateActor(aw::sdk::Actor* parent = nullptr);
		aw::sdk::Actor* InstantiateQuad(aw::sdk::Actor* parent = nullptr);
		aw::sdk::Actor* InstantiateCube(aw::sdk::Actor* parent = nullptr);
		aw::sdk::Actor* InstantiateSphere(aw::sdk::Actor* parent = nullptr);
		aw::sdk::Actor* InstantiateCapsule(aw::sdk::Actor* parent = nullptr);
		aw::sdk::Actor* InstantiateCylinder(aw::sdk::Actor* parent = nullptr);
		aw::sdk::Actor* InstantiateCone(aw::sdk::Actor* parent = nullptr);

		template <typename T>
		aw::sdk::MeshRenderer* CreateGizmo(aw::sdk::Actor* actor, aw::sdk::Component* parent);

		inline Window& GetWindow();

		inline void SetTranslationTool();
		inline void SetRotationTool();
		inline void SetScaleTool();

		inline void SetToolAbsolute();
		inline void SetToolRelative();

		void NewScene();
		void SaveScene();

		void RenameActor(aw::sdk::Actor* actor, const std::string& name);
		void RenameComponent(aw::sdk::Component* comp, const std::string& name);

		void Destroy(aw::sdk::Actor* actor);
		void Destroy(aw::sdk::Actor* actor, aw::sdk::Component* comp);

		aw::sdk::Actor* Import(const std::string& path, aw::sdk::Actor* parent = nullptr);

		void Start() override;
		void Update() override;
		void End() override;

	private:
		void UpdateCamera();
		void UpdateCameraMousePicking();
		void UpdateCameraPosition();
	};
}

#include <Editor/Core/EditorApplication.inl>

#endif // GUARD