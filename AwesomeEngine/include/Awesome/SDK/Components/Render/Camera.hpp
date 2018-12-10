#pragma once

#ifndef AWESOME_SDK_CAMERA_GUARD
#define AWESOME_SDK_CAMERA_GUARD

#include <Awesome/Maths/Matrix4.hpp>

#include <Awesome/SDK/Components/Component.hpp>
#include <Awesome/Renderer/Framegraph/RenderArea.hpp>

namespace aw
{
	namespace sdk
	{
		class Camera : public Component
		{
			friend class RenderSystem;

		protected:
			Matrix4 m_projection{};
			Matrix4 m_invProjection{};
			Matrix4 m_skyboxProjection{};
			Matrix4 m_textOrtho{};

			float m_fov;
			float m_width;
			float m_height;
			float m_nearPlane;
			float m_farPlane;
			float m_skyboxFov{ M_PI4 };

		public:
			RenderArea renderArea{};

			Camera() = delete;
			AWESOME_SDK_API explicit Camera(Actor* actor, float fov = M_PI4, float width = 800.0f, float height = 600.0f, float nearPlane = 0.1f, float farPlane = 10.0f);
			Camera(const Camera&) = delete;
			Camera(Camera&&) = delete;
			~Camera() override = default;

			inline float GetWidth() const;
			inline float GetHeight() const;
			inline float GetFOV() const;
			inline float GetSkyboxFOV() const;
			inline float GetNear() const;
			inline float GetFar() const;

			inline void SetWidth(float width);
			inline void SetHeight(float height);
			inline void SetFOV(float fov);
			inline void SetSkyboxFOV(float fov);
			inline void SetNear(float nearPlane);
			inline void SetFar(float farPlane);

			AWESOME_SDK_API Vector3 ScreenToSpace(const Vector2&);

			inline void Reset(float fov = M_PI4, float width = 800.0f, float height = 600.0f, float nearPlane = 0.1f, float farPlane = 10.0f);

			// inline void LookAt(const Vector3& target, const Vector3& up = Vector3::Up()) override;
			inline void Rotate_WorldSpace(const Quaternion& rotation) override;
			inline void Rotate_SelfSpace(const Quaternion& rotation) override;
			inline void Rotate_WorldSpace(const Vector3& eulerAngles) override;
			inline void Rotate_SelfSpace(const Vector3& eulerAngles) override;
			inline void Translate_SelfSpace(const Vector3& translationVector) override;

		private:
			inline void CalculatePerspective();

		public:
			inline Camera& operator=(const Camera&) = delete;
			inline Camera& operator=(Camera&&) = delete;

			REFLECT_BEGIN(Camera)
				REFLECT_MEMBER_INHERITANCE(Component)
				REFLECT_MEMBER(m_projection)
				REFLECT_MEMBER(m_invProjection)
				REFLECT_MEMBER(m_skyboxProjection)
				REFLECT_MEMBER(m_textOrtho)
				REFLECT_MEMBER(m_fov)
				REFLECT_MEMBER(m_width)
				REFLECT_MEMBER(m_height)
				REFLECT_MEMBER(m_nearPlane)
				REFLECT_MEMBER(m_farPlane)
				REFLECT_MEMBER(m_skyboxFov)
			REFLECT_END()
		};
	}

	template <>
	class TypeInfo<sdk::Camera>
	{
	public:
		static const uint32 id = uint32(sdk::ComponentID::Camera);
	};
}

#include <Awesome/SDK/Components/Render/Camera.inl>

#endif // GUARD
