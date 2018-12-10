#pragma once

#ifndef AWESOME_RENDERER_SHADER_BUILDING_TYPE_GUARD
#define AWESOME_RENDERER_SHADER_BUILDING_TYPE_GUARD

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>
#include <Awesome/Renderer/ShaderBuilder/IdWrapper.hpp>

namespace aw
{
	namespace sb
	{
		struct Type : IdWrapper
		{
			Type() = default;
			using IdWrapper::IdWrapper;

			inline bool IsBool() const;
			inline bool IsInt() const;
			inline bool IsUint() const;
			inline bool IsFloat() const;
			inline bool IsPointer() const;
			inline bool IsScalar() const;
			inline bool IsVector() const;
			inline bool IsVectorf() const;
			inline bool IsVector2f() const;
			inline bool IsVector3f() const;
			inline bool IsVector4f() const;
			inline bool IsMatrix() const;
			inline bool IsStruct() const;
			inline bool IsArray() const;
			inline bool IsAggregate() const;
			inline bool IsImage() const;
			inline bool IsSampler() const;
			inline bool IsSampledImage() const;

			inline int32 GetNumComponents() const;

			inline Type GetScalarType() const;

			inline static Type MakeVoid();
			inline static Type MakeBool();
			inline static Type MakeInteger(uint32 width, bool hasSign);
			inline static Type MakeInt();
			inline static Type MakeUint();
			inline static Type MakeInt64();
			inline static Type MakeUint64();
			inline static Type MakeFloat();
			inline static Type MakeDouble();
			inline static Type MakeVector(Type, uint32);
			inline static Type MakeVectorf(uint32);
			inline static Type MakeVector2f();
			inline static Type MakeVector3f();
			inline static Type MakeVector4f();
			inline static Type MakeMatrix(Type, uint32, uint32);
			inline static Type MakeMatrixf(uint32, uint32);
			inline static Type MakeMatrix2f();
			inline static Type MakeMatrix3f();
			inline static Type MakeMatrix4f();
			inline static Type MakeImage(Type = MakeFloat(), spv::Dim = spv::Dim2D, bool depth = false, bool arrayed = false, bool ms = false, uint32 sampled = 1, spv::ImageFormat = spv::ImageFormatUnknown);
			inline static Type MakeSampledImage(Type = MakeImage());
			inline static Type MakeStruct(const std::vector<Type>& types, const char* name);
			AWESOME_RENDERER_API static Type MakeArray(Type type, const Value& size, int32 stride);
			inline static Type MakePointer(Type, spv::StorageClass);
		};
	}
}

#include <Awesome/Renderer/ShaderBuilder/Type.inl>

#endif //GUARD
