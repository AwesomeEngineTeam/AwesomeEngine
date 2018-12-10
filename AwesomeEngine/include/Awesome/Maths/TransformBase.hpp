#pragma once

#ifndef AWESOME_MATHS_TRANSFORM_BASE_GUARD
#define AWESOME_MATHS_TRANSFORM_BASE_GUARD

#include <Awesome/Maths/Vector3.hpp>
#include <Awesome/Maths/Quaternion.hpp>

#include <mutex>

namespace aw
{
	struct Matrix4;

	struct TransformBase
	{
		struct Rotation : Quaternion
		{
			inline explicit Rotation(const Quaternion& quaternion);
			inline explicit Rotation(Quaternion&& quaternion);
			~Rotation() override = default;

		private:
			inline Quaternion FromQuaternion(const Quaternion& quaternion);
			using Quaternion::GetNormalized;
			inline Quaternion GetConjugated() const;

		public:
			inline Quaternion GetInversed() const;

		private:
			using Quaternion::Normalize;
			inline Quaternion& Conjugate();

		public:
			inline Quaternion& Inverse();

			inline Rotation& operator=(const Quaternion& quaternion);
			inline Rotation& operator=(Quaternion&& quaternion);

			REFLECT_BEGIN(Rotation)
				REFLECT_MEMBER_INHERITANCE(Quaternion)
			REFLECT_END()
		};

	protected:
		Vector3 position{ 0.0f, 0.0f, 0.0f };
		Rotation rotation{ { 1.0f, 0.0f, 0.0f, 0.0f } };

		mutable std::mutex m_posMutex;
		mutable std::mutex m_rotMutex;

	public:
		TransformBase() = default;
		inline explicit TransformBase(const Vector3& position, const Quaternion& rotation = Quaternion::Identity());
		inline TransformBase(const TransformBase& other) : position{ other.GetPosition() }, rotation{ other.GetRotation() }
		{
		};

		~TransformBase() = default;

		inline Rotation GetRotation() const
		{
			m_rotMutex.lock();
			Rotation rot = rotation;
			m_rotMutex.unlock();

			return rot;
		}

		inline void SetRotation(Quaternion rot)
		{
			m_rotMutex.lock();
			rotation = rot;
			m_rotMutex.unlock();
		}

		inline Vector3 GetPosition() const
		{
			m_posMutex.lock();
			Vector3 pos = position;
			m_posMutex.unlock();

			return pos;
		}

		inline void SetPosition(Vector3 pos)
		{
			m_posMutex.lock();
			position = pos;
			m_posMutex.unlock();
		}

		REFLECT_BEGIN(TransformBase)
			REFLECT_MEMBER(position)
			REFLECT_MEMBER(rotation)
		REFLECT_END()
	};
}

#include <Awesome/Maths/TransformBase.inl>
#include <Awesome/Maths/Rotation.inl>

#endif //GUARD