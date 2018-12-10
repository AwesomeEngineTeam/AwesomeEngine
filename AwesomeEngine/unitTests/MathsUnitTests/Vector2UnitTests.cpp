#include "CppUnitTest.h"

#include <Awesome/Maths/Vector2.hpp>
#include <Awesome/Maths/Vector3.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace aw;

namespace MathUnitTest
{
	TEST_CLASS(Vector2_Test)
	{
	public:
		TEST_METHOD(DefaultConstructor)
		{
			Vector2 v1;

			Assert::AreEqual(0.0f, v1.x, FLT_EPSILON);
			Assert::AreEqual(0.0f, v1.y, FLT_EPSILON);
		}

		TEST_METHOD(ScaleConstructor)
		{
			Vector2 v1(5.6f);

			Assert::AreEqual(5.6f, v1.x, FLT_EPSILON);
			Assert::AreEqual(5.6f, v1.y, FLT_EPSILON);
		}

		TEST_METHOD(ValueConstructor)
		{
			Vector2 v1(1.0f, 2.0f);

			Assert::AreEqual(1.0f, v1.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, v1.y, FLT_EPSILON);
		}

		TEST_METHOD(CopyConstructor)
		{
			Vector2 v1(1.0f, 2.0f);
			Vector2 v2(v1);

			Assert::AreEqual(1.0f, v2.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, v2.y, FLT_EPSILON);
		}

		TEST_METHOD(CopyVec3Constructor)
		{
			Vector3 v1(1.0f, 2.0f, 3.0f);
			Vector2 v2(v1);

			Assert::AreEqual(1.0f, v2.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, v2.y, FLT_EPSILON);
		}

		TEST_METHOD(RValueCopyConstructor)
		{
			Vector2 v1(1.0f, 2.0f);
			Vector2 v2 (std::move(v1));

			Assert::AreEqual(1.0f, v2.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, v2.y, FLT_EPSILON);
		}

		TEST_METHOD(GetNorm)
		{
			Vector2 v1(4.0f, 2.0f);

			Assert::AreEqual(4.4721359f, v1.Norm(), FLT_EPSILON);
		}

		TEST_METHOD(GetNormSq)
		{
			Vector2 v1(1.0f, 2.0f);

			Assert::AreEqual(5.0f, v1.SquaredNorm(), FLT_EPSILON);
		}

		TEST_METHOD(GetNormalized)
		{
			Vector2 v1(2.0f, 4.0f);
			Vector2 v2 = v1.GetNormalized();

			Assert::AreEqual(2.0f, v1.x, FLT_EPSILON);
			Assert::AreEqual(4.0f, v1.y, FLT_EPSILON);

			Assert::AreEqual(0.4472135f, v2.x, FLT_EPSILON);
			Assert::AreEqual(0.8944271f, v2.y, FLT_EPSILON);
		}

		TEST_METHOD(GetClamped)
		{
			Vector2 v1(1.0f, 4.0f);
			Vector2 v2(5.0f, 6.0f);
			Vector2 v3(3.0f, 9.0f);

			Vector2 v4 = v3.GetClamped(v1, v2);

			Assert::AreEqual(3.0f, v3.x, FLT_EPSILON);
			Assert::AreEqual(9.0f, v3.y, FLT_EPSILON);

			Assert::AreEqual(3.0f, v4.x, FLT_EPSILON);
			Assert::AreEqual(6.0f, v4.y, FLT_EPSILON);
		}

		TEST_METHOD(Angle)
		{
			Vector2 v1(1.0f, 0.0f);
			Vector2 v2(-sqrtf(3.0f) / 2.0f, 1.0f / 2.0f);

			Assert::AreEqual(2.6179938f, Vector2::AngleBetween(v1, v2), FLT_EPSILON);

			v1.x = sqrtf(3.0f) / 2.0f;
			v1.y = -0.5f;
			v2.y = -v2.y;

			Assert::AreEqual(-2.0943951f, Vector2::AngleBetween(v1, v2), FLT_EPSILON);

			v2 = v1;

			Assert::AreEqual(0.0f, Vector2::AngleBetween(v1, v2), FLT_EPSILON);
		}

		TEST_METHOD(Normalize)
		{
			Vector2 v1(2.0f, 4.0f);
			v1.Normalize();

			Assert::AreEqual(0.44721359f, v1.x);
			Assert::AreEqual(0.89442719f, v1.y);
		}

		TEST_METHOD(Dot)
		{
			Vector2 v1(2.0f, 4.0f);
			Vector2 v2(5.0f, 3.0f);

			Assert::AreEqual(22.0f, v1.Dot(v2), FLT_EPSILON);

			v2.x = 4.0f;
			v2.y = -2.0f;

			Assert::AreEqual(0.0f, v1.Dot(v2), FLT_EPSILON);
		}

		TEST_METHOD(Clamp)
		{
			Vector2 v1(1.0f, 4.0f);
			Vector2 v2(5.0f, 6.0f);
			Vector2 v3(3.0f, 9.0f);

			v3.Clamp(v1, v2);

			Assert::AreEqual(3.0f, v3.x, FLT_EPSILON);
			Assert::AreEqual(6.0f, v3.y, FLT_EPSILON);
		}

		TEST_METHOD(Lerp)
		{
			Vector2 start;
			Vector2 end(1.0f, 2.0f);

			Vector2 res = Vector2::Lerp(start, end, 0.5f);

			Assert::AreEqual(0.5f, res.x, FLT_EPSILON);
			Assert::AreEqual(1.0f, res.y, FLT_EPSILON);

			res = Vector2::Lerp(start, end, 0.0f);

			Assert::AreEqual(start.x, res.x, FLT_EPSILON);
			Assert::AreEqual(start.y, res.y, FLT_EPSILON);

			res = Vector2::Lerp(start, end, 1.0f);

			Assert::AreEqual(end.x, res.x, FLT_EPSILON);
			Assert::AreEqual(end.y, res.y, FLT_EPSILON);
		}

		TEST_METHOD(NLerp)
		{
			Vector2 start;
			Vector2 end(1.0f, 2.0f);

			Vector2 res = Vector2::Nlerp(start, end, 0.5f);

			Assert::AreEqual(0.4472136f, res.x, FLT_EPSILON);
			Assert::AreEqual(0.8944272f, res.y, FLT_EPSILON);

			start = Vector2(2.0f, 4.0f);
			res = Vector2::Nlerp(start, end, 0.0f);

			Assert::AreEqual(0.4472135f, res.x, FLT_EPSILON);
			Assert::AreEqual(0.8944271f, res.y, FLT_EPSILON);

			res = Vector2::Nlerp(start, end, 1.0f);

			Assert::AreEqual(0.4472135f, res.x, FLT_EPSILON);
			Assert::AreEqual(0.8944271f, res.y, FLT_EPSILON);
		}

		TEST_METHOD(SLerp)
		{
			Vector2 start(3.0f, 4.0f);
			Vector2 end(1.0f, 2.0f);

			Vector2 res = Vector2::Slerp(start, end, 0.5f);

			Assert::AreEqual(1.902113f, res.x, FLT_EPSILON);
			Assert::AreEqual(3.0776835f, res.y, FLT_EPSILON);

			res = Vector2::Slerp(start, end, 0.0f);

			Assert::AreEqual(3.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(4.0f, res.y, FLT_EPSILON);

			res = Vector2::Slerp(start, end, 1.0f);

			Assert::AreEqual(1.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.y, FLT_EPSILON);
		}

		TEST_METHOD(OpPlus)
		{
			Vector2 v1(1.0f, 2.0f);

			Vector2 res = +v1;

			Assert::AreEqual(1.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.y, FLT_EPSILON);
		}

		TEST_METHOD(OpMinus)
		{
			Vector2 v1(1.0f, 2.0f);

			Vector2 res = -v1;

			Assert::AreEqual(-1.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(-2.0f, res.y, FLT_EPSILON);
		}

		TEST_METHOD(Vec2MultFloat)
		{
			Vector2 v1(3.0f, 4.0f);

			Vector2 res = v1 * 2.0f;

			Assert::AreEqual(6.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(8.0f, res.y, FLT_EPSILON);
		}

		TEST_METHOD(Vec2DivFloat)
		{
			Vector2 v1(3.0f, 4.0f);

			Vector2 res = v1 / 2.0f;

			Assert::AreEqual(1.5f, res.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.y, FLT_EPSILON);
		}

		TEST_METHOD(Vec2PlusVec2)
		{
			Vector2 v1(3.0f, 4.0f);
			Vector2 v2(6.0f, 8.0f);

			Vector2 res = v1 + v2;

			Assert::AreEqual(9.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(12.0f, res.y, FLT_EPSILON);
		}

		TEST_METHOD(Vec2MinusVec2)
		{
			Vector2 v1(3.0f, 4.0f);
			Vector2 v2(6.0f, 8.0f);

			Vector2 res = v1 - v2;

			Assert::AreEqual(-3.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(-4.0f, res.y, FLT_EPSILON);
		}

		TEST_METHOD(OpEquals)
		{
			Vector2 v1(4.0f, 5.0f);

			Vector2 res;
			res = v1;

			Assert::AreEqual(v1.x, res.x, FLT_EPSILON);
			Assert::AreEqual(v1.y, res.y, FLT_EPSILON);
		}

		TEST_METHOD(OpEqualsRValue)
		{
			Vector2 v1(4.0f, 5.0f);

			Vector2 res;
			res = std::move(v1);

			Assert::AreEqual(4.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(5.0f, res.y, FLT_EPSILON);
		}

		TEST_METHOD(OpMultEqualsFloat)
		{
			Vector2 v1(3.0f, 4.0f);

			v1 *= 3.0f;

			Assert::AreEqual(9.0f, v1.x, FLT_EPSILON);
			Assert::AreEqual(12.0f, v1.y, FLT_EPSILON);
		}

		TEST_METHOD(OpDivEqualsFloat)
		{
			Vector2 v1(3.0f, 4.0f);

			v1 /= 2.0f;

			Assert::AreEqual(1.5f, v1.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, v1.y, FLT_EPSILON);
		}

		TEST_METHOD(OpPlusEqualsVec2)
		{
			Vector2 v1(3.0f, 4.0f);

			v1 += {3.0f, 4.0f};

			Assert::AreEqual(6.0f, v1.x, FLT_EPSILON);
			Assert::AreEqual(8.0f, v1.y, FLT_EPSILON);
		}

		TEST_METHOD(OpMinusEqualsVec2)
		{
			Vector2 v1(3.0f, 4.0f);

			v1 -= {3.0f, 4.0f};

			Assert::AreEqual(0.0f, v1.x, FLT_EPSILON);
			Assert::AreEqual(0.0f, v1.y, FLT_EPSILON);
		}

		TEST_METHOD(OpComparator)
		{
			Vector2 v1(1.0f, 2.0f);
			Vector2 v2;

			Assert::AreEqual(false, v1 == v2);

			v2 = v1;
			Assert::AreEqual(true, v1 == v2);
		}

		TEST_METHOD(OpComparatorDiff)
		{
			Vector2 v1(1.0f, 2.0f);
			Vector2 v2;

			Assert::AreEqual(true, v1 != v2);

			v2 = v1;
			Assert::AreEqual(false, v1 != v2);
		}

		TEST_METHOD(OpAccess)
		{
			Vector2 v1(2.0f, 4.0f);
			v1[0] = 5.0f;
			v1[1] = 8.0f;

			Assert::AreEqual(5.0f, v1.x, FLT_EPSILON);
			Assert::AreEqual(8.0f, v1.y, FLT_EPSILON);
		}

		TEST_METHOD(OpConstAccess)
		{
			const Vector2 v1(2.0f, 4.0f);

			Assert::AreEqual(2.0f, v1[0], FLT_EPSILON);
			Assert::AreEqual(4.0f, v1[1], FLT_EPSILON);
		}

		TEST_METHOD(OpFloatMultVec2)
		{
			Vector2 v1(2.0f, 4.0f);

			Vector2 res = 3.0f * v1;

			Assert::AreEqual(6.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(12.0f, res.y, FLT_EPSILON);
		}
	};
}