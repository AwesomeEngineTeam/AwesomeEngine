#include "CppUnitTest.h"

#include <Awesome/Maths/Vector2.hpp>
#include <Awesome/Maths/Vector3.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace aw;

namespace MathUnitTest
{
	TEST_CLASS(Vector3_Test)
	{
	public:
		TEST_METHOD(DefaultConstructor)
		{
			Vector3 v1;

			Assert::AreEqual(0.0f, v1.x, FLT_EPSILON);
			Assert::AreEqual(0.0f, v1.y, FLT_EPSILON);
			Assert::AreEqual(0.0f, v1.z, FLT_EPSILON);
		}

		TEST_METHOD(ScaleConstructor)
		{
			Vector3 v1(5.6f);

			Assert::AreEqual(5.6f, v1.x, FLT_EPSILON);
			Assert::AreEqual(5.6f, v1.y, FLT_EPSILON);
			Assert::AreEqual(5.6f, v1.z, FLT_EPSILON);
		}

		TEST_METHOD(ValueConstructor)
		{
			Vector3 v1(1.0f, 2.0f, 3.0f);

			Assert::AreEqual(1.0f, v1.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, v1.y, FLT_EPSILON);
			Assert::AreEqual(3.0f, v1.z, FLT_EPSILON);
		}

		TEST_METHOD(CopyConstructor)
		{
			Vector3 v1(1.0f, 2.0f, 3.0f);
			Vector3 v2(v1);

			Assert::AreEqual(1.0f, v2.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, v2.y, FLT_EPSILON);
			Assert::AreEqual(3.0f, v2.z, FLT_EPSILON);
		}

		TEST_METHOD(CopyVec2Constructor)
		{
			Vector2 v1(1.0f, 2.0f);
			Vector3 v2(v1, 3.0f);

			Assert::AreEqual(1.0f, v2.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, v2.y, FLT_EPSILON);
			Assert::AreEqual(3.0f, v2.z, FLT_EPSILON);
		}

		TEST_METHOD(RValueCopyConstructor)
		{
			Vector3 v1(1.0f, 2.0f, 3.0f);
			Vector3 v2(std::move(v1));

			Assert::AreEqual(1.0f, v2.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, v2.y, FLT_EPSILON);
			Assert::AreEqual(3.0f, v2.z, FLT_EPSILON);
		}

		TEST_METHOD(GetNorm)
		{
			Vector3 v1(4.0f, 2.0f, 6.0f);

			Assert::AreEqual(7.4833148f, v1.Norm(), FLT_EPSILON);
		}

		TEST_METHOD(GetNormSq)
		{
			Vector3 v1(4.0f, 2.0f, 6.0f);

			Assert::AreEqual(56.0f, v1.SquaredNorm(), FLT_EPSILON);
		}

		TEST_METHOD(GetNormalized)
		{
			Vector3 v1(4.0f, 2.0f, 6.0f);
			Vector3 v2 = v1.GetNormalized();

			Assert::AreEqual(4.0f, v1.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, v1.y, FLT_EPSILON);
			Assert::AreEqual(6.0f, v1.z, FLT_EPSILON);

			Assert::AreEqual(0.5345224f, v2.x, FLT_EPSILON);
			Assert::AreEqual(0.2672612f, v2.y, FLT_EPSILON);
			Assert::AreEqual(0.8017837f, v2.z, FLT_EPSILON);
		}

		TEST_METHOD(GetClamped)
		{
			Vector3 v1(1.0f, 4.0f, -5.0f);
			Vector3 v2(5.0f, 6.0f, 7.0f);
			Vector3 v3(3.0f, 9.0f, -8.0f);

			Vector3 v4 = v3.GetClamped(v1, v2);

			Assert::AreEqual(3.0f, v3.x, FLT_EPSILON);
			Assert::AreEqual(9.0f, v3.y, FLT_EPSILON);
			Assert::AreEqual(-8.0f, v3.z, FLT_EPSILON);

			Assert::AreEqual(3.0f, v4.x, FLT_EPSILON);
			Assert::AreEqual(6.0f, v4.y, FLT_EPSILON);
			Assert::AreEqual(-5.0f, v4.z, FLT_EPSILON);
		}

		TEST_METHOD(GetAngle)
		{
			Vector3 v1(1.0f, 2.0f, 3.0f);
			Vector3 v2(2.0f, 1.0f, 3.0f);

			Assert::AreEqual(0.3802514f, Vector3::AngleBetween(v1, v2), FLT_EPSILON);

			v2 = v1;

			Assert::AreEqual(0.0f, Vector3::AngleBetween(v1, v2), FLT_EPSILON);
		}

		TEST_METHOD(GetAngleSigned)
		{
			Vector3 v1(1.0f, 2.0f, 3.0f);
			Vector3 v2(2.0f, 1.0f, 3.0f);
			Vector3 v3(3.0f, 2.0f, 1.0f);

			Assert::AreEqual(0.38025119f, Vector3::AngleSignedBetween(v1, v2, v3), FLT_EPSILON);

			v3 = -v3;

			Assert::AreEqual(-0.38025119f, Vector3::AngleSignedBetween(v1, v2, v3), FLT_EPSILON);

			v2 = v1;

			Assert::AreEqual(0.0f, Vector3::AngleSignedBetween(v1, v2, v3), FLT_EPSILON);
		}

		TEST_METHOD(Normalize)
		{
			Vector3 v1(1.1111111f, 2.2222222f, 3.3333333f);
			v1.Normalize();

			Assert::AreEqual(0.2672612369f, v1.x, FLT_EPSILON);
			Assert::AreEqual(0.5345224738f, v1.y, FLT_EPSILON);
			Assert::AreEqual(0.8017837405f, v1.z, FLT_EPSILON);
		}

		TEST_METHOD(Dot)
		{
			Vector3 v1(2.0f, 4.0f, 3.0f);
			Vector3 v2(5.0f, 3.0f, 4.0f);

			Assert::AreEqual(34.0f, v1.Dot(v2), FLT_EPSILON);

			v1.z = -2.0f;
			v2.z = 11.0f;

			Assert::AreEqual(0.0f, v1.Dot(v2), FLT_EPSILON);
		}

		TEST_METHOD(Cross)
		{
			Vector3 v1(2.0f, 4.0f, 3.0f);
			Vector3 v2(5.0f, 3.0f, 4.0f);

			Vector3 res = v1.Cross(v2);

			Assert::AreEqual(7.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(7.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(-14.0f, res.z, FLT_EPSILON);

			v1 = v2;
			res = v1.Cross(v2);

			Assert::AreEqual(0.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(0.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(0.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(DoubleCross)
		{
			Vector3 v1(2.0f, 4.0f, 3.0f);
			Vector3 v2(5.0f, 3.0f, 4.0f);

			Vector3 res = v1.DoubleCross(v2);

			Assert::AreEqual(77.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(-49.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(14.0f, res.z, FLT_EPSILON);

			v1 = v2;
			res = v1.DoubleCross(v2);

			Assert::AreEqual(0.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(0.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(0.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(Clamp)
		{
			Vector3 v1(1.0f, 4.0f, -5.0f);
			Vector3 v2(5.0f, 6.0f, 7.0f);
			Vector3 v3(3.0f, 9.0f, -8.0f);

			v3.Clamp(v1, v2);

			Assert::AreEqual(3.0f, v3.x, FLT_EPSILON);
			Assert::AreEqual(6.0f, v3.y, FLT_EPSILON);
			Assert::AreEqual(-5.0f, v3.z, FLT_EPSILON);
		}

		TEST_METHOD(Lerp)
		{
			Vector3 start;
			Vector3 end(1.0f, 2.0f, 3.0f);

			Vector3 res = Vector3::Lerp(start, end, 0.5f);

			Assert::AreEqual(0.5f, res.x, FLT_EPSILON);
			Assert::AreEqual(1.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(1.5f, res.z, FLT_EPSILON);

			res = Vector3::Lerp(start, end, 0.0f);

			Assert::AreEqual(start.x, res.x, FLT_EPSILON);
			Assert::AreEqual(start.y, res.y, FLT_EPSILON);
			Assert::AreEqual(start.z, res.z, FLT_EPSILON);

			res = Vector3::Lerp(start, end, 1.0f);

			Assert::AreEqual(end.x, res.x, FLT_EPSILON);
			Assert::AreEqual(end.y, res.y, FLT_EPSILON);
			Assert::AreEqual(end.z, res.z, FLT_EPSILON);
		}

		TEST_METHOD(NLerp)
		{
			Vector3 start;
			Vector3 end(1.0f, 2.0f, 3.0f);

			Vector3 res = Vector3::Nlerp(start, end, 0.5f);

			Assert::AreEqual(0.2672612f, res.x, FLT_EPSILON);
			Assert::AreEqual(0.5345224f, res.y, FLT_EPSILON);
			Assert::AreEqual(0.8017837f, res.z, FLT_EPSILON);

			start = Vector3(4.0f, 2.0f, 6.0f);
			res = Vector3::Nlerp(start, end, 0.0f);

			Assert::AreEqual(0.5345224f, res.x, FLT_EPSILON);
			Assert::AreEqual(0.2672612f, res.y, FLT_EPSILON);
			Assert::AreEqual(0.8017837f, res.z, FLT_EPSILON);

			res = Vector3::Nlerp(start, end, 1.0f);

			Assert::AreEqual(0.2672612f, res.x, FLT_EPSILON);
			Assert::AreEqual(0.5345224f, res.y, FLT_EPSILON);
			Assert::AreEqual(0.8017837f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(SLerp)
		{
			Vector3 start(-4.0f, 1.0f, -2.0f);
			Vector3 end(2.0f, -0.5f, 1.0f);
			Vector3 ori(0.0f, 0.5f, 1.5f);

			Vector3 res = Vector3::Slerp(start, end, ori, 0.4f);

			Assert::AreEqual(-2.1076228619f, res.x, FLT_EPSILON);
			Assert::AreEqual(+1.7016133070f, res.y, FLT_EPSILON);
			Assert::AreEqual(+2.4703114033f, res.z, FLT_EPSILON);

			res = Vector3::Slerp(start, end, ori, -10.4f);

			Assert::AreEqual(-4.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(+1.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(-2.0f, res.z, FLT_EPSILON);

			res = Vector3::Slerp(start, end, ori, 10.4f);

			Assert::AreEqual(+2.0000002384f, res.x, FLT_EPSILON);
			Assert::AreEqual(-0.5000002980f, res.y, FLT_EPSILON);
			Assert::AreEqual(+0.9999993443f, res.z, FLT_EPSILON);

			start.x = -3.0f;

			res = Vector3::Slerp(start, end, ori, 0.6f);

			Assert::AreEqual(+2.2061817646f, res.x, FLT_EPSILON);
			Assert::AreEqual(+0.8219375610f, res.y, FLT_EPSILON);
			Assert::AreEqual(-1.6438751221f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpPlus)
		{
			Vector3 v1(1.0f, 2.0f, 3.0f);

			Vector3 res = +v1;

			Assert::AreEqual(1.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(3.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpMinus)
		{
			Vector3 v1(1.0f, 2.0f, 3.0f);

			Vector3 res = -v1;

			Assert::AreEqual(-1.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(-2.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(-3.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpVec3MultFloat)
		{
			Vector3 v1(3.0f, 4.0f, 5.0f);

			Vector3 res = v1 * 2.0f;

			Assert::AreEqual(6.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(8.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(10.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpVec3DivFloat)
		{
			Vector3 v1(3.0f, 4.0f, 5.0f);

			Vector3 res = v1 / 2.0f;

			Assert::AreEqual(1.5f, res.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(2.5f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpVec3PlusVec3)
		{
			Vector3 v1(3.0f, 4.0f, 1.0f);
			Vector3 v2(6.0f, 8.0f, 9.0f);

			Vector3 res = v1 + v2;

			Assert::AreEqual(9.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(12.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(10.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpVec3MinusVec3)
		{
			Vector3 v1(3.0f, 4.0f, 1.0f);
			Vector3 v2(6.0f, 8.0f, 9.0f);

			Vector3 res = v1 - v2;

			Assert::AreEqual(-3.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(-4.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(-8.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpEquals)
		{
			Vector3 v1(4.0f, 5.0f, 2.0f);

			Vector3 res;
			res = v1;

			Assert::AreEqual(v1.x, res.x, FLT_EPSILON);
			Assert::AreEqual(v1.y, res.y, FLT_EPSILON);
			Assert::AreEqual(v1.z, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpEqualsRValue)
		{
			Vector3 v1(4.0f, 5.0f, 2.0f);

			Vector3 res;
			res = std::move(v1);

			Assert::AreEqual(4.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(5.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpMultEqualsFloat)
		{
			Vector3 v1(3.0f, 4.0f, 5.0f);

			v1 *= 3.0f;

			Assert::AreEqual(9.0f, v1.x, FLT_EPSILON);
			Assert::AreEqual(12.0f, v1.y, FLT_EPSILON);
			Assert::AreEqual(15.0f, v1.z, FLT_EPSILON);
		}

		TEST_METHOD(OpDivEqualsFloat)
		{
			Vector3 v1(3.0f, 4.0f, 5.0f);

			v1 /= 2.0f;

			Assert::AreEqual(1.5f, v1.x, FLT_EPSILON);
			Assert::AreEqual(2.0f, v1.y, FLT_EPSILON);
			Assert::AreEqual(2.5f, v1.z, FLT_EPSILON);
		}

		TEST_METHOD(OpPlusEqualsVec3)
		{
			Vector3 v1(3.0f, 4.0f, 5.0f);

			v1 += {3.0f, 4.0f, 5.0f};

			Assert::AreEqual(6.0f, v1.x, FLT_EPSILON);
			Assert::AreEqual(8.0f, v1.y, FLT_EPSILON);
			Assert::AreEqual(10.0f, v1.z, FLT_EPSILON);
		}

		TEST_METHOD(OpMinusEqualsVec3)
		{
			Vector3 v1(3.0f, 4.0f, 5.0f);

			v1 -= {3.0f, 4.0f, 5.0f};

			Assert::AreEqual(0.0f, v1.x, FLT_EPSILON);
			Assert::AreEqual(0.0f, v1.y, FLT_EPSILON);
			Assert::AreEqual(0.0f, v1.z, FLT_EPSILON);
		}

		TEST_METHOD(OpComparator)
		{
			Vector3 v1(1.0f, 2.0f, 3.0f);
			Vector3 v2;

			Assert::AreEqual(false, v1 == v2);

			v2 = v1;
			Assert::AreEqual(true, v1 == v2);
		}

		TEST_METHOD(OpComparatorDiff)
		{
			Vector3 v1(1.0f, 2.0f, 3.0f);
			Vector3 v2;

			Assert::AreEqual(true, v1 != v2);

			v2 = v1;
			Assert::AreEqual(false, v1 != v2);
		}

		TEST_METHOD(OpAccess)
		{
			Vector3 v1(2.0f, 4.0f, 6.0f);
			v1[0] = 5.0f;
			v1[1] = 8.0f;
			v1[2] = 12.0f;

			Assert::AreEqual(5.0f, v1.x, FLT_EPSILON);
			Assert::AreEqual(8.0f, v1.y, FLT_EPSILON);
			Assert::AreEqual(12.0f, v1.z, FLT_EPSILON);
		}

		TEST_METHOD(OpConstAccess)
		{
			const Vector3 v1(2.0f, 4.0f, 6.0f);

			Assert::AreEqual(2.0f, v1[0], FLT_EPSILON);
			Assert::AreEqual(4.0f, v1[1], FLT_EPSILON);
			Assert::AreEqual(6.0f, v1[2], FLT_EPSILON);
		}

		TEST_METHOD(OpFloatMultVec3)
		{
			Vector3 v1(2.0f, 4.0f, 6.0f);

			Vector3 res = 3.0f * v1;

			Assert::AreEqual(6.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(12.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(18.0f, res.z, FLT_EPSILON);
		}
	};
}