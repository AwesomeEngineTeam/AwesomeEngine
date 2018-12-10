#pragma once

#ifndef AWESOME_SDK_EMPTY_SYSTEM_GUARD
#define AWESOME_SDK_EMPTY_SYSTEM_GUARD

#include <vector>

//TODO check system usage
namespace aw
{
	namespace sdk
	{
		class Component;
		class EmptySystem
		{
			static std::vector<Component*> s_components;

		public:
			static bool s_isActive;

			EmptySystem() = delete;
			virtual ~EmptySystem() = default;

			static void GarbageCollect();

			static bool Register(Component*);
		};
	}
}

#endif