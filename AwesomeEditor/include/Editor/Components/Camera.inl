namespace ed
{
	inline Camera::Camera(Scene* scene, float fov, float ratio, float nearPlane, float farPlane) :
		Actor(scene, "Editor Camera")
	{
		cameraComponent = this->AddComponent<aw::Camera>();
		ASSERT(!m_root, "Editor Camera couldn't create Camera Component");
	}

	inline float Camera::GetRatio() const
	{
		cameraComponent->GetRatio();
	}

	inline float Camera::GetFOV() const
	{
		cameraComponent->GetFOV();
	}

	inline float Camera::GetSkyboxFOV() const
	{
		cameraComponent->GetSkyboxFOV();
	}

	inline float Camera::GetNear() const
	{
		cameraComponent->GetNear();
	}

	inline float Camera::GetFar() const
	{
		cameraComponent->GetFar();
	}

	inline void Camera::SetRatio(float ratio)
	{
		cameraComponent->SetRatio(ratio);
	}

	inline void Camera::SetFOV(float fov)
	{
		cameraComponent->SetFOV(fov);
	}

	inline void Camera::SetSkyboxFOV(float fov)
	{
		cameraComponent->SetSkyboxFOV(fov);
	}

	inline void Camera::SetNear(float nearPlane)
	{
		cameraComponent->SetNear(nearPlane);
	}

	inline void Camera::SetFar(float farPlane)
	{
		cameraComponent->SetFar(farPlane);
	}

	inline void Camera::Translate(Vector3 translationVector)
	{
		Translate_SelfSpace(translationVector);
	}

	inline void Camera::Rotate(float xAxis, float yAxis)
	{
		Rotate_SelfSpace({ xAxis, 0.0f, 0.0f });
		Rotate_WorldSpace({ 0.0f, yAxis, 0.0f });
	}
}