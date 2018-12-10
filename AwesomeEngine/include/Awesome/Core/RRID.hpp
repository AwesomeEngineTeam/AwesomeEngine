#pragma once

#ifndef AWESOME_CORE_RRID_GUARD
#define AWESOME_CORE_RRID_GUARD

namespace aw
{
	template<typename InitFunc>
	struct RRID
	{
		template<typename... Args>
		RRID(InitFunc initFunc, void(*unInitFunc)(), Args&&... args) : m_unInitFunc{unInitFunc}
		{
			initFunc(args...);
		}

		~RRID() { m_unInitFunc(); }

	private:
		void(*m_unInitFunc)();
	};
}

#endif