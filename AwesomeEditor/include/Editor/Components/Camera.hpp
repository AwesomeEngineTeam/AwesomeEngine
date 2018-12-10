#pragma once

#ifndef AWESOME_EDITOR_CAMERA_GUARD
#define AWESOME_EDITOR_CAMERA_GUARD

#include <Awesome/SDK/Actor.hpp>
#include <Awesome/SDK/Camera.hpp>

namespace ed
{
	class Camera : public Actor
	{
	protected:
		aw::Camera* cameraComponent;

	public:
		inline explicit Camera(Scene* scene, float fov = M_PI4, float ratio = 1.33333f, float nearPlane = 0.1f, float farPlane = 1000.0f); // 1.3333f = 800 / 600
		inline ~Camera() override = default;

		inline float GetRatio() const;
		inline float GetFOV() const;
		inline float GetSkyboxFOV() const;
		inline float GetNear() const;
		inline float GetFar() const;

		inline void SetRatio(float ratio);
		inline void SetFOV(float fov);
		inline void SetSkyboxFOV(float fov);
		inline void SetNear(float nearPlane);
		inline void SetFar(float farPlane);

		inline void Translate(Vector3 translationVector);
		inline void Rotate(float xAxis, float yAxis);
	};
}

#include <Editor/Components/Camera.inl>

#endif // GUARD
