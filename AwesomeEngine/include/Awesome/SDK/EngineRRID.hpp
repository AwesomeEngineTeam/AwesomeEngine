#pragma once

#ifndef AWESOME_SDK_ENGINE_RRID_GUARD
#define AWESOME_SDK_ENGINE_RRID_GUARD

#include <Awesome/Windowing.hpp>
#include <Awesome/Renderer.hpp>
#include <Awesome/Physics.hpp>
#include <Awesome/Sound/Sound.hpp>

namespace aw
{
	namespace sdk
	{
		class EngineRRID
		{
		public:
			inline EngineRRID();
			EngineRRID(EngineRRID&&) = delete;
			EngineRRID(const EngineRRID&) = delete;
			inline ~EngineRRID();

			EngineRRID& operator=(EngineRRID&&) = delete;
			EngineRRID& operator=(const EngineRRID&) = delete;
		};
	}
}

#include <Awesome/SDK/EngineRRID.inl>

#endif // GUARD