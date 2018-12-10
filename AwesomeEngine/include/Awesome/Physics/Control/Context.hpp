#pragma once

#ifndef AWESOME_PHYSICS_CONTEXT_GUARD
#define AWESOME_PHYSICS_CONTEXT_GUARD

#include <Awesome/Physics/Control/Foundation.hpp>
#include <Awesome/Physics/Control/Pvd.hpp>
#include <Awesome/Physics/Control/Cooking.hpp>
#include <Awesome/Physics/Control/Instance.hpp>
#include <Awesome/Physics/Control/Extensions.hpp>
#include <Awesome/Physics/Control/GPUContext.hpp>

namespace aw
{
	namespace ph
	{
		class Context
		{
			AWESOME_PHYSICS_API static AllocatorCallback allocatorCallback;
			AWESOME_PHYSICS_API static DebugCallback debugCallback;

		public:
			AWESOME_PHYSICS_API static Foundation foundation;
			AWESOME_PHYSICS_API static Pvd pvd;
			AWESOME_PHYSICS_API static Instance instance;
			AWESOME_PHYSICS_API static Cooking cooking;
			AWESOME_PHYSICS_API static Extensions extensions;
			AWESOME_PHYSICS_API static GPUContext gpuContext;

			Context() = delete;
			~Context() = delete;

			static inline void Init();
			static inline void UnInit();
		};
	}
}

#include <Awesome/Physics/Control/Context.inl>

#endif // GUARD