namespace aw
{
	namespace sdk
	{
		inline float Camera::GetWidth() const
		{
			return m_width;
		}

		inline float Camera::GetHeight() const
		{
			return m_height;
		}

		inline float Camera::GetFOV() const
		{
			return m_fov;
		}

		inline float Camera::GetSkyboxFOV() const
		{
			return m_skyboxFov;
		}

		inline float Camera::GetNear() const
		{
			return m_nearPlane;
		}

		inline float Camera::GetFar() const
		{
			return m_farPlane;
		}

		inline void Camera::SetWidth(float width)
		{
			m_width = width;

			float ratio = m_width / m_height;

			const float m00 = 1.0f / tanf(m_fov * 0.5f) / ratio;

			m_projection.m00 = m00;
			m_skyboxProjection.m00 = m00;
			m_textOrtho.m00 = 2.0f / m_width;
		}

		inline void Camera::SetHeight(float height)
		{
			m_height = height;

			float ratio = m_width / m_height;

			const float m00 = 1.0f / tanf(m_fov * 0.5f) / ratio;

			m_projection.m00 = m00;
			m_skyboxProjection.m00 = m00;
			m_textOrtho.m11 = 2.0f / m_height;
		}

		inline void Camera::SetFOV(float fov)
		{
			m_fov = fov;

			const float invTanHalfFovy = 1.0f / tanf(m_fov * 0.5f);

			m_projection.m00 = invTanHalfFovy / (m_width / m_height);
			m_projection.m11 = -invTanHalfFovy;
		}

		inline void Camera::SetSkyboxFOV(float fov)
		{
			m_skyboxFov = fov;

			const float invTanHalfFovy = 1.0f / tanf(m_skyboxFov * 0.5f);

			m_skyboxProjection.m00 = invTanHalfFovy / (m_width / m_height);
			m_skyboxProjection.m11 = -invTanHalfFovy;
		}

		inline void Camera::SetNear(float nearPlane)
		{
			m_nearPlane = nearPlane;

			const float k = m_farPlane / (2.0f * (m_nearPlane - m_farPlane));
			const float k2 = k * (1.0f + 2.0f * m_nearPlane);

			m_projection.m22 = k;
			m_projection.m23 = k2;

			m_skyboxProjection.m22 = k;
			m_skyboxProjection.m23 = k2;
		}

		inline void Camera::SetFar(float farPlane)
		{
			m_farPlane = farPlane;

			const float k = m_farPlane / (2.0f * (m_nearPlane - m_farPlane));
			const float k2 = k * (1.0f + 2.0f * m_nearPlane);

			m_projection.m22 = k;
			m_projection.m23 = k2;

			m_skyboxProjection.m22 = k;
			m_skyboxProjection.m23 = k2;
		}

		inline void Camera::Reset(float fov, float width, float height, float nearPlane, float farPlane)
		{
			m_fov = fov;
			m_width = width;
			m_height = height;
			m_nearPlane = nearPlane;
			m_farPlane = farPlane;

			CalculatePerspective();
		}

		// TODO : Do
		// inline void Camera::LookAt(const Vector3& target, const Vector3& up)
		// {

		// 	m_trMutex.lock();
		// 	CalculateGlobalTransform();
		// 	SetChildHierarchyDirty();
		// 	m_trMutex.unlock();
		// }

		inline void Camera::Rotate_WorldSpace(const Quaternion& rotation)
		{
			Component::Rotate_WorldSpace(Quaternion(-rotation.w, rotation.x, rotation.y, rotation.z));
		}

		inline void Camera::Rotate_SelfSpace(const Quaternion& rotation)
		{
			Component::Rotate_SelfSpace(Quaternion(-rotation.w, rotation.x, rotation.y, rotation.z));
		}

		inline void Camera::Rotate_WorldSpace(const Vector3& eulerAngles)
		{
			Component::Rotate_WorldSpace(-eulerAngles);
		}

		inline void Camera::Rotate_SelfSpace(const Vector3& eulerAngles)
		{
			Component::Rotate_SelfSpace(-eulerAngles);
		}

		inline void Camera::Translate_SelfSpace(const Vector3& translationVector)
		{
			Quaternion globalRotation = GlobalRotation();

			m_trMutex.lock();

			m_localTransform.SetPosition(m_localTransform.GetPosition() + globalRotation * Vector3(translationVector.x, translationVector.y, -translationVector.z));
			CalculateGlobalTransform();
			SetChildHierarchyDirty();

			m_trMutex.unlock();
		}

		inline void Camera::CalculatePerspective()
		{
			float ratio = (m_width / m_height);
			m_projection = Matrix4::MakePerspective(m_fov, ratio, m_nearPlane, m_farPlane);
			m_invProjection = m_projection.GetInversed();

			const float skyboxInvTanHalfFovy = 1.0f / tanf(m_skyboxFov * 0.5f);

			m_skyboxProjection = {
				skyboxInvTanHalfFovy / ratio,	0.0f,					0.0f,				0.0f,
				0.0f,							-skyboxInvTanHalfFovy,	0.0f,				0.0f,
				0.0f,							0.0f,					m_projection.m22,	m_projection.m23,
				0.0f,							0.0f,					-1.0f,				0.0f
			};
		}
	}
}