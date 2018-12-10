namespace aw
{
	namespace sb
	{
		inline Type Value::GetType() const
		{
			return ShaderBuilder::GetBuilder().getTypeId(id);
		}

		inline Value Value::ExtractFloat(VectorComponent comp) const
		{
			return ShaderBuilder::GetBuilder().createCompositeExtract(id, Type::MakeFloat(), uint32(comp));
		}

		inline Value Value::ExtractVector2f() const
		{
			return ShaderBuilder::GetBuilder().createCompositeConstruct(
				Type::MakeVector2f(),
				{ ExtractFloat(VectorComponent::X), ExtractFloat(VectorComponent::Y) }
			);
		}

		inline Value Value::ExtractVector3f() const
		{
			return ShaderBuilder::GetBuilder().createCompositeConstruct(
				Type::MakeVector3f(),
				{ ExtractFloat(VectorComponent::X), ExtractFloat(VectorComponent::Y), ExtractFloat(VectorComponent::Z) }
			);
		}

		inline Value Value::Shuffle(Value rhs, uint32 a, uint32 b, uint32 c, uint32 d) const
		{
			return ShaderBuilder::GetBuilder().createOp(spv::OpVectorShuffle, Type::MakeVector4f(), { id, rhs, a, b, c, d });
		}

		template <typename T>
		inline Value Value::ToVector4f(T t1, T t2, T t3) const
		{
			if (GetType().IsFloat() == true)
			{
				return ShaderBuilder::GetBuilder().createCompositeConstruct(
					Type::MakeVector4f(),
					{ id, MakeConstant(t1, true), MakeConstant(t2, true), MakeConstant(t3, true) }
				);
			}
			else
				THROW("Value is not of type Float32");

			return Value();
		}

		template <typename T>
		inline Value Value::ToVector4f(T t1, T t2) const
		{
			if (GetType().IsVector() == true && GetType().GetNumComponents() == 2)
			{
				return ShaderBuilder::GetBuilder().createCompositeConstruct(
					Type::MakeVector4f(),
					{ ExtractFloat(VectorComponent::X), ExtractFloat(VectorComponent::Y), MakeConstant(t1, true), MakeConstant(t2, true) }
				);
			}
			else
				THROW("Value is not of type Vector2f");

			return Value();
		}

		template <typename T>
		inline Value Value::ToVector4f(T t) const
		{
			if (GetType().IsVector() == true && GetType().GetNumComponents() == 3)
			{
				return ShaderBuilder::GetBuilder().createCompositeConstruct(
					Type::MakeVector4f(),
					{ ExtractFloat(VectorComponent::X), ExtractFloat(VectorComponent::Y), ExtractFloat(VectorComponent::Z), MakeConstant(t, true) }
				);
			}
			else
				THROW("Value is not of type Vector3f");

			return Value();
		}

		//inline Value Value::ToMatrix3()
		//{
		//	if (GetType().IsMatrix() == true && GetType().GetNumComponents() == 4)
		//	{
		//		return ShaderBuilder::GetBuilder().createCompositeConstruct(
		//			Type::MakeMatrix3f(),
		//			{ ExtractFloat(VectorComponent::X), ExtractFloat(VectorComponent::Y), ExtractFloat(VectorComponent::Z), MakeConstant(t, true) }
		//		);
		//	}
		//	else
		//		THROW("Value is not of type Matrix4");
		//}

		inline Value Value::X() const
		{
			return ExtractFloat(VectorComponent::X);
		}

		inline Value Value::Y() const
		{
			return ExtractFloat(VectorComponent::Y);
		}

		inline Value Value::Z() const
		{
			return ExtractFloat(VectorComponent::Z);
		}

		inline Value Value::W() const
		{
			return ExtractFloat(VectorComponent::W);
		}

		inline Value Value::XY() const
		{
			return ShaderBuilder::GetBuilder().createOp(spv::OpVectorShuffle, Type::MakeVector2f(), { id, id, 0, 1 });
		}

		inline Value Value::XYZ() const
		{
			return ShaderBuilder::GetBuilder().createOp(spv::OpVectorShuffle, Type::MakeVector3f(), { id, id, 0, 1, 2 });
		}

		inline Value Value::Normalize() const
		{
			return ShaderBuilder::GetBuilder().createTriOp(spv::OpExtInst, GetType(), ShaderBuilder::GetGlsl450Import(), GLSLstd450Normalize, id);
		}

		inline Value Value::Dot(Value rhs) const
		{
			return ShaderBuilder::GetBuilder().createBinOp(spv::OpDot, Type::MakeFloat(), id, rhs);
		}

		inline Value Value::Cross(Value rhs) const
		{
			return ShaderBuilder::GetBuilder().createOp(spv::OpExtInst, GetType(), { ShaderBuilder::ShaderBuilder::GetGlsl450Import(), GLSLstd450Cross, id, rhs });
		}

		inline Value Value::Distance(Value rhs) const
		{
			return ShaderBuilder::GetBuilder().createOp(spv::OpExtInst, Type::MakeFloat(), { ShaderBuilder::ShaderBuilder::GetGlsl450Import(), GLSLstd450Distance, id, rhs });
		}

		inline Value Value::Length() const
		{
			return ShaderBuilder::GetBuilder().createOp(spv::OpExtInst, Type::MakeFloat(), { ShaderBuilder::ShaderBuilder::GetGlsl450Import(), GLSLstd450Length, id });
		}

		inline Value Value::Abs() const
		{
			return ShaderBuilder::GetBuilder().createTriOp(spv::OpExtInst, GetType(), ShaderBuilder::ShaderBuilder::GetGlsl450Import(), GLSLstd450FAbs, id);
		}

		inline Value Value::Transpose() const
		{
			return ShaderBuilder::GetBuilder().createUnaryOp(spv::OpTranspose, GetType(), id);
		}

		inline Value Value::Inverse() const
		{
			return ShaderBuilder::GetBuilder().createTriOp(spv::OpExtInst, GetType(), ShaderBuilder::ShaderBuilder::GetGlsl450Import(), GLSLstd450MatrixInverse, id);
		}

		inline Value Value::Negate() const
		{
			return ShaderBuilder::GetBuilder().createUnaryOp(spv::OpFNegate, GetType(), id);
		}

		inline Value Value::Reflect(Value incident, Value normal)
		{
			ASSERT(incident.GetType() != normal.GetType(), "incident and normal must be of the same type");
			return ShaderBuilder::GetBuilder().createOp(spv::OpExtInst, incident.GetType(), { ShaderBuilder::GetGlsl450Import(), GLSLstd450Reflect, incident, normal });
		}

		inline Value Value::Refract(Value incident, Value normal, Value refractionCoef)
		{
			ASSERT(incident.GetType() != normal.GetType(), "incident and normal must be of the same type");
			return ShaderBuilder::GetBuilder().createOp(spv::OpExtInst, incident.GetType(), { ShaderBuilder::GetGlsl450Import(), GLSLstd450Refract, incident, normal, refractionCoef });
		}

		inline Value Value::Pow(Value base, Value exp)
		{
			ASSERT(base.GetType() != exp.GetType(), "x and y must have the same type");
			return ShaderBuilder::GetBuilder().createOp(spv::OpExtInst, base.GetType(), { ShaderBuilder::GetGlsl450Import(), GLSLstd450Pow, base, exp });
		}

		inline Value Value::Sqrt(Value x)
		{
			return ShaderBuilder::GetBuilder().createOp(spv::OpExtInst, x.GetType(), { ShaderBuilder::GetGlsl450Import(), GLSLstd450Sqrt, x });
		}

		inline Value Value::Max(Value x, Value y)
		{
			return ShaderBuilder::GetBuilder().createOp(spv::OpExtInst, x.GetType(), { ShaderBuilder::GetGlsl450Import(), GLSLstd450FMax, x, y });
		}

		inline Value Value::Min(Value x, Value y)
		{
			return ShaderBuilder::GetBuilder().createOp(spv::OpExtInst, x.GetType(), { ShaderBuilder::GetGlsl450Import(), GLSLstd450FMin, x, y });
		}

		inline Value Value::Mix(Value x, Value y, Value a)
		{
			ASSERT(x.GetType() != y.GetType(), "x and y must have the same type");
			ASSERT(x.GetType() != a.GetType(), "x and a must have the same type");
			return ShaderBuilder::GetBuilder().createOp(spv::OpExtInst, x.GetType(), { ShaderBuilder::GetGlsl450Import(), GLSLstd450FMix, x, y, a });
		}

		inline Value Value::SmoothStep(Value x, Value y, Value a)
		{
			ASSERT(x.GetType() != y.GetType(), "x and y must have the same type");
			ASSERT(x.GetType() != a.GetType(), "x and a must have the same type");
			return ShaderBuilder::GetBuilder().createOp(spv::OpExtInst, x.GetType(), { ShaderBuilder::GetGlsl450Import(), GLSLstd450SmoothStep, x, y, a });
		}

		inline Value Value::Select(Value condition, Value t, Value f)
		{
			ASSERT(t.GetType() != f.GetType(), "t and f must have the same type");
			return ShaderBuilder::GetBuilder().createTriOp(spv::OpSelect, t.GetType(), condition, t, f);
		}

		inline Value Value::ImageSampleImplicitLod(Value texCoordinates, Type type) const
		{
			return ShaderBuilder::GetBuilder().createBinOp(spv::OpImageSampleImplicitLod, type, id, texCoordinates);
		}

		inline Value Value::MakeVectorf(uint32 numComp, Value a)
		{
			Type f = Type::MakeFloat();

			ASSERT(a.GetType() != f, "Value::MakeVector2f(Value a) a must be of type Float");

			std::vector<uint32> constituents(numComp);
			for (uint64 i{ 0 }; i < constituents.size(); i++)
				constituents[i] = a.id;

			return ShaderBuilder::GetBuilder().createCompositeConstruct(Type::MakeVector(f, numComp), constituents);
		}

		inline Value Value::MakeVector2f(Value a)
		{
			Type f = Type::MakeFloat();

			ASSERT(a.GetType() != f, "Value::MakeVector2f(Value a) a must be of type Float");

			return ShaderBuilder::GetBuilder().createCompositeConstruct(Type::MakeVector(f, 2), { a, a });
		}

		inline Value Value::MakeVector2f(Value a, Value b)
		{
			Type f = Type::MakeFloat();

			ASSERT(a.GetType() != f, "Value::MakeVector2f(Value a, Value b) a must be of type Float");
			ASSERT(b.GetType() != f, "Value::MakeVector2f(Value a, Value b) b must be of type Float");

			return ShaderBuilder::GetBuilder().createCompositeConstruct(Type::MakeVector(f, 2), { a, b });
		}

		inline Value Value::MakeVector3f(Value a)
		{
			Type f = Type::MakeFloat();

			ASSERT(a.GetType() != f, "Value::MakeVector3f(Value a) a must be of type Float");

			return ShaderBuilder::GetBuilder().createCompositeConstruct(Type::MakeVector(f, 3), { a, a, a });
		}

		inline Value Value::MakeVector3f(Value a, Value b, Value c)
		{
			Type f = Type::MakeFloat();

			ASSERT(a.GetType() != f, "Value::MakeVector3f(Value a, Value b, Value c) a must be of type Float");
			ASSERT(b.GetType() != f, "Value::MakeVector3f(Value a, Value b, Value c) b must be of type Float");
			ASSERT(c.GetType() != f, "Value::MakeVector3f(Value a, Value b, Value c) c must be of type Float");

			return ShaderBuilder::GetBuilder().createCompositeConstruct(Type::MakeVector(f, 3), { a, b, c });
		}

		inline Value Value::MakeVector4f(Value a)
		{
			Type f = Type::MakeFloat();

			ASSERT(a.GetType() != f, "Value::MakeVector4f(Value a) a must be of type Float");

			return ShaderBuilder::GetBuilder().createCompositeConstruct(Type::MakeVector(f, 4), { a, a, a, a });
		}

		inline Value Value::MakeVector4f(Value a, Value b, Value c, Value d)
		{
			Type f = Type::MakeFloat();

			ASSERT(a.GetType() != f, "Value::MakeVector4f(Value a, Value b, Value c, Value d) a must be of type Float");
			ASSERT(b.GetType() != f, "Value::MakeVector4f(Value a, Value b, Value c, Value d) b must be of type Float");
			ASSERT(c.GetType() != f, "Value::MakeVector4f(Value a, Value b, Value c, Value d) c must be of type Float");
			ASSERT(d.GetType() != f, "Value::MakeVector4f(Value a, Value b, Value c, Value d) d must be of type Float");

			return ShaderBuilder::GetBuilder().createCompositeConstruct(Type::MakeVector(f, 4), { a, b, c, d });
		}

		inline Value Value::MakeMatrix3f(Value a, Value b, Value c)
		{
			Type vec3f = Type::MakeVector3f();

			ASSERT(a.GetType() != vec3f, "Value::MakeMatrix3f(Value a, Value b, Value c) a must be of type Vector3f");
			ASSERT(b.GetType() != vec3f, "Value::MakeMatrix3f(Value a, Value b, Value c) b must be of type Vector3f");
			ASSERT(c.GetType() != vec3f, "Value::MakeMatrix3f(Value a, Value b, Value c) c must be of type Vector3f");

			return ShaderBuilder::GetBuilder().createCompositeConstruct(Type::MakeMatrix3f(), { a, b, c });
		}

		inline Value Value::MakeComposite(Type type, Value value)
		{
			std::vector<spv::Id> values;

			if (type.IsVector() == true)
			{
				uint64 numComp = uint64(type.GetNumComponents());
				values.resize(numComp);
				for (uint64 i = 0; i < numComp; i++)
					values[i] = value;
			}
			else
				THROW("Type not handled yet");

			return ShaderBuilder::GetBuilder().createCompositeConstruct(type, values);
		}

		inline Value Value::MakeComposite(vk::ArrayProxy<const Value> values)
		{
			if (values.size() == 2)
			{
				ASSERT(values.data()[0].GetType() != values.data()[1].GetType(), "Trying to make a composite with different types");

				if (values.data()[0].GetType().IsFloat() == true)
					return MakeVector2f(values.data()[0], values.data()[1]);
			}
			else if (values.size() == 3)
			{
				ASSERT((values.data()[0].GetType() != values.data()[1].GetType()) || (values.data()[0].GetType() != values.data()[2].GetType()), "Trying to make a composite with different types");

				if (values.data()[0].GetType().IsVector3f() == true)
					return MakeMatrix3f(values.data()[0], values.data()[1], values.data()[2]);
				else if (values.data()[0].GetType().IsFloat() == true)
					return MakeVector3f(values.data()[0], values.data()[1], values.data()[2]);
			}
			else if (values.size() == 4)
			{
				ASSERT((values.data()[0].GetType() != values.data()[1].GetType()) || (values.data()[0].GetType() != values.data()[2].GetType()) || (values.data()[0].GetType() != values.data()[3].GetType()), "Trying to make a composite with different types");

				if (values.data()[0].GetType().IsFloat() == true)
					return MakeVector4f(values.data()[0], values.data()[1], values.data()[2], values.data()[3]);
			}

			THROW("MakeComposite not implemented yet with these types");

			return Value();
		}

		inline Value Value::MakeConstant(bool b, bool specConstant)
		{
			return ShaderBuilder::GetBuilder().makeBoolConstant(b, specConstant);
		}

		inline Value Value::MakeConstant(int32 i, bool specConstant)
		{
			return ShaderBuilder::GetBuilder().makeIntConstant(i, specConstant);
		}

		inline Value Value::MakeConstant(uint32 i, bool specConstant)
		{
			return ShaderBuilder::GetBuilder().makeUintConstant(i, specConstant);
		}

		inline Value Value::MakeConstant(int64 i, bool specConstant)
		{
			return ShaderBuilder::GetBuilder().makeInt64Constant(i, specConstant);
		}

		inline Value Value::MakeConstant(uint64 i, bool specConstant)
		{
			return ShaderBuilder::GetBuilder().makeUint64Constant(i, specConstant);
		}

		inline Value Value::MakeConstant(float f, bool specConstant)
		{
			return ShaderBuilder::GetBuilder().makeFloatConstant(f, specConstant);
		}

		inline Value Value::MakeConstant(double d, bool specConstant)
		{
			return ShaderBuilder::GetBuilder().makeDoubleConstant(d, specConstant);
		}

		inline Value Value::MakeConstant(Vector2 v, bool specConstant)
		{
			return ShaderBuilder::GetBuilder().makeCompositeConstant(Type::MakeVector2f(), { MakeConstant(v.x), MakeConstant(v.y) }, specConstant);
		}

		inline Value Value::MakeConstant(Vector3 v, bool specConstant)
		{
			return ShaderBuilder::GetBuilder().makeCompositeConstant(Type::MakeVector3f(), { MakeConstant(v.x), MakeConstant(v.y), MakeConstant(v.z) }, specConstant);
		}

		inline Value Value::MakeConstant(const std::array<float, 4>& v, bool specConstant)
		{
			return ShaderBuilder::GetBuilder().makeCompositeConstant(Type::MakeVector4f(), { MakeConstant(v[0]), MakeConstant(v[1]), MakeConstant(v[2]), MakeConstant(v[3]) }, specConstant);
		}

		inline Value Value::operator-() const
		{
			return Negate();
		}

		inline Value Value::operator[](uint64 index) const
		{
			return ExtractFloat(VectorComponent(index));
		}

		inline Value Value::operator+(Value rhs) const
		{
			using namespace spv;

			Type type = GetType();
			Type rhsType = rhs.GetType();

			if (type == rhsType)
			{
				if (type.IsFloat() == true || type.IsVectorf() == true)
					return ShaderBuilder::GetBuilder().createBinOp(OpFAdd, type, id, rhs);
				else if (type.IsInt() == true || type.IsUint() == true)
					return ShaderBuilder::GetBuilder().createBinOp(OpIAdd, type, id, rhs);

				THROW("Type can't use this operator (or not implemented yet... \"Value::operator+(Value rhs)\")");
			}
			else if (type.IsFloat() == true && rhsType.IsVectorf() == true)
			{
				int32 numComp = rhsType.GetNumComponents();

				return ShaderBuilder::GetBuilder().createBinOp(OpFAdd, rhsType, Value::MakeVectorf(numComp, id), rhs);
			}
			else if (type.IsVectorf() == true && rhsType.IsFloat() == true)
			{
				int32 numComp = type.GetNumComponents();

				return ShaderBuilder::GetBuilder().createBinOp(OpFAdd, type, id, Value::MakeVectorf(numComp, rhs));
			}

			THROW("Value::operator+(Value rhs) Types must be equals");

			return Value();
		}

		inline Value Value::operator-(Value rhs) const
		{
			using namespace spv;

			Type type = GetType();
			Type rhsType = rhs.GetType();

			if (type == rhsType)
			{
				if (type.IsFloat() == true || type.IsVectorf() == true)
					return ShaderBuilder::GetBuilder().createBinOp(OpFSub, type, id, rhs);

				THROW("Type can't use this operator (or not implemented yet... \"Value::operator-(Value rhs)\")");
			}
			else if (type.IsFloat() == true && rhsType.IsVectorf() == true)
			{
				int32 numComp = rhsType.GetNumComponents();

				return ShaderBuilder::GetBuilder().createBinOp(OpFSub, rhsType, Value::MakeVectorf(numComp, id), rhs);
			}
			else if (type.IsVectorf() == true && rhsType.IsFloat() == true)
			{
				int32 numComp = type.GetNumComponents();

				return ShaderBuilder::GetBuilder().createBinOp(OpFSub, type, id, Value::MakeVectorf(numComp, rhs));
			}

			THROW("Value::operator-(Value rhs) Types must be equals or compatibles");

			return Value();
		}

		inline Value Value::operator*(Value rhs) const
		{
			using namespace spv;

			Type type = GetType();
			Type rhsType = rhs.GetType();

			if (type == rhsType)
			{
				if (type.IsFloat() == true || type.IsVectorf() == true)
					return ShaderBuilder::GetBuilder().createBinOp(OpFMul, type, id, rhs);

				THROW("Type can't use this operator (or not implemented yet... \"Value::operator*(Value rhs)\")");
			}
			else if (type.IsMatrix() == true && rhsType.IsVector() == true)
				return ShaderBuilder::GetBuilder().createBinOp(OpMatrixTimesVector, rhsType, id, rhs);
			else if (type.IsVector() == true && rhsType.IsScalar() == true)
				return ShaderBuilder::GetBuilder().createBinOp(OpVectorTimesScalar, type, id, rhs);
			else if (type.IsScalar() == true && rhsType.IsVector() == true)
				return ShaderBuilder::GetBuilder().createBinOp(OpVectorTimesScalar, rhsType, rhs, id);

			THROW("Value::operator*(Value rhs) not implemented yet");

			return Value();
		}

		inline Value Value::operator/(Value rhs) const
		{
			using namespace spv;

			Type type = GetType();
			Type rhsType = rhs.GetType();

			if (type == rhsType)
			{
				if (type.IsFloat() == true || type.IsVectorf() == true)
					return ShaderBuilder::GetBuilder().createBinOp(OpFDiv, type, id, rhs);
				else if (type.IsInt() == true)// || type.IsVectorI() == true)
					return ShaderBuilder::GetBuilder().createBinOp(OpSDiv, type, id, rhs);
				else if (type.IsUint() == true)// || type.IsVectorI() == true)
					return ShaderBuilder::GetBuilder().createBinOp(OpUDiv, type, id, rhs);

				THROW("Type can't use this operator (or not implemented yet... \"Value::operator*(Value rhs)\")");
			}
			else if (type.IsVector() == true && rhsType.IsFloat() == true)
			{
				Value inv = 1.0_c / rhs;

				return ShaderBuilder::GetBuilder().createBinOp(OpVectorTimesScalar, type, id, inv);
			}
			//else if (type.IsScalar() == true && rhsType.IsVector() == true)
			//{
			//	return ShaderBuilder::GetBuilder().createBinOp(OpVectorTimesScalar, rhsType, rhs, id);
			//}

			THROW("Value::operator/(Value rhs) not implemented yet");

			return Value();
		}

		inline Value Value::operator<(Value rhs) const
		{
			if (rhs.GetType().GetScalarType().IsFloat())
				return ShaderBuilder::GetBuilder().createBinOp(spv::OpFOrdLessThan, Type::MakeBool(), id, rhs.id);
			else if (rhs.GetType().GetScalarType().IsInt())
				return ShaderBuilder::GetBuilder().createBinOp(spv::OpSLessThan, Type::MakeBool(), id, rhs.id);
			else if (rhs.GetType().GetScalarType().IsUint())
				return ShaderBuilder::GetBuilder().createBinOp(spv::OpULessThan, Type::MakeBool(), id, rhs.id);
			else
				THROW("Value::operator<(Value rhs) not implemented yet");

			return Value();
		}

		inline Value Value::operator>(Value rhs) const
		{
			if (rhs.GetType().GetScalarType().IsFloat())
				return ShaderBuilder::GetBuilder().createBinOp(spv::OpFOrdGreaterThan, Type::MakeBool(), id, rhs.id);
			else if (rhs.GetType().GetScalarType().IsInt())
				return ShaderBuilder::GetBuilder().createBinOp(spv::OpSGreaterThan, Type::MakeBool(), id, rhs.id);
			else if (rhs.GetType().GetScalarType().IsUint())
				return ShaderBuilder::GetBuilder().createBinOp(spv::OpUGreaterThan, Type::MakeBool(), id, rhs.id);
			else
				THROW("Value::operator>(Value rhs) not implemented yet");

			return Value();
		}

		inline Value Value::operator<=(Value rhs) const
		{
			if (rhs.GetType().GetScalarType().IsFloat())
				return ShaderBuilder::GetBuilder().createBinOp(spv::OpFOrdLessThanEqual, Type::MakeBool(), id, rhs.id);
			else if (rhs.GetType().GetScalarType().IsInt())
				return ShaderBuilder::GetBuilder().createBinOp(spv::OpSLessThanEqual, Type::MakeBool(), id, rhs.id);
			else if (rhs.GetType().GetScalarType().IsUint())
				return ShaderBuilder::GetBuilder().createBinOp(spv::OpULessThanEqual, Type::MakeBool(), id, rhs.id);
			else
				THROW("Value::operator<=(Value rhs) not implemented yet");

			return Value();
		}

		inline Value Value::operator>=(Value rhs) const
		{
			if (rhs.GetType().GetScalarType().IsFloat())
				return ShaderBuilder::GetBuilder().createBinOp(spv::OpFOrdLessThanEqual, Type::MakeBool(), id, rhs.id);
			else if (rhs.GetType().GetScalarType().IsInt())
				return ShaderBuilder::GetBuilder().createBinOp(spv::OpSLessThanEqual, Type::MakeBool(), id, rhs.id);
			else if (rhs.GetType().GetScalarType().IsUint())
				return ShaderBuilder::GetBuilder().createBinOp(spv::OpULessThanEqual, Type::MakeBool(), id, rhs.id);
			else
				THROW("Value::operator>=(Value rhs) not implemented yet");

			return Value();
		}
	}
}