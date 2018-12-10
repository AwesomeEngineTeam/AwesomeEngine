#include "CppUnitTest.h"

#include <Awesome/Maths/Matrix4.hpp>
#include <Awesome/Maths/Vector3.hpp>
#include <Awesome/Maths/Quaternion.hpp>
#include <Awesome/Maths/Matrix3.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace aw;

namespace MathUnitTest
{
	TEST_CLASS(Matrix4_Test)
	{
	public:

		TEST_METHOD(DefaultConstructor)
		{
			Matrix4 m;

			Assert::AreEqual(0.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m30, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m31, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m22, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m32, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m03, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m13, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m23, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m33, FLT_EPSILON);
		}

		TEST_METHOD(ValueConstructor)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Assert::AreEqual(1.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(9.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(13.0f, m.m30, FLT_EPSILON);
			Assert::AreEqual(2.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(10.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(14.0f, m.m31, FLT_EPSILON);
			Assert::AreEqual(3.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(7.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(11.0f, m.m22, FLT_EPSILON);
			Assert::AreEqual(15.0f, m.m32, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m03, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m13, FLT_EPSILON);
			Assert::AreEqual(12.0f, m.m23, FLT_EPSILON);
			Assert::AreEqual(16.0f, m.m33, FLT_EPSILON);
		}

		TEST_METHOD(CopyConstructor)
		{
			Matrix4 m1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 m2 = m1;

			Assert::AreEqual(1.0f, m2.m00, FLT_EPSILON);
			Assert::AreEqual(5.0f, m2.m10, FLT_EPSILON);
			Assert::AreEqual(9.0f, m2.m20, FLT_EPSILON);
			Assert::AreEqual(13.0f, m2.m30, FLT_EPSILON);
			Assert::AreEqual(2.0f, m2.m01, FLT_EPSILON);
			Assert::AreEqual(6.0f, m2.m11, FLT_EPSILON);
			Assert::AreEqual(10.0f, m2.m21, FLT_EPSILON);
			Assert::AreEqual(14.0f, m2.m31, FLT_EPSILON);
			Assert::AreEqual(3.0f, m2.m02, FLT_EPSILON);
			Assert::AreEqual(7.0f, m2.m12, FLT_EPSILON);
			Assert::AreEqual(11.0f, m2.m22, FLT_EPSILON);
			Assert::AreEqual(15.0f, m2.m32, FLT_EPSILON);
			Assert::AreEqual(4.0f, m2.m03, FLT_EPSILON);
			Assert::AreEqual(8.0f, m2.m13, FLT_EPSILON);
			Assert::AreEqual(12.0f, m2.m23, FLT_EPSILON);
			Assert::AreEqual(16.0f, m2.m33, FLT_EPSILON);
		}

		TEST_METHOD(CopyMat3Constructor)
		 {
			 Matrix3 m1(
				 1.0f, 2.0f, 3.0f,
				 4.0f, 5.0f, 6.0f,
				 7.0f, 8.0f, 9.0f
			 );

			 Matrix4 m2(m1);

			 Assert::AreEqual(1.0f, m2.m00, FLT_EPSILON);
			 Assert::AreEqual(4.0f, m2.m10, FLT_EPSILON);
			 Assert::AreEqual(7.0f, m2.m20, FLT_EPSILON);
			 Assert::AreEqual(0.0f, m2.m30, FLT_EPSILON);
			 Assert::AreEqual(2.0f, m2.m01, FLT_EPSILON);
			 Assert::AreEqual(5.0f, m2.m11, FLT_EPSILON);
			 Assert::AreEqual(8.0f, m2.m21, FLT_EPSILON);
			 Assert::AreEqual(0.0f, m2.m31, FLT_EPSILON);
			 Assert::AreEqual(3.0f, m2.m02, FLT_EPSILON);
			 Assert::AreEqual(6.0f, m2.m12, FLT_EPSILON);
			 Assert::AreEqual(9.0f, m2.m22, FLT_EPSILON);
			 Assert::AreEqual(0.0f, m2.m32, FLT_EPSILON);
			 Assert::AreEqual(0.0f, m2.m03, FLT_EPSILON);
			 Assert::AreEqual(0.0f, m2.m13, FLT_EPSILON);
			 Assert::AreEqual(0.0f, m2.m23, FLT_EPSILON);
			 Assert::AreEqual(1.0f, m2.m33, FLT_EPSILON);
		 }

		TEST_METHOD(RValueCopyConstructor)
		{
			Matrix4 m1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 m2 = std::move(m1);

			Assert::AreEqual(1.0f, m2.m00, FLT_EPSILON);
			Assert::AreEqual(5.0f, m2.m10, FLT_EPSILON);
			Assert::AreEqual(9.0f, m2.m20, FLT_EPSILON);
			Assert::AreEqual(13.0f, m2.m30, FLT_EPSILON);
			Assert::AreEqual(2.0f, m2.m01, FLT_EPSILON);
			Assert::AreEqual(6.0f, m2.m11, FLT_EPSILON);
			Assert::AreEqual(10.0f, m2.m21, FLT_EPSILON);
			Assert::AreEqual(14.0f, m2.m31, FLT_EPSILON);
			Assert::AreEqual(3.0f, m2.m02, FLT_EPSILON);
			Assert::AreEqual(7.0f, m2.m12, FLT_EPSILON);
			Assert::AreEqual(11.0f, m2.m22, FLT_EPSILON);
			Assert::AreEqual(15.0f, m2.m32, FLT_EPSILON);
			Assert::AreEqual(4.0f, m2.m03, FLT_EPSILON);
			Assert::AreEqual(8.0f, m2.m13, FLT_EPSILON);
			Assert::AreEqual(12.0f, m2.m23, FLT_EPSILON);
			Assert::AreEqual(16.0f, m2.m33, FLT_EPSILON);
		}

		TEST_METHOD(GetTrace)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Assert::AreEqual(34.0f, m.Trace(), FLT_EPSILON);
		}

		TEST_METHOD(GetDeterminant)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Assert::AreEqual(0.0f, m.Determinant(), FLT_EPSILON);

			m.m22 = 4.0f;
			m.m31 = 3.0f;

			Assert::AreEqual(924.0f, m.Determinant(), FLT_EPSILON);

			m.m03 = m.m13 = m.m23 = m.m30 = m.m31 = m.m32 = 0.0f;
			m.m33 = 1.0f;

			Assert::AreEqual(28.0f, m.Determinant(), FLT_EPSILON);
		}

		TEST_METHOD(GetInversed)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 inv = m.GetInversed(); // Not inversible

			Assert::AreEqual(1.0f, inv.m00, FLT_EPSILON);
			Assert::AreEqual(5.0f, inv.m10, FLT_EPSILON);
			Assert::AreEqual(9.0f, inv.m20, FLT_EPSILON);
			Assert::AreEqual(13.0f, inv.m30, FLT_EPSILON);
			Assert::AreEqual(2.0f, inv.m01, FLT_EPSILON);
			Assert::AreEqual(6.0f, inv.m11, FLT_EPSILON);
			Assert::AreEqual(10.0f, inv.m21, FLT_EPSILON);
			Assert::AreEqual(14.0f, inv.m31, FLT_EPSILON);
			Assert::AreEqual(3.0f, inv.m02, FLT_EPSILON);
			Assert::AreEqual(7.0f, inv.m12, FLT_EPSILON);
			Assert::AreEqual(11.0f, inv.m22, FLT_EPSILON);
			Assert::AreEqual(15.0f, inv.m32, FLT_EPSILON);
			Assert::AreEqual(4.0f, inv.m03, FLT_EPSILON);
			Assert::AreEqual(8.0f, inv.m13, FLT_EPSILON);
			Assert::AreEqual(12.0f, inv.m23, FLT_EPSILON);
			Assert::AreEqual(16.0f, inv.m33, FLT_EPSILON);

			m.m22 = 4.0f;
			m.m31 = 3.0f;
			inv = m.GetInversed(); // Inversible

			Assert::AreEqual(-0.4978354f, inv.m00, FLT_EPSILON);
			Assert::AreEqual(-0.1818181f, inv.m10, FLT_EPSILON);
			Assert::AreEqual(-0.1428571f, inv.m20, FLT_EPSILON);
			Assert::AreEqual(0.5725108f, inv.m30, FLT_EPSILON);
			Assert::AreEqual(0.0562770f, inv.m01, FLT_EPSILON);
			Assert::AreEqual(0.2727272f, inv.m11, FLT_EPSILON);
			Assert::AreEqual(0.2857142f, inv.m21, FLT_EPSILON);
			Assert::AreEqual(-0.3647186f, inv.m31, FLT_EPSILON);
			Assert::AreEqual(0.0476190f, inv.m02, FLT_EPSILON);
			Assert::AreEqual(0.0f, inv.m12, FLT_EPSILON);
			Assert::AreEqual(-0.1428571f, inv.m22, FLT_EPSILON);
			Assert::AreEqual(0.0952380f, inv.m32, FLT_EPSILON);
			Assert::AreEqual(0.0606060f, inv.m03, FLT_EPSILON);
			Assert::AreEqual(-0.0909090f, inv.m13, FLT_EPSILON);
			Assert::AreEqual(0.0f, inv.m23, FLT_EPSILON);
			Assert::AreEqual(0.0303030f, inv.m33, FLT_EPSILON);

			//===============================

			Matrix4 m2(
				-0.88888888f, 0.44444444f, 0.11111111f, 0.0f,
				0.44444444f, 0.77777777f, 0.44444444f, 0.0f,
				0.11111111f, 0.44444444f, -0.88888888f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);

			inv = m2.GetInversed(); // Orthogonal

			Assert::AreEqual(m2.m00, inv.m00, FLT_EPSILON);
			Assert::AreEqual(m2.m10, inv.m01, FLT_EPSILON);
			Assert::AreEqual(m2.m20, inv.m02, FLT_EPSILON);
			Assert::AreEqual(m2.m30, inv.m03, FLT_EPSILON);
			Assert::AreEqual(m2.m01, inv.m10, FLT_EPSILON);
			Assert::AreEqual(m2.m11, inv.m11, FLT_EPSILON);
			Assert::AreEqual(m2.m21, inv.m12, FLT_EPSILON);
			Assert::AreEqual(m2.m31, inv.m13, FLT_EPSILON);
			Assert::AreEqual(m2.m02, inv.m20, FLT_EPSILON);
			Assert::AreEqual(m2.m12, inv.m21, FLT_EPSILON);
			Assert::AreEqual(m2.m22, inv.m22, FLT_EPSILON);
			Assert::AreEqual(m2.m32, inv.m32, FLT_EPSILON);
			Assert::AreEqual(m2.m03, inv.m30, FLT_EPSILON);
			Assert::AreEqual(m2.m13, inv.m31, FLT_EPSILON);
			Assert::AreEqual(m2.m23, inv.m32, FLT_EPSILON);
			Assert::AreEqual(m2.m33, inv.m33, FLT_EPSILON);
		}

		TEST_METHOD(GetTransposed)
		{
			const Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 tr = m.GetTransposed();

			Assert::AreEqual(1.0f, tr.m00, FLT_EPSILON);
			Assert::AreEqual(5.0f, tr.m01, FLT_EPSILON);
			Assert::AreEqual(9.0f, tr.m02, FLT_EPSILON);
			Assert::AreEqual(13.0f, tr.m03, FLT_EPSILON);
			Assert::AreEqual(2.0f, tr.m10, FLT_EPSILON);
			Assert::AreEqual(6.0f, tr.m11, FLT_EPSILON);
			Assert::AreEqual(10.0f, tr.m12, FLT_EPSILON);
			Assert::AreEqual(14.0f, tr.m13, FLT_EPSILON);
			Assert::AreEqual(3.0f, tr.m20, FLT_EPSILON);
			Assert::AreEqual(7.0f, tr.m21, FLT_EPSILON);
			Assert::AreEqual(11.0f, tr.m22, FLT_EPSILON);
			Assert::AreEqual(15.0f, tr.m23, FLT_EPSILON);
			Assert::AreEqual(4.0f, tr.m30, FLT_EPSILON);
			Assert::AreEqual(8.0f, tr.m31, FLT_EPSILON);
			Assert::AreEqual(12.0f, tr.m32, FLT_EPSILON);
			Assert::AreEqual(16.0f, tr.m33, FLT_EPSILON);
		}

		TEST_METHOD(Inverse)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			m.Inverse(); // Not inversible

			Assert::AreEqual(1.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(9.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(13.0f, m.m30, FLT_EPSILON);
			Assert::AreEqual(2.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(10.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(14.0f, m.m31, FLT_EPSILON);
			Assert::AreEqual(3.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(7.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(11.0f, m.m22, FLT_EPSILON);
			Assert::AreEqual(15.0f, m.m32, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m03, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m13, FLT_EPSILON);
			Assert::AreEqual(12.0f, m.m23, FLT_EPSILON);
			Assert::AreEqual(16.0f, m.m33, FLT_EPSILON);

			m = Matrix4(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 4.0f, 12.0f,
				13.0f, 3.0f, 15.0f, 16.0f
			);

			m.Inverse(); // Inversible

			Assert::AreEqual(-0.4978354f, m.m00, FLT_EPSILON);
			Assert::AreEqual(-0.1818181f, m.m10, FLT_EPSILON);
			Assert::AreEqual(-0.1428571f, m.m20, FLT_EPSILON);
			Assert::AreEqual(0.5725108f, m.m30, FLT_EPSILON);
			Assert::AreEqual(0.0562770f, m.m01, FLT_EPSILON);
			Assert::AreEqual(0.2727272f, m.m11, FLT_EPSILON);
			Assert::AreEqual(0.2857142f, m.m21, FLT_EPSILON);
			Assert::AreEqual(-0.3647186f, m.m31, FLT_EPSILON);
			Assert::AreEqual(0.0476190f, m.m02, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(-0.1428571f, m.m22, FLT_EPSILON);
			Assert::AreEqual(0.0952380f, m.m32, FLT_EPSILON);
			Assert::AreEqual(0.0606060f, m.m03, FLT_EPSILON);
			Assert::AreEqual(-0.0909090f, m.m13, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m23, FLT_EPSILON);
			Assert::AreEqual(0.0303030f, m.m33, FLT_EPSILON);

			//===============================

			Matrix4 m2(
				-0.88888888f, 0.44444444f, 0.11111111f, 0.0f,
				0.44444444f, 0.77777777f, 0.44444444f, 0.0f,
				0.11111111f, 0.44444444f, -0.88888888f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);

			m2.Inverse(); // Orthogonal

			Assert::AreEqual(m2.m00, -0.8888888f, FLT_EPSILON);
			Assert::AreEqual(m2.m10, 0.4444444f, FLT_EPSILON);
			Assert::AreEqual(m2.m20, 0.1111111f, FLT_EPSILON);
			Assert::AreEqual(m2.m30, 0.0f, FLT_EPSILON);
			Assert::AreEqual(m2.m01, 0.4444444f, FLT_EPSILON);
			Assert::AreEqual(m2.m11, 0.7777777f, FLT_EPSILON);
			Assert::AreEqual(m2.m21, 0.4444444f, FLT_EPSILON);
			Assert::AreEqual(m2.m31, 0.0f, FLT_EPSILON);
			Assert::AreEqual(m2.m02, 0.1111111f, FLT_EPSILON);
			Assert::AreEqual(m2.m12, 0.4444444f, FLT_EPSILON);
			Assert::AreEqual(m2.m22, -0.8888888f, FLT_EPSILON);
			Assert::AreEqual(m2.m32, 0.0f, FLT_EPSILON);
			Assert::AreEqual(m2.m03, 0.0f, FLT_EPSILON);
			Assert::AreEqual(m2.m13, 0.0f, FLT_EPSILON);
			Assert::AreEqual(m2.m23, 0.0f, FLT_EPSILON);
			Assert::AreEqual(m2.m33, 1.0f, FLT_EPSILON);
		}

		TEST_METHOD(Transpose)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			m.Transpose();

			Assert::AreEqual(1.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(9.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(13.0f, m.m03, FLT_EPSILON);
			Assert::AreEqual(2.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(10.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(14.0f, m.m13, FLT_EPSILON);
			Assert::AreEqual(3.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(7.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(11.0f, m.m22, FLT_EPSILON);
			Assert::AreEqual(15.0f, m.m23, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m30, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m31, FLT_EPSILON);
			Assert::AreEqual(12.0f, m.m32, FLT_EPSILON);
			Assert::AreEqual(16.0f, m.m33, FLT_EPSILON);
		}

		TEST_METHOD(IsOrthogonal)
		{
			Matrix4 m1(
				-0.88888888f, 0.44444444f, 0.11111111f, 0.0f,
				0.44444444f, 0.77777777f, 0.44444444f, 0.0f,
				0.11111111f, 0.44444444f, -0.88888888f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);

			Assert::AreEqual(true, m1.IsOrthogonal());

			Matrix4 m2(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Assert::AreEqual(false, m2.IsOrthogonal());
		}

		TEST_METHOD(IsHomogenous)
		{
			Matrix4 m1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Assert::AreEqual(false, m1.IsHomogenous());

			Matrix4 m2(
				1.0f, 2.0f, 3.0f, 0.0f,
				5.0f, 6.0f, 7.0f, 0.0f,
				9.0f, 10.0f, 11.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);

			Assert::AreEqual(true, m2.IsHomogenous());
		}

		TEST_METHOD(FromColumns)
		{
			Vector3 col1(1.0f, 2.0f, 3.0f);
			Vector3 col2(4.0f, 5.0f, 6.0f);
			Vector3 col3(7.0f, 8.0f, 9.0f);

			Matrix4 m = Matrix4::FromColumns(col1, col2, col3);

			Assert::AreEqual(1.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(2.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(3.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m30, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m31, FLT_EPSILON);
			Assert::AreEqual(7.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(9.0f, m.m22, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m32, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m03, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m13, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m23, FLT_EPSILON);
			Assert::AreEqual(1.0f, m.m33, FLT_EPSILON);
		}

		TEST_METHOD(FromRows)
		{
			Vector3 row1(1.0f, 4.0f, 7.0f);
			Vector3 row2(2.0f, 5.0f, 8.0f);
			Vector3 row3(3.0f, 6.0f, 9.0f);

			Matrix4 m = Matrix4::FromRows(row1, row2, row3);

			Assert::AreEqual(1.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(2.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(3.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m30, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m31, FLT_EPSILON);
			Assert::AreEqual(7.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(9.0f, m.m22, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m32, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m03, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m13, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m23, FLT_EPSILON);
			Assert::AreEqual(1.0f, m.m33, FLT_EPSILON);
		}

		TEST_METHOD(MakeTranslation)
		{
			Matrix4 m = Matrix4::MakeTranslation(Vector3(2.0f, 5.0f, 6.0f));

			Assert::AreEqual(1.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m30, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(1.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m31, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(1.0f, m.m22, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m32, FLT_EPSILON);
			Assert::AreEqual(2.0f, m.m03, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m13, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m23, FLT_EPSILON);
			Assert::AreEqual(1.0f, m.m33, FLT_EPSILON);
		}

		TEST_METHOD(MakeRotation)
		{
			Quaternion q(M_PI2, Vector3(0, 1, 0));

			Matrix4 m = Matrix4::MakeRotation(q);

			Assert::AreEqual(0.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(-1.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m30, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(1.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m31, FLT_EPSILON);
			Assert::AreEqual(1.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m22, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m32, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m03, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m13, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m23, FLT_EPSILON);
			Assert::AreEqual(1.0f, m.m33, FLT_EPSILON);
		}

		TEST_METHOD(MakeScale)
		{
			Matrix4 m = Matrix4::MakeScale(Vector3(2.0f, 5.0f, 6.0f));

			Assert::AreEqual(2.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m30, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m31, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m22, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m32, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m03, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m13, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m23, FLT_EPSILON);
			Assert::AreEqual(1.0f, m.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpPlus)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 res = +m;

			Assert::AreEqual(1.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(9.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(13.0f, m.m30, FLT_EPSILON);
			Assert::AreEqual(2.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(10.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(14.0f, m.m31, FLT_EPSILON);
			Assert::AreEqual(3.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(7.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(11.0f, m.m22, FLT_EPSILON);
			Assert::AreEqual(15.0f, m.m32, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m03, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m13, FLT_EPSILON);
			Assert::AreEqual(12.0f, m.m23, FLT_EPSILON);
			Assert::AreEqual(16.0f, m.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpMinus)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 res = -m;

			Assert::AreEqual(-1.0f, res.m00, FLT_EPSILON);
			Assert::AreEqual(-5.0f, res.m10, FLT_EPSILON);
			Assert::AreEqual(-9.0f, res.m20, FLT_EPSILON);
			Assert::AreEqual(-13.0f, res.m30, FLT_EPSILON);
			Assert::AreEqual(-2.0f, res.m01, FLT_EPSILON);
			Assert::AreEqual(-6.0f, res.m11, FLT_EPSILON);
			Assert::AreEqual(-10.0f, res.m21, FLT_EPSILON);
			Assert::AreEqual(-14.0f, res.m31, FLT_EPSILON);
			Assert::AreEqual(-3.0f, res.m02, FLT_EPSILON);
			Assert::AreEqual(-7.0f, res.m12, FLT_EPSILON);
			Assert::AreEqual(-11.0f, res.m22, FLT_EPSILON);
			Assert::AreEqual(-15.0f, res.m32, FLT_EPSILON);
			Assert::AreEqual(-4.0f, res.m03, FLT_EPSILON);
			Assert::AreEqual(-8.0f, res.m13, FLT_EPSILON);
			Assert::AreEqual(-12.0f, res.m23, FLT_EPSILON);
			Assert::AreEqual(-16.0f, res.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpMat4MultFloat)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 res = m * 2.0f;

			Assert::AreEqual(2.0f, res.m00, FLT_EPSILON);
			Assert::AreEqual(10.0f, res.m10, FLT_EPSILON);
			Assert::AreEqual(18.0f, res.m20, FLT_EPSILON);
			Assert::AreEqual(26.0f, res.m30, FLT_EPSILON);
			Assert::AreEqual(4.0f, res.m01, FLT_EPSILON);
			Assert::AreEqual(12.0f, res.m11, FLT_EPSILON);
			Assert::AreEqual(20.0f, res.m21, FLT_EPSILON);
			Assert::AreEqual(28.0f, res.m31, FLT_EPSILON);
			Assert::AreEqual(6.0f, res.m02, FLT_EPSILON);
			Assert::AreEqual(14.0f, res.m12, FLT_EPSILON);
			Assert::AreEqual(22.0f, res.m22, FLT_EPSILON);
			Assert::AreEqual(30.0f, res.m32, FLT_EPSILON);
			Assert::AreEqual(8.0f, res.m03, FLT_EPSILON);
			Assert::AreEqual(16.0f, res.m13, FLT_EPSILON);
			Assert::AreEqual(24.0f, res.m23, FLT_EPSILON);
			Assert::AreEqual(32.0f, res.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpMat4DivFloat)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 res = m / 2.0f;

			Assert::AreEqual(0.5f, res.m00, FLT_EPSILON);
			Assert::AreEqual(2.5f, res.m10, FLT_EPSILON);
			Assert::AreEqual(4.5f, res.m20, FLT_EPSILON);
			Assert::AreEqual(6.5f, res.m30, FLT_EPSILON);
			Assert::AreEqual(1.0f, res.m01, FLT_EPSILON);
			Assert::AreEqual(3.0f, res.m11, FLT_EPSILON);
			Assert::AreEqual(5.0f, res.m21, FLT_EPSILON);
			Assert::AreEqual(7.0f, res.m31, FLT_EPSILON);
			Assert::AreEqual(1.5f, res.m02, FLT_EPSILON);
			Assert::AreEqual(3.5f, res.m12, FLT_EPSILON);
			Assert::AreEqual(5.5f, res.m22, FLT_EPSILON);
			Assert::AreEqual(7.5f, res.m32, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.m03, FLT_EPSILON);
			Assert::AreEqual(4.0f, res.m13, FLT_EPSILON);
			Assert::AreEqual(6.0f, res.m23, FLT_EPSILON);
			Assert::AreEqual(8.0f, res.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpMat4MultVec3)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Vector3 v(2.0f, 3.0f, 4.0f);

			Vector3 res = m * v;

			Assert::AreEqual(20.0f, res.x, FLT_EPSILON);
			Assert::AreEqual(56.0f, res.y, FLT_EPSILON);
			Assert::AreEqual(92.0f, res.z, FLT_EPSILON);
		}

		TEST_METHOD(OpMat4PlusMat4)
		{
			Matrix4 m1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 m2(
				1.0f, 0.0f, 3.0f, 1.0f,
				5.0f, 6.0f, 1.0f, 7.0f,
				3.0f, 6.0f, 2.0f, 12.0f,
				9.0f, 14.0f, 1.0f, 7.0f
			);

			Matrix4 res = m1 + m2;

			Assert::AreEqual(2.0f, res.m00, FLT_EPSILON);
			Assert::AreEqual(10.0f, res.m10, FLT_EPSILON);
			Assert::AreEqual(12.0f, res.m20, FLT_EPSILON);
			Assert::AreEqual(22.0f, res.m30, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.m01, FLT_EPSILON);
			Assert::AreEqual(12.0f, res.m11, FLT_EPSILON);
			Assert::AreEqual(16.0f, res.m21, FLT_EPSILON);
			Assert::AreEqual(28.0f, res.m31, FLT_EPSILON);
			Assert::AreEqual(6.0f, res.m02, FLT_EPSILON);
			Assert::AreEqual(8.0f, res.m12, FLT_EPSILON);
			Assert::AreEqual(13.0f, res.m22, FLT_EPSILON);
			Assert::AreEqual(16.0f, res.m32, FLT_EPSILON);
			Assert::AreEqual(5.0f, res.m03, FLT_EPSILON);
			Assert::AreEqual(15.0f, res.m13, FLT_EPSILON);
			Assert::AreEqual(24.0f, res.m23, FLT_EPSILON);
			Assert::AreEqual(23.0f, res.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpMat4MinusMat4)
		{
			Matrix4 m1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, -8.0f,
				9.0f, 10.0f, 11.0f, 10.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 m2(
				1.0f, 0.0f, 1.0f, 1.0f,
				5.0f, 6.0f, 1.0f, 7.0f,
				3.0f, 6.0f, 2.0f, 12.0f,
				9.0f, 14.0f, 1.0f, 7.0f
			);

			Matrix4 res = m1 - m2;

			Assert::AreEqual(0.0f, res.m00, FLT_EPSILON);
			Assert::AreEqual(0.0f, res.m10, FLT_EPSILON);
			Assert::AreEqual(6.0f, res.m20, FLT_EPSILON);
			Assert::AreEqual(4.0f, res.m30, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.m01, FLT_EPSILON);
			Assert::AreEqual(0.0f, res.m11, FLT_EPSILON);
			Assert::AreEqual(4.0f, res.m21, FLT_EPSILON);
			Assert::AreEqual(0.0f, res.m31, FLT_EPSILON);
			Assert::AreEqual(2.0f, res.m02, FLT_EPSILON);
			Assert::AreEqual(6.0f, res.m12, FLT_EPSILON);
			Assert::AreEqual(9.0f, res.m22, FLT_EPSILON);
			Assert::AreEqual(14.0f, res.m32, FLT_EPSILON);
			Assert::AreEqual(3.0f, res.m03, FLT_EPSILON);
			Assert::AreEqual(-15.0f, res.m13, FLT_EPSILON);
			Assert::AreEqual(-2.0f, res.m23, FLT_EPSILON);
			Assert::AreEqual(9.0f, res.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpMat4MultMat4)
		{
			Matrix4 m1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, -8.0f,
				9.0f, 10.0f, 11.0f, 10.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 m2(
				1.0f, 0.0f, 1.0f, 1.0f,
				5.0f, 6.0f, 1.0f, 7.0f,
				3.0f, 6.0f, 2.0f, 12.0f,
				9.0f, 14.0f, 1.0f, 7.0f
			);

			Matrix4 res = m1 * m2;

			Assert::AreEqual(56.0f, res.m00, FLT_EPSILON);
			Assert::AreEqual(-16.0f, res.m10, FLT_EPSILON);
			Assert::AreEqual(182.0f, res.m20, FLT_EPSILON);
			Assert::AreEqual(272.0f, res.m30, FLT_EPSILON);
			Assert::AreEqual(86.0f, res.m01, FLT_EPSILON);
			Assert::AreEqual(-34.0f, res.m11, FLT_EPSILON);
			Assert::AreEqual(266.0f, res.m21, FLT_EPSILON);
			Assert::AreEqual(398.0f, res.m31, FLT_EPSILON);
			Assert::AreEqual(13.0f, res.m02, FLT_EPSILON);
			Assert::AreEqual(17.0f, res.m12, FLT_EPSILON);
			Assert::AreEqual(51.0f, res.m22, FLT_EPSILON);
			Assert::AreEqual(73.0f, res.m32, FLT_EPSILON);
			Assert::AreEqual(79.0f, res.m03, FLT_EPSILON);
			Assert::AreEqual(75.0f, res.m13, FLT_EPSILON);
			Assert::AreEqual(281.0f, res.m23, FLT_EPSILON);
			Assert::AreEqual(403.0f, res.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpEquals)
		{
			Matrix4 m1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, -8.0f,
				9.0f, 10.0f, 11.0f, 10.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 m2;
			m2 = m1;

			Assert::AreEqual(1.0f, m2.m00, FLT_EPSILON);
			Assert::AreEqual(5.0f, m2.m10, FLT_EPSILON);
			Assert::AreEqual(9.0f, m2.m20, FLT_EPSILON);
			Assert::AreEqual(13.0f, m2.m30, FLT_EPSILON);
			Assert::AreEqual(2.0f, m2.m01, FLT_EPSILON);
			Assert::AreEqual(6.0f, m2.m11, FLT_EPSILON);
			Assert::AreEqual(10.0f, m2.m21, FLT_EPSILON);
			Assert::AreEqual(14.0f, m2.m31, FLT_EPSILON);
			Assert::AreEqual(3.0f, m2.m02, FLT_EPSILON);
			Assert::AreEqual(7.0f, m2.m12, FLT_EPSILON);
			Assert::AreEqual(11.0f, m2.m22, FLT_EPSILON);
			Assert::AreEqual(15.0f, m2.m32, FLT_EPSILON);
			Assert::AreEqual(4.0f, m2.m03, FLT_EPSILON);
			Assert::AreEqual(-8.0f, m2.m13, FLT_EPSILON);
			Assert::AreEqual(10.0f, m2.m23, FLT_EPSILON);
			Assert::AreEqual(16.0f, m2.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpEqualsRValue)
		{
			Matrix4 m1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, -8.0f,
				9.0f, 10.0f, 11.0f, 10.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 m2;
			m2 = std::move(m1);

			Assert::AreEqual(1.0f, m2.m00, FLT_EPSILON);
			Assert::AreEqual(5.0f, m2.m10, FLT_EPSILON);
			Assert::AreEqual(9.0f, m2.m20, FLT_EPSILON);
			Assert::AreEqual(13.0f, m2.m30, FLT_EPSILON);
			Assert::AreEqual(2.0f, m2.m01, FLT_EPSILON);
			Assert::AreEqual(6.0f, m2.m11, FLT_EPSILON);
			Assert::AreEqual(10.0f, m2.m21, FLT_EPSILON);
			Assert::AreEqual(14.0f, m2.m31, FLT_EPSILON);
			Assert::AreEqual(3.0f, m2.m02, FLT_EPSILON);
			Assert::AreEqual(7.0f, m2.m12, FLT_EPSILON);
			Assert::AreEqual(11.0f, m2.m22, FLT_EPSILON);
			Assert::AreEqual(15.0f, m2.m32, FLT_EPSILON);
			Assert::AreEqual(4.0f, m2.m03, FLT_EPSILON);
			Assert::AreEqual(-8.0f, m2.m13, FLT_EPSILON);
			Assert::AreEqual(10.0f, m2.m23, FLT_EPSILON);
			Assert::AreEqual(16.0f, m2.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpMat4MultEqualsFloat)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			m *= 2.0f;

			Assert::AreEqual(2.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(10.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(18.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(26.0f, m.m30, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(12.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(20.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(28.0f, m.m31, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(14.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(22.0f, m.m22, FLT_EPSILON);
			Assert::AreEqual(30.0f, m.m32, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m03, FLT_EPSILON);
			Assert::AreEqual(16.0f, m.m13, FLT_EPSILON);
			Assert::AreEqual(24.0f, m.m23, FLT_EPSILON);
			Assert::AreEqual(32.0f, m.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpMat4DivEqualsFloat)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			m /= 2.0f;

			Assert::AreEqual(0.5f, m.m00, FLT_EPSILON);
			Assert::AreEqual(2.5f, m.m10, FLT_EPSILON);
			Assert::AreEqual(4.5f, m.m20, FLT_EPSILON);
			Assert::AreEqual(6.5f, m.m30, FLT_EPSILON);
			Assert::AreEqual(1.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(3.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(7.0f, m.m31, FLT_EPSILON);
			Assert::AreEqual(1.5f, m.m02, FLT_EPSILON);
			Assert::AreEqual(3.5f, m.m12, FLT_EPSILON);
			Assert::AreEqual(5.5f, m.m22, FLT_EPSILON);
			Assert::AreEqual(7.5f, m.m32, FLT_EPSILON);
			Assert::AreEqual(2.0f, m.m03, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m13, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m23, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpMat4PlusEqualsMat4)
		{
			Matrix4 m1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 m2(
				1.0f, 0.0f, 3.0f, 1.0f,
				5.0f, 6.0f, 1.0f, 7.0f,
				3.0f, 6.0f, 2.0f, 12.0f,
				9.0f, 14.0f, 1.0f, 7.0f
			);

			m1 += m2;

			Assert::AreEqual(2.0f, m1.m00, FLT_EPSILON);
			Assert::AreEqual(10.0f, m1.m10, FLT_EPSILON);
			Assert::AreEqual(12.0f, m1.m20, FLT_EPSILON);
			Assert::AreEqual(22.0f, m1.m30, FLT_EPSILON);
			Assert::AreEqual(2.0f, m1.m01, FLT_EPSILON);
			Assert::AreEqual(12.0f, m1.m11, FLT_EPSILON);
			Assert::AreEqual(16.0f, m1.m21, FLT_EPSILON);
			Assert::AreEqual(28.0f, m1.m31, FLT_EPSILON);
			Assert::AreEqual(6.0f, m1.m02, FLT_EPSILON);
			Assert::AreEqual(8.0f, m1.m12, FLT_EPSILON);
			Assert::AreEqual(13.0f, m1.m22, FLT_EPSILON);
			Assert::AreEqual(16.0f, m1.m32, FLT_EPSILON);
			Assert::AreEqual(5.0f, m1.m03, FLT_EPSILON);
			Assert::AreEqual(15.0f, m1.m13, FLT_EPSILON);
			Assert::AreEqual(24.0f, m1.m23, FLT_EPSILON);
			Assert::AreEqual(23.0f, m1.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpMat4MinusEqualsMat4)
		{
			Matrix4 m1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, -8.0f,
				9.0f, 10.0f, 11.0f, 10.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 m2(
				1.0f, 0.0f, 1.0f, 1.0f,
				5.0f, 6.0f, 1.0f, 7.0f,
				3.0f, 6.0f, 2.0f, 12.0f,
				9.0f, 14.0f, 1.0f, 7.0f
			);

			m1 -= m2;

			Assert::AreEqual(0.0f, m1.m00, FLT_EPSILON);
			Assert::AreEqual(0.0f, m1.m10, FLT_EPSILON);
			Assert::AreEqual(6.0f, m1.m20, FLT_EPSILON);
			Assert::AreEqual(4.0f, m1.m30, FLT_EPSILON);
			Assert::AreEqual(2.0f, m1.m01, FLT_EPSILON);
			Assert::AreEqual(0.0f, m1.m11, FLT_EPSILON);
			Assert::AreEqual(4.0f, m1.m21, FLT_EPSILON);
			Assert::AreEqual(0.0f, m1.m31, FLT_EPSILON);
			Assert::AreEqual(2.0f, m1.m02, FLT_EPSILON);
			Assert::AreEqual(6.0f, m1.m12, FLT_EPSILON);
			Assert::AreEqual(9.0f, m1.m22, FLT_EPSILON);
			Assert::AreEqual(14.0f, m1.m32, FLT_EPSILON);
			Assert::AreEqual(3.0f, m1.m03, FLT_EPSILON);
			Assert::AreEqual(-15.0f, m1.m13, FLT_EPSILON);
			Assert::AreEqual(-2.0f, m1.m23, FLT_EPSILON);
			Assert::AreEqual(9.0f, m1.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpMat4MultEqualsMat4)
		{
			Matrix4 m1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, -8.0f,
				9.0f, 10.0f, 11.0f, 10.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 m2(
				1.0f, 0.0f, 1.0f, 1.0f,
				5.0f, 6.0f, 1.0f, 7.0f,
				3.0f, 6.0f, 2.0f, 12.0f,
				9.0f, 14.0f, 1.0f, 7.0f
			);

			m1 *= m2;

			Assert::AreEqual(56.0f, m1.m00, FLT_EPSILON);
			Assert::AreEqual(-16.0f, m1.m10, FLT_EPSILON);
			Assert::AreEqual(182.0f, m1.m20, FLT_EPSILON);
			Assert::AreEqual(272.0f, m1.m30, FLT_EPSILON);
			Assert::AreEqual(86.0f, m1.m01, FLT_EPSILON);
			Assert::AreEqual(-34.0f, m1.m11, FLT_EPSILON);
			Assert::AreEqual(266.0f, m1.m21, FLT_EPSILON);
			Assert::AreEqual(398.0f, m1.m31, FLT_EPSILON);
			Assert::AreEqual(13.0f, m1.m02, FLT_EPSILON);
			Assert::AreEqual(17.0f, m1.m12, FLT_EPSILON);
			Assert::AreEqual(51.0f, m1.m22, FLT_EPSILON);
			Assert::AreEqual(73.0f, m1.m32, FLT_EPSILON);
			Assert::AreEqual(79.0f, m1.m03, FLT_EPSILON);
			Assert::AreEqual(75.0f, m1.m13, FLT_EPSILON);
			Assert::AreEqual(281.0f, m1.m23, FLT_EPSILON);
			Assert::AreEqual(403.0f, m1.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpComparator)
		{
			Matrix4 m1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);
			Matrix4 m2;

			Assert::AreEqual(false, m1 == m2);

			m2 = m1;

			Assert::AreEqual(true, m1 == m2);
		}

		TEST_METHOD(OpComparatorDiff)
		{
			Matrix4 m1(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);
			Matrix4 m2;

			Assert::AreEqual(true, m1 != m2);

			m2 = m1;

			Assert::AreEqual(false, m1 != m2);
		}

		TEST_METHOD(OpAccess)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			m[0] = 5.0f;
			m[1] = 8.0f;
			m[2] = 12.0f;
			m[3] = -3.0f;
			m[4] = 0.0f;
			m[5] = 4.0f;
			m[6] = 5.0f;
			m[7] = 1.0f;
			m[8] = -2.0f;
			m[9] = 8.0f;
			m[10] = 4.0f;
			m[11] = 1.0f;
			m[12] = 0.0f;
			m[13] = 6.0f;
			m[14] = -5.0f;
			m[15] = 3.0f;

			Assert::AreEqual(5.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(12.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(-3.0f, m.m30, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(1.0f, m.m31, FLT_EPSILON);
			Assert::AreEqual(-2.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m22, FLT_EPSILON);
			Assert::AreEqual(1.0f, m.m32, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m03, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m13, FLT_EPSILON);
			Assert::AreEqual(-5.0f, m.m23, FLT_EPSILON);
			Assert::AreEqual(3.0f, m.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpConstAccess)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Assert::AreEqual(1.0f, m[0], FLT_EPSILON);
			Assert::AreEqual(5.0f, m[1], FLT_EPSILON);
			Assert::AreEqual(9.0f, m[2], FLT_EPSILON);
			Assert::AreEqual(13.0f, m[3], FLT_EPSILON);
			Assert::AreEqual(2.0f, m[4], FLT_EPSILON);
			Assert::AreEqual(6.0f, m[5], FLT_EPSILON);
			Assert::AreEqual(10.0f, m[6], FLT_EPSILON);
			Assert::AreEqual(14.0f, m[7], FLT_EPSILON);
			Assert::AreEqual(3.0f, m[8], FLT_EPSILON);
			Assert::AreEqual(7.0f, m[9], FLT_EPSILON);
			Assert::AreEqual(11.0f, m[10], FLT_EPSILON);
			Assert::AreEqual(15.0f, m[11], FLT_EPSILON);
			Assert::AreEqual(4.0f, m[12], FLT_EPSILON);
			Assert::AreEqual(8.0f, m[13], FLT_EPSILON);
			Assert::AreEqual(12.0f, m[14], FLT_EPSILON);
			Assert::AreEqual(16.0f, m[15], FLT_EPSILON);
		}

		TEST_METHOD(OpAccess2)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			m(0,0) = 5.0f;
			m(1,0) = 8.0f;
			m(2,0) = 12.0f;
			m(3,0) = -3.0f;
			m(0,1) = 0.0f;
			m(1,1) = 4.0f;
			m(2,1) = 5.0f;
			m(3,1) = 1.0f;
			m(0,2) = -2.0f;
			m(1,2) = 8.0f;
			m(2,2) = 4.0f;
			m(3,2) = 1.0f;
			m(0,3) = 0.0f;
			m(1,3) = 6.0f;
			m(2,3) = -5.0f;
			m(3,3) = 3.0f;

			Assert::AreEqual(5.0f, m.m00, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m10, FLT_EPSILON);
			Assert::AreEqual(12.0f, m.m20, FLT_EPSILON);
			Assert::AreEqual(-3.0f, m.m30, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m01, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m11, FLT_EPSILON);
			Assert::AreEqual(5.0f, m.m21, FLT_EPSILON);
			Assert::AreEqual(1.0f, m.m31, FLT_EPSILON);
			Assert::AreEqual(-2.0f, m.m02, FLT_EPSILON);
			Assert::AreEqual(8.0f, m.m12, FLT_EPSILON);
			Assert::AreEqual(4.0f, m.m22, FLT_EPSILON);
			Assert::AreEqual(1.0f, m.m32, FLT_EPSILON);
			Assert::AreEqual(0.0f, m.m03, FLT_EPSILON);
			Assert::AreEqual(6.0f, m.m13, FLT_EPSILON);
			Assert::AreEqual(-5.0f, m.m23, FLT_EPSILON);
			Assert::AreEqual(3.0f, m.m33, FLT_EPSILON);
		}

		TEST_METHOD(OpConstAccess2)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Assert::AreEqual(1.0f, m(0,0), FLT_EPSILON);
			Assert::AreEqual(5.0f, m(1,0), FLT_EPSILON);
			Assert::AreEqual(9.0f, m(2,0), FLT_EPSILON);
			Assert::AreEqual(13.0f, m(3,0), FLT_EPSILON);
			Assert::AreEqual(2.0f, m(0,1), FLT_EPSILON);
			Assert::AreEqual(6.0f, m(1,1), FLT_EPSILON);
			Assert::AreEqual(10.0f, m(2,1), FLT_EPSILON);
			Assert::AreEqual(14.0f, m(3,1), FLT_EPSILON);
			Assert::AreEqual(3.0f, m(0,2), FLT_EPSILON);
			Assert::AreEqual(7.0f, m(1,2), FLT_EPSILON);
			Assert::AreEqual(11.0f, m(2,2), FLT_EPSILON);
			Assert::AreEqual(15.0f, m(3,2), FLT_EPSILON);
			Assert::AreEqual(4.0f, m(0,3), FLT_EPSILON);
			Assert::AreEqual(8.0f, m(1,3), FLT_EPSILON);
			Assert::AreEqual(12.0f, m(2,3), FLT_EPSILON);
			Assert::AreEqual(16.0f, m(3,3), FLT_EPSILON);
		}

		TEST_METHOD(OpFloatMultMat4)
		{
			Matrix4 m(
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f
			);

			Matrix4 res = 2.0f * m;

			Assert::AreEqual(2.0f, res.m00, FLT_EPSILON);
			Assert::AreEqual(10.0f, res.m10, FLT_EPSILON);
			Assert::AreEqual(18.0f, res.m20, FLT_EPSILON);
			Assert::AreEqual(26.0f, res.m30, FLT_EPSILON);
			Assert::AreEqual(4.0f, res.m01, FLT_EPSILON);
			Assert::AreEqual(12.0f, res.m11, FLT_EPSILON);
			Assert::AreEqual(20.0f, res.m21, FLT_EPSILON);
			Assert::AreEqual(28.0f, res.m31, FLT_EPSILON);
			Assert::AreEqual(6.0f, res.m02, FLT_EPSILON);
			Assert::AreEqual(14.0f, res.m12, FLT_EPSILON);
			Assert::AreEqual(22.0f, res.m22, FLT_EPSILON);
			Assert::AreEqual(30.0f, res.m32, FLT_EPSILON);
			Assert::AreEqual(8.0f, res.m03, FLT_EPSILON);
			Assert::AreEqual(16.0f, res.m13, FLT_EPSILON);
			Assert::AreEqual(24.0f, res.m23, FLT_EPSILON);
			Assert::AreEqual(32.0f, res.m33, FLT_EPSILON);
		}
	};
}