namespace aw
{
	namespace sb
	{
		bool Type::IsBool() const
		{
			return ShaderBuilder::GetBuilder().isBoolType(id);
		}

		bool Type::IsInt() const
		{
			return ShaderBuilder::GetBuilder().isIntType(id);
		}

		bool Type::IsUint() const
		{
			return ShaderBuilder::GetBuilder().isUintType(id);
		}

		bool Type::IsFloat() const
		{
			return ShaderBuilder::GetBuilder().isFloatType(id);
		}

		bool Type::IsPointer() const
		{
			return ShaderBuilder::GetBuilder().isPointerType(id);
		}

		bool Type::IsScalar() const
		{
			return ShaderBuilder::GetBuilder().isScalarType(id);
		}

		bool Type::IsVector() const
		{
			return ShaderBuilder::GetBuilder().isVectorType(id);
		}

		bool Type::IsVectorf() const
		{
			return IsVector() && GetScalarType().IsFloat();
		}

		bool Type::IsVector2f() const
		{
			return IsVectorf() && (GetNumComponents() == 2);
		}

		bool Type::IsVector3f() const
		{
			return IsVectorf() && (GetNumComponents() == 3);
		}

		bool Type::IsVector4f() const
		{
			return IsVectorf() && (GetNumComponents() == 4);
		}

		bool Type::IsMatrix() const
		{
			return ShaderBuilder::GetBuilder().isMatrixType(id);
		}

		bool Type::IsStruct() const
		{
			return ShaderBuilder::GetBuilder().isStructType(id);
		}

		bool Type::IsArray() const
		{
			return ShaderBuilder::GetBuilder().isArrayType(id);
		}

		bool Type::IsAggregate() const
		{
			return ShaderBuilder::GetBuilder().isAggregateType(id);
		}

		bool Type::IsImage() const
		{
			return ShaderBuilder::GetBuilder().isImageType(id);
		}

		bool Type::IsSampler() const
		{
			return ShaderBuilder::GetBuilder().isSamplerType(id);
		}

		bool Type::IsSampledImage() const
		{
			return ShaderBuilder::GetBuilder().isSampledImageType(id);
		}

		inline int32 Type::GetNumComponents() const
		{
			return ShaderBuilder::GetBuilder().getNumTypeComponents(id);
		}

		inline Type Type::GetScalarType() const
		{
			return ShaderBuilder::GetBuilder().getScalarTypeId(id);
		}

		inline Type Type::MakeVoid()
		{
			return ShaderBuilder::GetBuilder().makeVoidType();
		}

		inline Type Type::MakeBool()
		{
			return ShaderBuilder::GetBuilder().makeBoolType();
		}

		inline Type Type::MakeInteger(uint32 width, bool hasSign)
		{
			return ShaderBuilder::GetBuilder().makeIntegerType(width, hasSign);
		}

		inline Type Type::MakeInt()
		{
			return MakeInteger(32, false);
		}

		inline Type Type::MakeUint()
		{
			//return MakeInteger(32, true);
			return ShaderBuilder::GetBuilder().makeUintType(32);
		}

		inline Type Type::MakeInt64()
		{
			return MakeInteger(64, false);
		}

		inline Type Type::MakeUint64()
		{
			//return MakeInteger(64, true);
			return ShaderBuilder::GetBuilder().makeUintType(64);
		}

		inline Type Type::MakeFloat()
		{
			return ShaderBuilder::GetBuilder().makeFloatType(32);
		}

		inline Type Type::MakeDouble()
		{
			return ShaderBuilder::GetBuilder().makeFloatType(64);
		}

		inline Type Type::MakeVector(Type type, uint32 size)
		{
			return ShaderBuilder::GetBuilder().makeVectorType(type, size);
		}

		inline Type Type::MakeVectorf(uint32 size)
		{
			return MakeVector(MakeFloat(), size);
		}

		inline Type Type::MakeVector2f()
		{
			return MakeVectorf(2);
		}

		inline Type Type::MakeVector3f()
		{
			return MakeVectorf(3);
		}

		inline Type Type::MakeVector4f()
		{
			return MakeVectorf(4);
		}

		inline Type Type::MakeMatrix(Type type, uint32 cols, uint32 rows)
		{
			return ShaderBuilder::GetBuilder().makeMatrixType(type, cols, rows);
		}

		inline Type Type::MakeMatrixf(uint32 cols, uint32 rows)
		{
			return MakeMatrix(MakeFloat(), cols, rows);
		}

		inline Type Type::MakeMatrix2f()
		{
			return MakeMatrixf(2, 2);
		}

		inline Type Type::MakeMatrix3f()
		{
			return MakeMatrixf(3, 3);
		}

		inline Type Type::MakeMatrix4f()
		{
			return MakeMatrixf(4, 4);
		}

		inline Type Type::MakeImage(Type type, spv::Dim dim, bool depth, bool arrayed, bool ms, uint32 sampled, spv::ImageFormat format)
		{
			return ShaderBuilder::GetBuilder().makeImageType(type, dim, depth, arrayed, ms, sampled, format);
		}

		inline Type Type::MakeSampledImage(Type type)
		{
			return ShaderBuilder::GetBuilder().makeSampledImageType(type);
		}

		inline Type Type::MakeStruct(const std::vector<Type>& types, const char* name)
		{
			static_assert(sizeof(Type) == sizeof(spv::Id), "Type's size must be equal to std::Id's size");
			return ShaderBuilder::GetBuilder().makeStructType(reinterpret_cast<const std::vector<spv::Id>&>(types), name);
		}

		inline Type Type::MakePointer(Type type, spv::StorageClass storageClass)
		{
			return ShaderBuilder::GetBuilder().makePointer(storageClass, type);
		}
	}
}