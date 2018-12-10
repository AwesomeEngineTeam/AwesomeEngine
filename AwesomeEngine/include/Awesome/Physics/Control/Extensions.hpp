#pragma once

#ifndef AWESOME_PHYSICS_EXTENSIONS_GUARD
#define AWESOME_PHYSICS_EXTENSIONS_GUARD

#include <physX/extensions/PxExtensionsAPI.h>

#include <Awesome/Physics/Config.hpp>

namespace aw
{
	namespace ph
	{
		class Extensions
		{
			bool m_isInit = false;

		public:
			Extensions() = default;
			inline Extensions(Extensions&& other);
			Extensions(const Extensions&) = delete;
			inline ~Extensions();

			AWESOME_PHYSICS_API void Create();
			AWESOME_PHYSICS_API void Destroy();

			inline Extensions& operator=(Extensions&& other);
			Extensions& operator=(const Extensions&) = delete;
		};
	}
}

#include <Awesome/Physics/Control/Extensions.inl>

#endif // GUARD