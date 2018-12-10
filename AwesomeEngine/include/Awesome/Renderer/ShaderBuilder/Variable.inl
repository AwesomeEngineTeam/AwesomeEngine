namespace aw
{
	namespace sb
	{
		inline Variable::Variable(const Variable& other)
		{
			operator=(other);
		}

		inline Variable::Variable(Variable&& other) noexcept :
			IdWrapper(other.id),
			value{ other.value },
			storageClass{ other.storageClass }
		{
		}

		inline Variable::Variable(spv::StorageClass _storageClass, Type type, const char* name) :
			storageClass(_storageClass)
		{
			id = ShaderBuilder::GetBuilder().createVariable(storageClass, type, name);
		}

		inline Variable::Variable(spv::Id id, spv::StorageClass _storageClass) :
			IdWrapper(id),
			storageClass{ _storageClass }
		{
		}

		void Variable::Store(const Value& _value)
		{
			ShaderBuilder::GetBuilder().createStore(_value, id);
		}

		inline Value Variable::Load() const
		{
			return ShaderBuilder::GetBuilder().createLoad(id);
		}

		inline Value Variable::GetValue()
		{
			if (value.id == 0)
				value = Load();

			return value;
		}

		inline Variable Variable::MakeAccessChain(const std::vector<Value>& offsets)
		{
			std::vector<spv::Id> offsetIds(offsets.size());

			for (uint64 i = 0; i < offsets.size(); i++)
				offsetIds[i] = offsets[i];

			return {
				ShaderBuilder::GetBuilder().createAccessChain(storageClass, id, offsetIds),
				storageClass
			};
		}

		inline Variable Variable::CreateUniform(Type type, int32 binding, int32 set, const char* name)
		{
			Variable var(spv::StorageClassUniform, type, name);

			var.AddDecoration(spv::DecorationBinding, binding);
			var.AddDecoration(spv::DecorationDescriptorSet, set);

			return var;
		}

		inline Variable Variable::CreateUniform(Type type, const char* name)
		{
			return Variable(spv::StorageClassUniform, type, name);
		}

		inline Variable Variable::CreateUniformConstant(Type type, int32 binding, int32 set, const char* name)
		{
			Variable var(spv::StorageClassUniformConstant, type, name);

			var.AddDecoration(spv::DecorationBinding, binding);
			var.AddDecoration(spv::DecorationDescriptorSet, set);

			return var;
		}

		inline Variable Variable::CreateUniformConstant(Type type, const char* name)
		{
			return Variable(spv::StorageClassUniformConstant, type, name);
		}
		
		inline Type Variable::GetType()
		{
			return GetValue().GetType();
		}

		inline Type Variable::GetVariableType() const
		{
			return ShaderBuilder::GetBuilder().getTypeId(id);
		}

		inline Value Variable::ExtractFloat(VectorComponent comp)
		{
			return GetValue().ExtractFloat(comp);
		}

		inline Value Variable::ExtractVector2f()
		{
			return GetValue().ExtractVector2f();
		}

		inline Value Variable::ExtractVector3f()
		{
			return GetValue().ExtractVector3f();
		}

		inline Value Variable::Shuffle(Value rhs, uint32 a, uint32 b, uint32 c, uint32 d)
		{
			return GetValue().Shuffle(rhs, a, b, c, d);
		}

		template <typename T>
		inline Value Variable::ToVector4f(T t)
		{
			return GetValue().ToVector4f(t);
		}

		template <typename T>
		inline Value Variable::ToVector4f(T t1, T t2)
		{
			return GetValue().ToVector4f(t1, t2);
		}

		template <typename T>
		inline Value Variable::ToVector4f(T t1, T t2, T t3)
		{
			return GetValue().ToVector4f(t1, t2, t3);
		}

		inline Value aw::sb::Variable::X()
		{
			return GetValue().X();
		}

		inline Value aw::sb::Variable::Y()
		{
			return GetValue().Y();
		}

		inline Value aw::sb::Variable::Z()
		{
			return GetValue().Z();
		}

		inline Value aw::sb::Variable::W()
		{
			return GetValue().W();
		}

		inline Value aw::sb::Variable::XY()
		{
			return GetValue().XY();
		}

		inline Value aw::sb::Variable::XYZ()
		{
			return GetValue().XYZ();
		}

		inline Value Variable::Normalize()
		{
			return GetValue().Normalize();
		}

		inline Value Variable::Dot(Value rhs)
		{
			return GetValue().Dot(rhs);
		}

		inline Value Variable::Cross(Value rhs)
		{
			return GetValue().Cross(rhs);
		}

		inline Value Variable::Abs()
		{
			return GetValue().Abs();
		}

		inline Value Variable::Transpose()
		{
			return GetValue().Transpose();
		}

		inline Value Variable::Inverse()
		{
			return GetValue().Inverse();
		}

		inline Value Variable::Negate()
		{
			return GetValue().Negate();
		}

		inline Value Variable::ImageSampleImplicitLod(Value texCoordinates, Type type)
		{
			return GetValue().ImageSampleImplicitLod(texCoordinates, type);
		}

		inline Variable::operator Value()
		{
			return GetValue();
		}

		inline Value Variable::operator-()
		{
			return -GetValue();
		}

		inline Value Variable::operator+(Value _value)
		{
			return GetValue() + _value;
		}

		inline Value Variable::operator-(Value _value)
		{
			return GetValue() - _value;
		}

		inline Value Variable::operator*(Value _value)
		{
			return GetValue() * _value;
		}

		inline Value Variable::operator/(Value _value)
		{
			return GetValue() / _value;
		}

		inline Value Variable::operator<(Value _value)
		{
			return GetValue() < _value;
		}

		inline Value Variable::operator>(Value _value)
		{
			return GetValue() > _value;
		}

		inline Variable& Variable::operator++()
		{
			return operator+=(1_c);
		}

		inline Variable Variable::operator++(int)
		{
			Variable variable(*this);
			return ++variable;
		}

		inline Variable& Variable::operator--()
		{
			return operator-=(1_c);
		}

		inline Variable Variable::operator--(int)
		{
			Variable variable(*this);
			return --variable;
		}

		inline Variable& Variable::operator+=(Value _value)
		{
			operator=(Load() + _value);

			return *this;
		}

		inline Variable& Variable::operator-=(Value _value)
		{
			operator=(Load() - _value);

			return *this;
		}

		inline Variable& Variable::operator*=(Value _value)
		{
			operator=(Load() * _value);

			return *this;
		}

		inline Variable& Variable::operator=(Value _value)
		{
			Store(_value);
			value = _value;

			return *this;
		}

		inline Variable& Variable::operator=(const Variable& other)
		{
			return operator=(other.Load());
		}

		inline Variable& Variable::operator=(Variable&& other)
		{
			id = other.id;
			value = other.value;
			storageClass = other.storageClass;

			return *this;
		}
	}
}