#pragma once

#ifndef AWESOME_RENDERER_SHADER_BUILDING_ID_WRAPPER_GUARD
#define AWESOME_RENDERER_SHADER_BUILDING_ID_WRAPPER_GUARD

#include <Awesome/Renderer/ShaderBuilder/ShaderBuilder.hpp>

namespace aw
{
	namespace sb
	{
		struct IdWrapper
		{
			spv::Id id{ 0 };

			IdWrapper() = default;
			inline IdWrapper(spv::Id);

			inline void AddDecoration(spv::Decoration, int32 num = -1);
			inline void AddMemberDecoration(uint32 member, spv::Decoration, int32 num = -1);

			inline operator spv::Id();
			inline operator const spv::Id() const;
		};
	}
}

#include <Awesome/Renderer/ShaderBuilder/IdWrapper.inl>

#endif //GUARD
