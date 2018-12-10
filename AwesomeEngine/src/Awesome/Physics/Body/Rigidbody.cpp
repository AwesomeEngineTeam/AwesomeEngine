#include <Awesome/Physics/Body/Rigidbody.hpp>

namespace aw
{
	namespace ph
	{
		std::vector<Shape> Rigidbody::GetShapes() const
		{
			std::vector<Shape> shapesVector;

			uint32 size = m_handle->getNbShapes();
			physx::PxShape** shapes = new physx::PxShape*[size];

			m_handle->getShapes(shapes, size);

			for (uint32 i = 0; i < size; i++)
				shapesVector.push_back(shapes[i]);

			delete[] shapes;

			return shapesVector;
		}

		void Rigidbody::ClearShapes()
		{
			uint32 size = m_handle->getNbShapes();

			if (size == 0u)
				return;

			physx::PxShape** shapes = new physx::PxShape*[size];

			m_handle->getShapes(shapes, size);

			for (uint32 i = 0; i < size; i++)
				m_handle->detachShape(*shapes[i]);

			delete[] shapes;
		}

		void Rigidbody::AttachShape(Shape& shape)
		{
			ASSERT(!shape.IsValid(), "Physcis: Rigidbody:AttachShape shape not valid!");

			m_handle->attachShape(*(shape.Get()));
		}

		void Rigidbody::DetachShape(Shape& shape)
		{
			ASSERT(!shape.IsValid(), "Physcis: Rigidbody::AttachShape shape not valid!");

			m_handle->detachShape(*(shape.Get()));
		}

		Shape Rigidbody::CreateShape(const Geometry& geo, const Material& mat)
		{
			physx::PxShape* shape = m_handle->createShape(geo.Get(), *(mat.m_handle));
			ASSERT(!shape, "Physics: Rigidbody::CreateShape failed!");

			return Shape(shape);
		}

		UniformTransform Rigidbody::FetchGlobalTransform(Shape& shape)
		{
			UniformTransform tr;
			physx::PxTransform phTr = physx::PxShapeExt::getGlobalPose(*(shape.Get()), *m_handle);

			tr.SetPosition(reinterpret_cast<const Vector3&>(phTr.p));
			tr.SetRotation(Quaternion(phTr.q.w, phTr.q.x, phTr.q.y, phTr.q.z));
			// physX to Awesome reference
			tr.SetRotation(tr.GetRotation() * Quaternion(-M_PI2, Vector3(0.0f, 0.0f, 1.0f)));

			return tr;
		}
	}
}