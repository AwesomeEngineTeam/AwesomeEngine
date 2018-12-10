#include "CppUnitTest.h"

#include <Awesome/Maths/Vector3.hpp>
#include <Awesome/Maths/Quaternion.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace aw;

namespace MathUnitTest
{
	TEST_CLASS(Quaternion_Test)
	{
	public:
		TEST_METHOD(DefaultConstructor)
		{
			Quaternion q1;

			Assert::AreEqual(0.0f, q1.w, FLT_EPSILON);
			Assert::AreEqual(0.0f, q1.x, FLT_EPSILON);
			Assert::AreEqual(0.0f, q1.y, FLT_EPSILON);
			Assert::AreEqual(0.0f, q1.z, FLT_EPSILON);
		}

		TEST_METHOD(ValueConstructor)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);

			Assert::AreEqual(1.0f, q1.w, FLT_EPSILON);
			Assert::AreEqual(2.0f, q1.x, FLT_EPSILON);
			Assert::AreEqual(3.0f, q1.y, FLT_EPSILON);
			Assert::AreEqual(4.0f, q1.z, FLT_EPSILON);
		}

		TEST_METHOD(AngleAxisConstructor)
		{
			Vector3 v1(1.0f, 2.0f, 3.0f);
			Quaternion q1(5.0f, v1);

			v1.Normalize();
			v1 *= sinf(5.0f / 2);

			Assert::AreEqual(cosf(5.0f / 2.0f), q1.w, FLT_EPSILON);
			Assert::AreEqual(v1.x, q1.x, FLT_EPSILON);
			Assert::AreEqual(v1.y, q1.y, FLT_EPSILON);
			Assert::AreEqual(v1.z, q1.z, FLT_EPSILON);
		}

		TEST_METHOD(CopyConstructor)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
			Quaternion q2(q1);

			Assert::AreEqual(1.0f, q2.w, FLT_EPSILON);
			Assert::AreEqual(2.0f, q2.x, FLT_EPSILON);
			Assert::AreEqual(3.0f, q2.y, FLT_EPSILON);
			Assert::AreEqual(4.0f, q2.z, FLT_EPSILON);
		}

		TEST_METHOD(RValueCopyConstructor)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
			Quaternion q2(std::move(q1));

			Assert::AreEqual(1.0f, q2.w, FLT_EPSILON);
			Assert::AreEqual(2.0f, q2.x, FLT_EPSILON);
			Assert::AreEqual(3.0f, q2.y, FLT_EPSILON);
			Assert::AreEqual(4.0f, q2.z, FLT_EPSILON);
		}

		TEST_METHOD(GetNorm)
		{
			Quaternion q1(4.0f, 2.0f, 6.0f, 5.0f);

			Assert::AreEqual(sqrtf(81.0f), q1.Norm(), FLT_EPSILON);
		}

		TEST_METHOD(GetNormSq)
		{
			Quaternion q1(4.0f, 2.0f, 6.0f, 5.0f);

			Assert::AreEqual(81.0f, q1.SquaredNorm(), FLT_EPSILON);
		}

		TEST_METHOD(GetConjugated)
		{
			Quaternion q1(4.0f, 2.0f, 6.0f, 5.0f);
			Quaternion q2 = q1.GetConjugated();

			Assert::AreEqual(4.0f, q1.w, FLT_EPSILON);
			Assert::AreEqual(2.0f, q1.x, FLT_EPSILON);
			Assert::AreEqual(6.0f, q1.y, FLT_EPSILON);
			Assert::AreEqual(5.0f, q1.z, FLT_EPSILON);

			Assert::AreEqual(4.0f, q2.w, FLT_EPSILON);
			Assert::AreEqual(-2.0f, q2.x, FLT_EPSILON);
			Assert::AreEqual(-6.0f, q2.y, FLT_EPSILON);
			Assert::AreEqual(-5.0f, q2.z, FLT_EPSILON);
		}

		TEST_METHOD(GetInversed)
		{
			Quaternion q1(4.0f, 2.0f, 6.0f, 5.0f);
			Quaternion q2 = q1.GetInversed();

			Assert::AreEqual(4.0f, q1.w, FLT_EPSILON);
			Assert::AreEqual(2.0f, q1.x, FLT_EPSILON);
			Assert::AreEqual(6.0f, q1.y, FLT_EPSILON);
			Assert::AreEqual(5.0f, q1.z, FLT_EPSILON);

			Assert::AreEqual(0.0493827f, q2.w, FLT_EPSILON);
			Assert::AreEqual(-0.0246913f, q2.x, FLT_EPSILON);
			Assert::AreEqual(-0.0740740f, q2.y, FLT_EPSILON);
			Assert::AreEqual(-0.0617283f, q2.z, FLT_EPSILON);
		}

		TEST_METHOD(GetNormalized)
		{
			Quaternion q1(4.0f, 2.0f, 6.0f, 5.0f);
			Quaternion q2 = q1.GetNormalized();

			Assert::AreEqual(4.0f, q1.w, FLT_EPSILON);
			Assert::AreEqual(2.0f, q1.x, FLT_EPSILON);
			Assert::AreEqual(6.0f, q1.y, FLT_EPSILON);
			Assert::AreEqual(5.0f, q1.z, FLT_EPSILON);

			Assert::AreEqual(0.4444444f, q2.w, FLT_EPSILON);
			Assert::AreEqual(0.2222222f, q2.x, FLT_EPSILON);
			Assert::AreEqual(0.6666666f, q2.y, FLT_EPSILON);
			Assert::AreEqual(0.5555555f, q2.z, FLT_EPSILON);
		}

		TEST_METHOD(GetClamped)
		{
			Quaternion q1(1.0f, 4.0f, -5.0f, 3.0f);
			Quaternion q2(5.0f, 6.0f, 7.0f, 5.0f);
			Quaternion q3(3.0f, 9.0f, -8.0f, 4.0f);

			Quaternion q4 = q3.GetClamped(q1, q2);

			Assert::AreEqual(3.0f, q3.w, FLT_EPSILON);
			Assert::AreEqual(9.0f, q3.x, FLT_EPSILON);
			Assert::AreEqual(-8.0f, q3.y, FLT_EPSILON);
			Assert::AreEqual(4.0f, q3.z, FLT_EPSILON);

			Assert::AreEqual(3.0f, q4.w, FLT_EPSILON);
			Assert::AreEqual(6.0f, q4.x, FLT_EPSILON);
			Assert::AreEqual(-5.0f, q4.y, FLT_EPSILON);
			Assert::AreEqual(4.0f, q4.z, FLT_EPSILON);
		}

		TEST_METHOD(GetEulerAngles)
		{
			Quaternion q(0.7095140219f, 0.4366602898f, 0.5349978805f, 0.1403384954f);
			Vector3 res = q.EulerAngles();

			Assert::AreEqual(28.0000029073f, res.x, FLT_EPSILON);
			Assert::AreEqual(86.9999847412f, res.y, FLT_EPSILON);
			Assert::AreEqual(48.9999923706f, res.z, FLT_EPSILON);

			q = Quaternion(0.8223631382f, 0.3919038177f, 0.2005621493f, 0.3604233861f);
			res = q.EulerAngles();

			Assert::AreEqual(30.000001f, res.x, FLT_EPSILON);
			Assert::AreEqual(45.000000f, res.y, FLT_EPSILON);
			Assert::AreEqual(59.999996f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(Conjugate)
		{
			Quaternion q1(4.0f, 2.0f, 6.0f, 5.0f);
			q1.Conjugate();

			Assert::AreEqual(4.0f, q1.w, FLT_EPSILON);
			Assert::AreEqual(-2.0f, q1.x, FLT_EPSILON);
			Assert::AreEqual(-6.0f, q1.y, FLT_EPSILON);
			Assert::AreEqual(-5.0f, q1.z, FLT_EPSILON);
		}

		TEST_METHOD(Inverse)
		{
			Quaternion q1(4.0f, 2.0f, 6.0f, 5.0f);
			q1.Inverse();

			Assert::AreEqual(0.0493827f, q1.w, FLT_EPSILON);
			Assert::AreEqual(-0.0246913f, q1.x, FLT_EPSILON);
			Assert::AreEqual(-0.0740740f, q1.y, FLT_EPSILON);
			Assert::AreEqual(-0.0617283f, q1.z, FLT_EPSILON);
		}

		TEST_METHOD(Normalize)
		{
			Quaternion q1(4.0f, 2.0f, 6.0f, 5.0f);
			q1.Normalize();

			Assert::AreEqual(0.4444444f, q1.w, FLT_EPSILON);
			Assert::AreEqual(0.2222222f, q1.x, FLT_EPSILON);
			Assert::AreEqual(0.6666666f, q1.y, FLT_EPSILON);
			Assert::AreEqual(0.5555555f, q1.z, FLT_EPSILON);
		}

		TEST_METHOD(Dot)
		{
			Quaternion q1(1.0f, 4.0f, -5.0f, 3.0f);
			Quaternion q2(5.0f, 6.0f, 7.0f, 5.0f);

			Assert::AreEqual(9.0f, q1.Dot(q2), FLT_EPSILON);
		}

		TEST_METHOD(Cross)
		{
			Quaternion q1(1.0f, 4.0f, -5.0f, 3.0f);
			Quaternion q2(5.0f, 6.0f, 7.0f, 5.0f);

			Quaternion res = q1.Cross(q2);

			Assert::AreEqual(1, res.w, FLT_EPSILON);
			Assert::AreEqual(-46.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(-2.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(58.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(Clamp)
		{
			Quaternion q1(1.0f, 4.0f, -5.0f, 3.0f);
			Quaternion q2(5.0f, 6.0f, 7.0f, 5.0f);
			Quaternion q3(3.0f, 9.0f, -8.0f, 4.0f);

			q3.Clamp(q1, q2);

			Assert::AreEqual(3.0f, q3.w, FLT_EPSILON);
			Assert::AreEqual(6.0f, q3.x, FLT_EPSILON);
			Assert::AreEqual(-5.0f, q3.y, FLT_EPSILON);
			Assert::AreEqual(4.0f, q3.z, FLT_EPSILON);
		}

		TEST_METHOD(FromEulerAnglesVec3)
		{
			Vector3 v(0.52359877559f, 1.0471975512f, 0.52359877559f);
			Quaternion q = Quaternion::FromEulerAngles(v);

			Assert::AreEqual(0.8415088f, q.w, FLT_EPSILON);
			Assert::AreEqual(0.3415054f, q.x, FLT_EPSILON);
			Assert::AreEqual(0.4084925f, q.y, FLT_EPSILON);
			Assert::AreEqual(0.0915060f, q.z, FLT_EPSILON);
		}

		TEST_METHOD(Lerp)
		{
			Quaternion start;
			Quaternion end(1.0f, 2.0f, 3.0f, 4.0f);

			Quaternion res = Quaternion::Lerp(start, end, 0.5f);

			Assert::AreEqual(0.5f, res.w, FLT_EPSILON);
			Assert::AreEqual(1.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(1.5f, res.y, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.z, FLT_EPSILON);

			res = Quaternion::Lerp(start, end, 0.0f);

			Assert::AreEqual(start.w, res.w, FLT_EPSILON);
			Assert::AreEqual(start.x, res.x, FLT_EPSILON);
			Assert::AreEqual(start.y, res.y, FLT_EPSILON);
			Assert::AreEqual(start.z, res.z, FLT_EPSILON);

			res = Quaternion::Lerp(start, end, 1.0f);

			Assert::AreEqual(end.w, res.w, FLT_EPSILON);
			Assert::AreEqual(end.x, res.x, FLT_EPSILON);
			Assert::AreEqual(end.y, res.y, FLT_EPSILON);
			Assert::AreEqual(end.z, res.z, FLT_EPSILON);
		}

		TEST_METHOD(NLerp)
		{
			Quaternion start;
			Quaternion end(1.0f, 2.0f, 3.0f, 4.0f);

			Quaternion res = Quaternion::NLerp(start, end, 0.5f);

			Assert::AreEqual(0.1825741f, res.w, FLT_EPSILON);
			Assert::AreEqual(0.3651483f, res.x, FLT_EPSILON);
			Assert::AreEqual(0.5477225f, res.y, FLT_EPSILON);
			Assert::AreEqual(0.7302967f, res.z, FLT_EPSILON);
			
			start = Quaternion(4.0f, 2.0f, 6.0f, 7.0f);
			res = Quaternion::NLerp(start, end, 0.0f);

			Assert::AreEqual(0.3903600f, res.w, FLT_EPSILON);
			Assert::AreEqual(0.1951800f, res.x, FLT_EPSILON);
			Assert::AreEqual(0.5855400f, res.y, FLT_EPSILON);
			Assert::AreEqual(0.6831300f, res.z, FLT_EPSILON);

			res = Quaternion::NLerp(start, end, 1.0f);

			Assert::AreEqual(0.1825741f, res.w, FLT_EPSILON);
			Assert::AreEqual(0.3651483f, res.x, FLT_EPSILON);
			Assert::AreEqual(0.5477225f, res.y, FLT_EPSILON);
			Assert::AreEqual(0.7302967f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(SLerp)
		{
			Quaternion start;
			Quaternion end(1.0f, 2.0f, 3.0f, 4.0f);

			Quaternion res = Quaternion::SLerp(start, end, 0.5f);

			Assert::AreEqual(0.7071068f, res.w, FLT_EPSILON);
			Assert::AreEqual(1.4142135f, res.x, FLT_EPSILON);
			Assert::AreEqual(2.1213203f, res.y, FLT_EPSILON);
			Assert::AreEqual(2.8284271f, res.z, FLT_EPSILON);

			start = Quaternion(4.0f, 2.0f, 6.0f, 7.0f);
			res = Quaternion::SLerp(start, end, 0.0f);

			Assert::AreEqual(4.0f, res.w, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(6.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(7.0f, res.z, FLT_EPSILON);

			res = Quaternion::SLerp(start, end, 1.0f);

			Assert::AreEqual(1.0f, res.w, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(3.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(4.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpPlus)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);

			Quaternion res = +q1;

			Assert::AreEqual(1.0f, res.w, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(3.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(4.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpMinus)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);

			Quaternion res = -q1;

			Assert::AreEqual(-1.0f, res.w, FLT_EPSILON);
			Assert::AreEqual(-2.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(-3.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(-4.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(QuatMultFloat)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);

			Quaternion res = q1 * 2.0f;

			Assert::AreEqual(2.0f, res.w, FLT_EPSILON);
			Assert::AreEqual(4.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(6.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(8.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(QuatDivFloat)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);

			Quaternion res = q1 / 2.0f;

			Assert::AreEqual(0.5f, res.w, FLT_EPSILON);
			Assert::AreEqual(1.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(1.5f, res.y, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(QuatMultVec)
		{
			Vector3 v(1.0f, 2.0f, 3.0f);
			Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);

			v = q * v;

			Assert::AreEqual(1.8000001f, v.x, FLT_EPSILON);
			Assert::AreEqual(1.9999999f, v.y, FLT_EPSILON);
			Assert::AreEqual(2.6000001f, v.z, FLT_EPSILON);
		}

		TEST_METHOD(QuatPlusQuat)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
			Quaternion q2(5.0f, 6.0f, 7.0f, 5.0f);

			Quaternion res = q1 + q2;

			Assert::AreEqual(6.0f, res.w, FLT_EPSILON);
			Assert::AreEqual(8.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(10.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(9.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(QuatMinusQuat)
		{
			Quaternion q1(1.0f, 6.0f, 9.0f, 4.0f);
			Quaternion q2(5.0f, 2.5f, 4.0f, 5.0f);

			Quaternion res = q1 - q2;

			Assert::AreEqual(-4.0f, res.w, FLT_EPSILON);
			Assert::AreEqual(3.5f, res.x, FLT_EPSILON);
			Assert::AreEqual(5.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(-1.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(QuatMultQuat)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
			Quaternion q2(5.0f, 6.0f, 7.0f, 5.0f);

			Quaternion res = q1 * q2;

			Assert::AreEqual(-48.0f, res.w, FLT_EPSILON);
			Assert::AreEqual(3.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(36.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(21.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpEquals)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);

			Quaternion res;
			res = q1;

			Assert::AreEqual(1.0f, res.w, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(3.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(4.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpEqualsRValue)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);

			Quaternion res;
			res = std::move(q1);

			Assert::AreEqual(1.0f, res.w, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(3.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(4.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpMultEqualsFloat)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);

			q1 *= 2.0f;

			Assert::AreEqual(2.0f, q1.w, FLT_EPSILON);
			Assert::AreEqual(4.0f, q1.x, FLT_EPSILON);
			Assert::AreEqual(6.0f, q1.y, FLT_EPSILON);
			Assert::AreEqual(8.0f, q1.z, FLT_EPSILON);
		}

		TEST_METHOD(OpDivEqualsFloat)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);

			q1 /= 2.0f;

			Assert::AreEqual(0.5f, q1.w, FLT_EPSILON);
			Assert::AreEqual(1.0f, q1.x, FLT_EPSILON);
			Assert::AreEqual(1.5f, q1.y, FLT_EPSILON);
			Assert::AreEqual(2.0f, q1.z, FLT_EPSILON);
		}

		TEST_METHOD(OpPlusEqualsQuat)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
			Quaternion q2(5.0f, 6.0f, 7.0f, 5.0f);

			q1 += q2;

			Assert::AreEqual(6.0f, q1.w, FLT_EPSILON);
			Assert::AreEqual(8.0f, q1.x, FLT_EPSILON);
			Assert::AreEqual(10.0f, q1.y, FLT_EPSILON);
			Assert::AreEqual(9.0f, q1.z, FLT_EPSILON);
		}

		TEST_METHOD(OpMinusEqualsQuat)
		{
			Quaternion q1(1.0f, 6.0f, 9.0f, 4.0f);
			Quaternion q2(5.0f, 2.5f, 4.0f, 5.0f);

			q1 -= q2;

			Assert::AreEqual(-4.0f, q1.w, FLT_EPSILON);
			Assert::AreEqual(3.5f, q1.x, FLT_EPSILON);
			Assert::AreEqual(5.0f, q1.y, FLT_EPSILON);
			Assert::AreEqual(-1.0f, q1.z, FLT_EPSILON);
		}

		TEST_METHOD(OpMultEqualsQuat)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
			Quaternion q2(5.0f, 6.0f, 7.0f, 5.0f);

			q1 *= q2;

			Assert::AreEqual(-48.0f, q1.w, FLT_EPSILON);
			Assert::AreEqual(3.0f, q1.x, FLT_EPSILON);
			Assert::AreEqual(36.0f, q1.y, FLT_EPSILON);
			Assert::AreEqual(21.0f, q1.z, FLT_EPSILON);
		}

		TEST_METHOD(OpComparator)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
			Quaternion q2;

			Assert::AreEqual(false, q1 == q2);

			q2 = q1;
			Assert::AreEqual(true, q1 == q2);
		}

		TEST_METHOD(OpComparatorDiff)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
			Quaternion q2;

			Assert::AreEqual(true, q1 != q2);

			q2 = q1;
			Assert::AreEqual(false, q1 != q2);
		}

		TEST_METHOD(OpAccess)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
			q1[0] = 5.0f;
			q1[1] = 8.0f;
			q1[2] = 12.0f;
			q1[3] = 4.0f;

			Assert::AreEqual(5.0f, q1.w, FLT_EPSILON);
			Assert::AreEqual(8.0f, q1.x, FLT_EPSILON);
			Assert::AreEqual(12.0f, q1.y, FLT_EPSILON);
			Assert::AreEqual(4.0f, q1.z, FLT_EPSILON);
		}

		TEST_METHOD(OpConstAccess)
		{
			const Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);

			Assert::AreEqual(1.0f, q1[0], FLT_EPSILON);
			Assert::AreEqual(2.0f, q1[1], FLT_EPSILON);
			Assert::AreEqual(3.0f, q1[2], FLT_EPSILON);
			Assert::AreEqual(4.0f, q1[3], FLT_EPSILON);
		}

		TEST_METHOD(OpFloatMultQuat)
		{
			Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);

			Quaternion q2 = 2.0f * q1;

			Assert::AreEqual(2.0f, q2.w, FLT_EPSILON);
			Assert::AreEqual(4.0f, q2.x, FLT_EPSILON);
			Assert::AreEqual(6.0f, q2.y, FLT_EPSILON);
			Assert::AreEqual(8.0f, q2.z, FLT_EPSILON);
		}
	};
}