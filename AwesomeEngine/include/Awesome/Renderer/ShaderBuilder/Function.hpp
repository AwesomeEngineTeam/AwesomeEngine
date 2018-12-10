#pragma once

#ifndef AWESOME_RENDERER_SHADER_BUILDING_FUNCTION_GUARD
#define AWESOME_RENDERER_SHADER_BUILDING_FUNCTION_GUARD

#include <Awesome/Renderer/ShaderBuilder/Variable.hpp>

#include <Awesome/Core/Debug.hpp>

namespace aw
{
	namespace sb
	{
		struct Function
		{
			spv::Function* func{ nullptr };
			spv::Block* entryBlock{ nullptr };
			
			Function() = default;
			inline Function(Type returnType, const std::vector<Type>& paramTypesNonPtr = {}, const std::vector<std::vector<spv::Decoration>>& precisions = {}, const char* name = nullptr);
			
			inline Variable GetParam(int32 index) const;
			inline Variable CreateVariable(Type type, const char* name = nullptr) const;

			inline Value Call(const std::vector<Value>& arguments) const;
			inline Value CallWithIds(const std::vector<spv::Id>& arguments) const;
			inline void Return();
			inline void ReturnImplicit();
			inline void Return(Value);
			inline void Leave();
			inline void AddExecutionMode(spv::ExecutionMode) const;

			inline static Function MakeEntryPoint(const char* name);

		private:
			inline static std::vector<spv::Id> TypesToPointerIds(const std::vector<Type>& paramTypesNonPtr);

		public:
			inline Value operator()(const std::vector<Value>& arguments) const;
		};
	}
}

#include <Awesome/Renderer/ShaderBuilder/Function.inl>

#endif //GUARD
