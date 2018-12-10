#include "CppUnitTest.h"

#include <Awesome/Maths/Matrix3.hpp>
#include <Awesome/Maths/Matrix4.hpp>
#include <Awesome/Maths/Vector3.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace aw;

namespace MathUnitTest
{
	TEST_CLASS(Matrix3_Test)
	{
	public:

		TEST_METHOD(DefaultConstructor)
		{
			Matrix3 m;

			Assert::AreEqual(0.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m22, FLT_EPSILON);
		}

		TEST_METHOD(ValueConstructor)
		{
			Matrix3 m(
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			);

			Assert::AreEqual(1.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(2.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(3.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(7.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(9.0f, m.m22, FLT_EPSILON);
		}

		TEST_METHOD(CopyConstructor)
		{
			Matrix3 m1(
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			);

			Matrix3 m2(m1);

			Assert::AreEqual(1.0f, m2.m00, FLT_EPSILON);
			Assert::AreEqual(2.0f, m2.m01, FLT_EPSILON);
			Assert::AreEqual(3.0f, m2.m02, FLT_EPSILON);
			Assert::AreEqual(4.0f, m2.m10, FLT_EPSILON);
			Assert::AreEqual(5.0f, m2.m11, FLT_EPSILON);
			Assert::AreEqual(6.0f, m2.m12, FLT_EPSILON);
			Assert::AreEqual(7.0f, m2.m20, FLT_EPSILON);
			Assert::AreEqual(8.0f, m2.m21, FLT_EPSILON);
			Assert::AreEqual(9.0f, m2.m22, FLT_EPSILON);
		}

		TEST_METHOD(CopyMat4Constructor)
		 {
			 Matrix4 m1(
				 1.0f, 2.0f, 3.0f, 4.0f,
				 5.0f, 6.0f, 7.0f, 8.0f,
				 9.0f, 10.0f, 11.0f, 12.0f,
				 13.0f, 14.0f, 15.0f, 16.0f
			 );

			 Matrix3 m2(m1);

			 Assert::AreEqual(1.0f, m2.m00, FLT_EPSILON);
			 Assert::AreEqual(2.0f, m2.m01, FLT_EPSILON);
			 Assert::AreEqual(3.0f, m2.m02, FLT_EPSILON);
			 Assert::AreEqual(5.0f, m2.m10, FLT_EPSILON);
			 Assert::AreEqual(6.0f, m2.m11, FLT_EPSILON);
			 Assert::AreEqual(7.0f, m2.m12, FLT_EPSILON);
			 Assert::AreEqual(9.0f, m2.m20, FLT_EPSILON);
			 Assert::AreEqual(10.0f, m2.m21, FLT_EPSILON);
			 Assert::AreEqual(11.0f, m2.m22, FLT_EPSILON);
		 }

		TEST_METHOD(RValueCopyConstructor)
		{
			Matrix3 m1(
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			);

			Matrix3 m2(std::move(m1));

			Assert::AreEqual(1.0f, m2.m00, FLT_EPSILON);
			Assert::AreEqual(2.0f, m2.m01, FLT_EPSILON);
			Assert::AreEqual(3.0f, m2.m02, FLT_EPSILON);
			Assert::AreEqual(4.0f, m2.m10, FLT_EPSILON);
			Assert::AreEqual(5.0f, m2.m11, FLT_EPSILON);
			Assert::AreEqual(6.0f, m2.m12, FLT_EPSILON);
			Assert::AreEqual(7.0f, m2.m20, FLT_EPSILON);
			Assert::AreEqual(8.0f, m2.m21, FLT_EPSILON);
			Assert::AreEqual(9.0f, m2.m22, FLT_EPSILON);
		}

		TEST_METHOD(GetTrace)
		{
			Matrix3 m(
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			);

			Assert::AreEqual(15.0f, m.Trace(), FLT_EPSILON);
		}

		TEST_METHOD(GetDeterminant)
		{
			Matrix3 m(
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			);

			Assert::AreEqual(0.0f, m.Determinant(), FLT_EPSILON);

			m.m20 = 3.0f;

			Assert::AreEqual(12.0f, m.Determinant(), FLT_EPSILON);
		}

		TEST_METHOD(GetInversed)
		{
			Matrix3 m(
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			);

			Matrix3 inv = m.GetInversed(); // Not inversible

			Assert::AreEqual(1.0f, inv.m00, FLT_EPSILON);
			Assert::AreEqual(4.0f, inv.m10, FLT_EPSILON);
			Assert::AreEqual(7.0f, inv.m20, FLT_EPSILON);
			Assert::AreEqual(2.0f, inv.m01, FLT_EPSILON);
			Assert::AreEqual(5.0f, inv.m11, FLT_EPSILON);
			Assert::AreEqual(8.0f, inv.m21, FLT_EPSILON);
			Assert::AreEqual(3.0f, inv.m02, FLT_EPSILON);
			Assert::AreEqual(6.0f, inv.m12, FLT_EPSILON);
			Assert::AreEqual(9.0f, inv.m22, FLT_EPSILON);

			m.m20 = 3.0f;
			inv = m.GetInversed(); // Inversible

			Assert::AreEqual(-0.25f, inv.m00, FLT_EPSILON);
			Assert::AreEqual(-1.5f, inv.m10, FLT_EPSILON);
			Assert::AreEqual(1.4166666f, inv.m20, FLT_EPSILON);
			Assert::AreEqual(0.5f, inv.m01, FLT_EPSILON);
			Assert::AreEqual(0.0f, inv.m11, FLT_EPSILON);
			Assert::AreEqual(-0.1666666f, inv.m21, FLT_EPSILON);
			Assert::AreEqual(-0.25f, inv.m02, FLT_EPSILON);
			Assert::AreEqual(0.5f, inv.m12, FLT_EPSILON);
			Assert::AreEqual(-0.25f, inv.m22, FLT_EPSILON);

			Matrix3 m2(
				-0.88888888f, 0.44444444f, 0.11111111f,
				0.44444444f, 0.77777777f, 0.44444444f,
				0.11111111f, 0.44444444f, -0.88888888f
			);			

			inv = m2.GetInversed(); // Orthogonal

			Assert::AreEqual(m2.m00, inv.m00, FLT_EPSILON);
			Assert::AreEqual(m2.m10, inv.m01, FLT_EPSILON);
			Assert::AreEqual(m2.m20, inv.m02, FLT_EPSILON);
			Assert::AreEqual(m2.m01, inv.m10, FLT_EPSILON);
			Assert::AreEqual(m2.m11, inv.m11, FLT_EPSILON);
			Assert::AreEqual(m2.m21, inv.m12, FLT_EPSILON);
			Assert::AreEqual(m2.m02, inv.m20, FLT_EPSILON);
			Assert::AreEqual(m2.m12, inv.m21, FLT_EPSILON);
			Assert::AreEqual(m2.m22, inv.m22, FLT_EPSILON);
		}

		TEST_METHOD(GetTransposed)
		{
			Matrix3 m(
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			);

			Matrix3 tr = m.GetTransposed();

			Assert::AreEqual(1.0f, tr.m00, FLT_EPSILON);
			Assert::AreEqual(4.0f, tr.m01, FLT_EPSILON);
			Assert::AreEqual(7.0f, tr.m02, FLT_EPSILON);
			Assert::AreEqual(2.0f, tr.m10, FLT_EPSILON);
			Assert::AreEqual(5.0f, tr.m11, FLT_EPSILON);
			Assert::AreEqual(8.0f, tr.m12, FLT_EPSILON);
			Assert::AreEqual(3.0f, tr.m20, FLT_EPSILON);
			Assert::AreEqual(6.0f, tr.m21, FLT_EPSILON);
			Assert::AreEqual(9.0f, tr.m22, FLT_EPSILON);
		}

		TEST_METHOD(Inverse)
		{
			Matrix3 m(
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			);

			m.Inverse(); // Not inversible

			Assert::AreEqual(1.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(7.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(2.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(3.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(9.0f, m.m22, FLT_EPSILON);

			m.m20 = 3.0f;
			m.Inverse(); // Inversible

			Assert::AreEqual(-0.25f, m.m00, FLT_EPSILON);
			Assert::AreEqual(-1.5f, m.m10, FLT_EPSILON);
			Assert::AreEqual(1.4166666f, m.m20, FLT_EPSILON);
			Assert::AreEqual(0.5f, m.m01, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(-0.1666666f, m.m21, FLT_EPSILON);
			Assert::AreEqual(-0.25f, m.m02, FLT_EPSILON);
			Assert::AreEqual(0.5f, m.m12, FLT_EPSILON);
			Assert::AreEqual(-0.25f, m.m22, FLT_EPSILON);

			Matrix3 m2(
				-0.88888888f, 0.44444444f, 0.11111111f,
				0.44444444f, 0.77777777f, 0.44444444f,
				0.11111111f, 0.44444444f, -0.88888888f
			);			

			m2.Inverse(); // Orthogonal

			Assert::AreEqual(m2.m00, m2.m00, FLT_EPSILON);
			Assert::AreEqual(m2.m10, m2.m01, FLT_EPSILON);
			Assert::AreEqual(m2.m20, m2.m02, FLT_EPSILON);
			Assert::AreEqual(m2.m01, m2.m10, FLT_EPSILON);
			Assert::AreEqual(m2.m11, m2.m11, FLT_EPSILON);
			Assert::AreEqual(m2.m21, m2.m12, FLT_EPSILON);
			Assert::AreEqual(m2.m02, m2.m20, FLT_EPSILON);
			Assert::AreEqual(m2.m12, m2.m21, FLT_EPSILON);
			Assert::AreEqual(m2.m22, m2.m22, FLT_EPSILON);
		}

		TEST_METHOD(Transpose)
		{
			Matrix3 m(
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			);

			m.Transpose();

			Assert::AreEqual(1.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(7.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(2.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(3.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(9.0f, m.m22, FLT_EPSILON);
		}

		TEST_METHOD(Orthogonalize)
		{
			Matrix3 m(
				1.1111111f, 4.4444444f, 7.7777777f,
				2.2222222f, 1.1111111f, 8.8888888f,
				3.3333333f,	6.6666666f,	9.9999999f
			);

			m.Orthogonalize();

			Assert::AreEqual(+0.2672612369f, m.m00, FLT_EPSILON);
			Assert::AreEqual(+0.5345224738f, m.m10, FLT_EPSILON);
			Assert::AreEqual(+0.8017837405f, m.m20, FLT_EPSILON);
			Assert::AreEqual(+0.6637918353f, m.m01, FLT_EPSILON);
			Assert::AreEqual(-0.7052789330f, m.m11, FLT_EPSILON);
			Assert::AreEqual(+0.2489218712f, m.m21, FLT_EPSILON);
			Assert::AreEqual(+0.6985356808f, m.m02, FLT_EPSILON);
			Assert::AreEqual(+0.4656899869f, m.m12, FLT_EPSILON);
			Assert::AreEqual(-0.5433052778f, m.m22, FLT_EPSILON);
		}

		TEST_METHOD(IsOrthogonal)
		{
			Matrix3 m1(
				-0.88888888f, 0.44444444f, 0.11111111f,
				0.44444444f, 0.77777777f, 0.44444444f,
				0.11111111f, 0.44444444f, -0.88888888f
			);	

			Assert::AreEqual(true, m1.IsOrthogonal());

			Matrix3 m2(
				-0.88888888f, 0.44444444f, 0.11111111f,
				0.44444444f, 0.77777777f, 0.66666666f,
				0.11111111f, 0.44444444f, -0.88888888f
			);

			Assert::AreEqual(false, m2.IsOrthogonal());
		}

		TEST_METHOD(FromColumns)
		{
			Vector3 col1(1.0f, 2.0f, 3.0f);
			Vector3 col2(4.0f, 5.0f, 6.0f);
			Vector3 col3(7.0f, 8.0f, 9.0f);

			Matrix3 m = Matrix3::FromColumns(col1, col2, col3);

			Assert::AreEqual(1.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(2.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(3.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(7.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(9.0f, m.m22, FLT_EPSILON);
		}

		TEST_METHOD(FromRows)
		{
			Vector3 row1(1.0f, 2.0f, 3.0f);
			Vector3 row2(4.0f, 5.0f, 6.0f);
			Vector3 row3(7.0f, 8.0f, 9.0f);

			Matrix3 m = Matrix3::FromRows(row1, row2, row3);

			Assert::AreEqual(1.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(2.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(3.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(7.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(9.0f, m.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpPlus)
		{
			Matrix3 m(
				1.0f, 2.0f, 3.0f, 
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			Matrix3 res = +m;

			Assert::AreEqual(1.0f, res.m00, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.m01, FLT_EPSILON);
			Assert::AreEqual(3.0f, res.m02, FLT_EPSILON);
			Assert::AreEqual(4.0f, res.m10, FLT_EPSILON);
			Assert::AreEqual(5.0f, res.m11, FLT_EPSILON);
			Assert::AreEqual(6.0f, res.m12, FLT_EPSILON);
			Assert::AreEqual(7.0f, res.m20, FLT_EPSILON);
			Assert::AreEqual(8.0f, res.m21, FLT_EPSILON);
			Assert::AreEqual(9.0f, res.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpMinus)
		{
			Matrix3 m(
				1.0f, 2.0f, 3.0f, 
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f);

			Matrix3 res = -m;

			Assert::AreEqual(-1.0f, res.m00, FLT_EPSILON);
			Assert::AreEqual(-2.0f, res.m01, FLT_EPSILON);
			Assert::AreEqual(-3.0f, res.m02, FLT_EPSILON);
			Assert::AreEqual(-4.0f, res.m10, FLT_EPSILON);
			Assert::AreEqual(-5.0f, res.m11, FLT_EPSILON);
			Assert::AreEqual(-6.0f, res.m12, FLT_EPSILON);
			Assert::AreEqual(-7.0f, res.m20, FLT_EPSILON);
			Assert::AreEqual(-8.0f, res.m21, FLT_EPSILON);
			Assert::AreEqual(-9.0f, res.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpMat3MultFloat)
		{
			Matrix3 m(
				1.00000000f, 2.00000000f, 3.00000000f, 
				4.00000000f, 5.00000000f, 6.00000000f,
				7.00000000f, 8.00000000f, 9.00000000f);

			Matrix3 res = m * 2.30000000f;

			Assert::AreEqual(2.3f, res.m00, FLT_EPSILON);
			Assert::AreEqual(4.6f, res.m01, FLT_EPSILON);
			Assert::AreEqual(6.8999997f, res.m02, FLT_EPSILON);
			Assert::AreEqual(9.2f, res.m10, FLT_EPSILON);
			Assert::AreEqual(11.5f, res.m11, FLT_EPSILON);
			Assert::AreEqual(13.799999f, res.m12, FLT_EPSILON);
			Assert::AreEqual(16.0999998f, res.m20, FLT_EPSILON);
			Assert::AreEqual(18.3999999f, res.m21, FLT_EPSILON);
			Assert::AreEqual(20.699999f, res.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpMat3DivFloat)
		{
			Matrix3 m(
				1.00000000f, 2.00000000f, 3.00000000f, 
				4.00000000f, 5.00000000f, 6.00000000f,
				7.00000000f, 8.00000000f, 9.00000000f);

			Matrix3 res = m / 2.0f;

			Assert::AreEqual(0.5f, res.m00, FLT_EPSILON);
			Assert::AreEqual(1.0f, res.m01, FLT_EPSILON);
			Assert::AreEqual(1.5f, res.m02, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.m10, FLT_EPSILON);
			Assert::AreEqual(2.5f, res.m11, FLT_EPSILON);
			Assert::AreEqual(3.0f, res.m12, FLT_EPSILON);
			Assert::AreEqual(3.5f, res.m20, FLT_EPSILON);
			Assert::AreEqual(4.0f, res.m21, FLT_EPSILON);
			Assert::AreEqual(4.5f, res.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpMat3MultVec3)
		{
			Matrix3 m(
				1.00000000f, 2.00000000f, 3.00000000f,
				4.00000000f, 5.00000000f, 6.00000000f,
				7.00000000f, 8.00000000f, 9.00000000f);

			Vector3 v(1.0f, 2.0f, 3.0f);

			Vector3 res = m * v;

			Assert::AreEqual(14.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(32.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(50.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpMat3PlusMat3)
		{
			Matrix3 m1(
				1.00000000f, 2.00000000f, 3.00000000f, 
				4.00000000f, 5.00000000f, 6.00000000f,
				7.00000000f, 8.00000000f, 9.00000000f);

			Matrix3 m2(
				-1.00000000f, 3.00000000f, 1.00000000f, 
				5.00000000f, 3.00000000f, 7.00000000f,
				5.00000000f, 9.00000000f, 7.00000000f);

			Matrix3 res = m1 + m2;

			Assert::AreEqual(0.0f, res.m00, FLT_EPSILON);
			Assert::AreEqual(5.0f, res.m01, FLT_EPSILON);
			Assert::AreEqual(4.0f, res.m02, FLT_EPSILON);
			Assert::AreEqual(9.0f, res.m10, FLT_EPSILON);
			Assert::AreEqual(8.0f, res.m11, FLT_EPSILON);
			Assert::AreEqual(13.0f, res.m12, FLT_EPSILON);
			Assert::AreEqual(12.0f, res.m20, FLT_EPSILON);
			Assert::AreEqual(17.0f, res.m21, FLT_EPSILON);
			Assert::AreEqual(16.0f, res.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpMat3MinusMat3)
		{
			Matrix3 m1(
				1.00000000f, 2.00000000f, 3.00000000f, 
				4.00000000f, 5.00000000f, 6.00000000f,
				7.00000000f, 8.00000000f, 9.00000000f);

			Matrix3 m2(
				-1.00000000f, 3.00000000f, 1.00000000f, 
				5.00000000f, 3.00000000f, 7.00000000f,
				5.00000000f, 9.00000000f, 7.00000000f);

			Matrix3 res = m1 - m2;

			Assert::AreEqual(2.0f, res.m00, FLT_EPSILON);
			Assert::AreEqual(-1.0f, res.m01, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.m02, FLT_EPSILON);
			Assert::AreEqual(-1.0f, res.m10, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.m11, FLT_EPSILON);
			Assert::AreEqual(-1.0f, res.m12, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.m20, FLT_EPSILON);
			Assert::AreEqual(-1.0f, res.m21, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpMat3MultMat3)
		{
			Matrix3 m1(
				1.00000000f, 2.00000000f, 3.00000000f, 
				4.00000000f, 5.00000000f, 6.00000000f,
				7.00000000f, 8.00000000f, 9.00000000f);

			Matrix3 m2(
				-1.00000000f, 3.00000000f, 1.00000000f, 
				5.00000000f, 3.00000000f, 7.00000000f,
				5.00000000f, 9.00000000f, 7.00000000f);

			Matrix3 res = m1 * m2;

			Assert::AreEqual(24.0f, res.m00, FLT_EPSILON);
			Assert::AreEqual(36.0f, res.m01, FLT_EPSILON);
			Assert::AreEqual(36.0f, res.m02, FLT_EPSILON);
			Assert::AreEqual(51.0f, res.m10, FLT_EPSILON);
			Assert::AreEqual(81.0f, res.m11, FLT_EPSILON);
			Assert::AreEqual(81.0f, res.m12, FLT_EPSILON);
			Assert::AreEqual(78.0f, res.m20, FLT_EPSILON);
			Assert::AreEqual(126.0f, res.m21, FLT_EPSILON);
			Assert::AreEqual(126.0f, res.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpEquals)
		{
			Matrix3 m1(
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			);

			Matrix3 m2;
			m2 = m1;

			Assert::AreEqual(1.0f, m2.m00, FLT_EPSILON);
			Assert::AreEqual(2.0f, m2.m01, FLT_EPSILON);
			Assert::AreEqual(3.0f, m2.m02, FLT_EPSILON);
			Assert::AreEqual(4.0f, m2.m10, FLT_EPSILON);
			Assert::AreEqual(5.0f, m2.m11, FLT_EPSILON);
			Assert::AreEqual(6.0f, m2.m12, FLT_EPSILON);
			Assert::AreEqual(7.0f, m2.m20, FLT_EPSILON);
			Assert::AreEqual(8.0f, m2.m21, FLT_EPSILON);
			Assert::AreEqual(9.0f, m2.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpEqualsRValue)
		{
			Matrix3 m1(
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f
			);

			Matrix3 m2;
			m2 = std::move(m1);

			Assert::AreEqual(1.0f, m2.m00, FLT_EPSILON);
			Assert::AreEqual(2.0f, m2.m01, FLT_EPSILON);
			Assert::AreEqual(3.0f, m2.m02, FLT_EPSILON);
			Assert::AreEqual(4.0f, m2.m10, FLT_EPSILON);
			Assert::AreEqual(5.0f, m2.m11, FLT_EPSILON);
			Assert::AreEqual(6.0f, m2.m12, FLT_EPSILON);
			Assert::AreEqual(7.0f, m2.m20, FLT_EPSILON);
			Assert::AreEqual(8.0f, m2.m21, FLT_EPSILON);
			Assert::AreEqual(9.0f, m2.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpMat3MultEqualsFloat)
		{
			Matrix3 m(
				1.00000000f, 2.00000000f, 3.00000000f, 
				4.00000000f, 5.00000000f, 6.00000000f,
				7.00000000f, 8.00000000f, 9.00000000f);

			m *= 2.30000000f;

			Assert::AreEqual(2.3f, m.m00, FLT_EPSILON);
			Assert::AreEqual(4.6f, m.m01, FLT_EPSILON);
			Assert::AreEqual(6.8999997f, m.m02, FLT_EPSILON);
			Assert::AreEqual(9.2f, m.m10, FLT_EPSILON);
			Assert::AreEqual(11.5f, m.m11, FLT_EPSILON);
			Assert::AreEqual(13.799999f, m.m12, FLT_EPSILON);
			Assert::AreEqual(16.0999998f, m.m20, FLT_EPSILON);
			Assert::AreEqual(18.3999999f, m.m21, FLT_EPSILON);
			Assert::AreEqual(20.699999f, m.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpMat3DivEqualsFloat)
		{
			Matrix3 m(
				1.00000000f, 2.00000000f, 3.00000000f, 
				4.00000000f, 5.00000000f, 6.00000000f,
				7.00000000f, 8.00000000f, 9.00000000f);

			m /= 2.0f;

			Assert::AreEqual(0.5f, m.m00, FLT_EPSILON);
			Assert::AreEqual(1.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(1.5f, m.m02, FLT_EPSILON);
			Assert::AreEqual(2.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(2.5f, m.m11, FLT_EPSILON);
			Assert::AreEqual(3.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(3.5f, m.m20, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(4.5f, m.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpMat3PlusEqualsMat3)
		{
			Matrix3 m1(
				1.00000000f, 2.00000000f, 3.00000000f, 
				4.00000000f, 5.00000000f, 6.00000000f,
				7.00000000f, 8.00000000f, 9.00000000f);

			Matrix3 m2(
				-1.00000000f, 3.00000000f, 1.00000000f, 
				5.00000000f, 3.00000000f, 7.00000000f,
				5.00000000f, 9.00000000f, 7.00000000f);

			m1 += m2;

			Assert::AreEqual(0.0f, m1.m00, FLT_EPSILON);
			Assert::AreEqual(5.0f, m1.m01, FLT_EPSILON);
			Assert::AreEqual(4.0f, m1.m02, FLT_EPSILON);
			Assert::AreEqual(9.0f, m1.m10, FLT_EPSILON);
			Assert::AreEqual(8.0f, m1.m11, FLT_EPSILON);
			Assert::AreEqual(13.0f, m1.m12, FLT_EPSILON);
			Assert::AreEqual(12.0f, m1.m20, FLT_EPSILON);
			Assert::AreEqual(17.0f, m1.m21, FLT_EPSILON);
			Assert::AreEqual(16.0f, m1.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpMat3MinusEqualsMat3)
		{
			Matrix3 m1(
				1.00000000f, 2.00000000f, 3.00000000f, 
				4.00000000f, 5.00000000f, 6.00000000f,
				7.00000000f, 8.00000000f, 9.00000000f);

			Matrix3 m2(
				-1.00000000f, 3.00000000f, 1.00000000f, 
				5.00000000f, 3.00000000f, 7.00000000f,
				5.00000000f, 9.00000000f, 7.00000000f);

			m1 -= m2;

			Assert::AreEqual(2.0f, m1.m00, FLT_EPSILON);
			Assert::AreEqual(-1.0f, m1.m01, FLT_EPSILON);
			Assert::AreEqual(2.0f, m1.m02, FLT_EPSILON);
			Assert::AreEqual(-1.0f, m1.m10, FLT_EPSILON);
			Assert::AreEqual(2.0f, m1.m11, FLT_EPSILON);
			Assert::AreEqual(-1.0f, m1.m12, FLT_EPSILON);
			Assert::AreEqual(2.0f, m1.m20, FLT_EPSILON);
			Assert::AreEqual(-1.0f, m1.m21, FLT_EPSILON);
			Assert::AreEqual(2.0f, m1.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpMat3MultEqualsMat3)
		{
			Matrix3 m1(
				1.00000000f, 2.00000000f, 3.00000000f, 
				4.00000000f, 5.00000000f, 6.00000000f,
				7.00000000f, 8.00000000f, 9.00000000f);

			Matrix3 m2(
				-1.00000000f, 3.00000000f, 1.00000000f, 
				5.00000000f, 3.00000000f, 7.00000000f,
				5.00000000f, 9.00000000f, 7.00000000f);

			m1 *= m2;

			Assert::AreEqual(24.0f, m1.m00, FLT_EPSILON);
			Assert::AreEqual(36.0f, m1.m01, FLT_EPSILON);
			Assert::AreEqual(36.0f, m1.m02, FLT_EPSILON);
			Assert::AreEqual(51.0f, m1.m10, FLT_EPSILON);
			Assert::AreEqual(81.0f, m1.m11, FLT_EPSILON);
			Assert::AreEqual(81.0f, m1.m12, FLT_EPSILON);
			Assert::AreEqual(78.0f, m1.m20, FLT_EPSILON);
			Assert::AreEqual(126.0f, m1.m21, FLT_EPSILON);
			Assert::AreEqual(126.0f, m1.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpComparator)
		{
			Matrix3 m1(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
			Matrix3 m2;

			Assert::AreEqual(false, m1 == m2);

			m2 = m1;
			Assert::AreEqual(true, m1 == m2);
		}

		TEST_METHOD(OpComparatorDiff)
		{
			Matrix3 m1(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
			Matrix3 m2;

			Assert::AreEqual(true, m1 != m2);

			m2 = m1;
			Assert::AreEqual(false, m1 != m2);
		}

		TEST_METHOD(OpAccess)
		{
			Matrix3 m1(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
			m1[0] = 5.0f;
			m1[1] = 8.0f;
			m1[2] = 12.0f;
			m1[3] = -3.0f;
			m1[4] = 0.0f;
			m1[5] = 4.0f;
			m1[6] = 5.0f;
			m1[7] = 1.0f;
			m1[8] = -2.0f;

			Assert::AreEqual(5.0f, m1.m00, FLT_EPSILON);
			Assert::AreEqual(8.0f, m1.m10, FLT_EPSILON);
			Assert::AreEqual(12.0f, m1.m20, FLT_EPSILON);
			Assert::AreEqual(-3.0f, m1.m01, FLT_EPSILON);
			Assert::AreEqual(0.0f, m1.m11, FLT_EPSILON);
			Assert::AreEqual(4.0f, m1.m21, FLT_EPSILON);
			Assert::AreEqual(5.0f, m1.m02, FLT_EPSILON);
			Assert::AreEqual(1.0f, m1.m12, FLT_EPSILON);
			Assert::AreEqual(-2.0f, m1.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpConstAccess)
		{
			const Matrix3 m1(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);

			Assert::AreEqual(1.0f, m1[0], FLT_EPSILON);
			Assert::AreEqual(4.0f, m1[1], FLT_EPSILON);
			Assert::AreEqual(7.0f, m1[2], FLT_EPSILON);
			Assert::AreEqual(2.0f, m1[3], FLT_EPSILON);
			Assert::AreEqual(5.0f, m1[4], FLT_EPSILON);
			Assert::AreEqual(8.0f, m1[5], FLT_EPSILON);
			Assert::AreEqual(3.0f, m1[6], FLT_EPSILON);
			Assert::AreEqual(6.0f, m1[7], FLT_EPSILON);
			Assert::AreEqual(9.0f, m1[8], FLT_EPSILON);
		}

		TEST_METHOD(OpAccess2)
		{
			Matrix3 m1(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
			m1(0, 0) = 5.0f;
			m1(1, 0) = 8.0f;
			m1(2, 0) = 12.0f;
			m1(0, 1) = -3.0f;
			m1(1, 1) = 0.0f;
			m1(2, 1) = 4.0f;
			m1(0, 2) = 5.0f;
			m1(1, 2) = 1.0f;
			m1(2, 2) = -2.0f;

			Assert::AreEqual(5.0f, m1.m00, FLT_EPSILON);
			Assert::AreEqual(8.0f, m1.m10, FLT_EPSILON);
			Assert::AreEqual(12.0f, m1.m20, FLT_EPSILON);
			Assert::AreEqual(-3.0f, m1.m01, FLT_EPSILON);
			Assert::AreEqual(0.0f, m1.m11, FLT_EPSILON);
			Assert::AreEqual(4.0f, m1.m21, FLT_EPSILON);
			Assert::AreEqual(5.0f, m1.m02, FLT_EPSILON);
			Assert::AreEqual(1.0f, m1.m12, FLT_EPSILON);
			Assert::AreEqual(-2.0f, m1.m22, FLT_EPSILON);
		}

		TEST_METHOD(OpConstAccess2)
		{
			const Matrix3 m1(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);

			Assert::AreEqual(1.0f, m1(0, 0), FLT_EPSILON);
			Assert::AreEqual(4.0f, m1(1, 0), FLT_EPSILON);
			Assert::AreEqual(7.0f, m1(2, 0), FLT_EPSILON);
			Assert::AreEqual(2.0f, m1(0, 1), FLT_EPSILON);
			Assert::AreEqual(5.0f, m1(1, 1), FLT_EPSILON);
			Assert::AreEqual(8.0f, m1(2, 1), FLT_EPSILON);
			Assert::AreEqual(3.0f, m1(0, 2), FLT_EPSILON);
			Assert::AreEqual(6.0f, m1(1, 2), FLT_EPSILON);
			Assert::AreEqual(9.0f, m1(2, 2), FLT_EPSILON);
		}

		TEST_METHOD(OpFloatMultMat3)
		{
			Matrix3 m(
				1.00000000f, 2.00000000f, 3.00000000f, 
				4.00000000f, 5.00000000f, 6.00000000f,
				7.00000000f, 8.00000000f, 9.00000000f);

			Matrix3 res = 2.30000000f * m;

			Assert::AreEqual(2.3f, res.m00, FLT_EPSILON);
			Assert::AreEqual(4.6f, res.m01, FLT_EPSILON);
			Assert::AreEqual(6.8999997f, res.m02, FLT_EPSILON);
			Assert::AreEqual(9.2f, res.m10, FLT_EPSILON);
			Assert::AreEqual(11.5f, res.m11, FLT_EPSILON);
			Assert::AreEqual(13.799999f, res.m12, FLT_EPSILON);
			Assert::AreEqual(16.0999998f, res.m20, FLT_EPSILON);
			Assert::AreEqual(18.3999999f, res.m21, FLT_EPSILON);
			Assert::AreEqual(20.699999f, res.m22, FLT_EPSILON);
		}
	};
}