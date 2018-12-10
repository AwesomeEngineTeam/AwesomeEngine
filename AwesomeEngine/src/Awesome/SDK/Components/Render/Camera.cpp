#include <Awesome/SDK/Components/Render/Camera.hpp>
#include <Awesome/Maths/Vector2.hpp>

namespace aw
{
	namespace sdk
	{
		Camera::Camera(Actor* actor, float fov, float width, float height, float nearPlane, float farPlane) :
			Component(actor, uint32(ComponentID::Camera), "Unnamed Camera"),
			m_fov{ fov },
			m_width{ width },
			m_height{ height },
			m_nearPlane{ nearPlane },
			m_farPlane{ farPlane }
		{
			CalculatePerspective();

			m_textOrtho = {
				2.0f / width, 0, 0, -1.0f,
				0, 2.0f / height, 0, -1.0f,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}

		Vector3 Camera::ScreenToSpace(const Vector2& screen)
		{
			float imageAspectRatio = m_width / m_height;
			float Px = (2.0f * ((screen.x + 0.5f) / m_width) - 1.0f) * tan(m_fov / 2.0f) * imageAspectRatio;
			float Py = (1.0f - 2.0f * ((screen.y + 0.5f) / m_height)) * tan(m_fov / 2.0f);

			return GlobalPosition() + GlobalRotation() * Vector3(Px, Py, -1);
		}
	}
}