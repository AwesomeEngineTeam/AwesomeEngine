#include <Awesome/Maths/Vector2.hpp>
#include <Awesome/Maths/Vector3.hpp>

namespace aw
{
	Vector2::Vector2(const Vector3& other) :
		x{ other.x },
		y{ other.y }
	{
	}

	float& Vector2::operator[](const uint8 index)
	{
		if (index == 0)
			return x;
		else if (index == 1)
			return y;
		
		LOG_ERROR("float& Vector2[" + std::to_string(index) + "] Element out of Array");
			return x;
	}

	float Vector2::operator[](const uint8 index) const
	{
		if (index == 0)
			return x;
		else if (index == 1)
			return y;
			
		LOG_ERROR("float Vector2[" + std::to_string(index) + "] Element out of Array");
			return 0.0f;
	}
}