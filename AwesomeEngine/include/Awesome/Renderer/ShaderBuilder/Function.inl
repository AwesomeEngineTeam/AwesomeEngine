namespace aw
{
	namespace sb
	{
		inline Function::Function(Type returnType, const std::vector<Type>& paramTypesNonPtr, const std::vector<std::vector<spv::Decoration>>& precisions, const char* name)
		{
			func = ShaderBuilder::GetBuilder().makeFunctionEntry(spv::NoPrecision, returnType, name, TypesToPointerIds(paramTypesNonPtr), precisions, &entryBlock);
		}

		inline Variable Function::GetParam(int32 index) const
		{
			return Variable(func->getParamId(index), spv::StorageClassFunction);
		}

		inline Variable Function::CreateVariable(Type type, const char* name) const
		{
			return Variable(spv::StorageClassFunction, type, name);
		}

		inline Value Function::Call(const std::vector<Value>& arguments) const
		{
			std::vector<spv::Id> argumentIds(arguments.size());

			for (uint32 i = 0; i < arguments.size(); i++)
			{
				Variable var = CreateVariable(arguments[i].GetType());
				var = arguments[i];
				argumentIds[i] = var.id;
			}

			return CallWithIds(argumentIds);
		}

		inline Value Function::CallWithIds(const std::vector<spv::Id>& arguments) const
		{
			return ShaderBuilder::GetBuilder().createFunctionCall(func, arguments);
		}

		inline void Function::Return()
		{
			ShaderBuilder::GetBuilder().makeReturn(false);
		}

		inline void Function::ReturnImplicit()
		{
			ShaderBuilder::GetBuilder().makeReturn(true);
		}

		inline void Function::Return(Value value)
		{
			ShaderBuilder::GetBuilder().makeReturn(false, value);
		}

		inline void Function::Leave()
		{
			ShaderBuilder::GetBuilder().leaveFunction();
		}

		inline void Function::AddExecutionMode(spv::ExecutionMode executionMode) const
		{
			ShaderBuilder::GetBuilder().addExecutionMode(func, executionMode);
		}

		inline Function Function::MakeEntryPoint(const char* name)
		{
			Function entry;

			entry.func = ShaderBuilder::GetBuilder().makeEntryPoint(name);
			entry.entryBlock = entry.func->getEntryBlock();

			return entry;
		}

		inline std::vector<spv::Id> Function::TypesToPointerIds(const std::vector<Type>& paramTypesNonPtr)
		{
			std::vector<spv::Id> paramTypes(paramTypesNonPtr.size());

			for (uint32 i = 0; i < paramTypesNonPtr.size(); i++)
				paramTypes[i] = Type::MakePointer(paramTypesNonPtr[i], spv::StorageClassFunction);

			return paramTypes;
		}

		inline Value Function::operator()(const std::vector<Value>& arguments) const
		{
			return Call(arguments);
		}
	}
}