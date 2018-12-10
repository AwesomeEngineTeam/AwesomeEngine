#pragma once

#ifndef AWESEOME_PHYSICS_TOLERANCES_SCALE_GUARD
#define AWESEOME_PHYSICS_TOLERANCES_SCALE_GUARD

#include <physX/common/PxTolerancesScale.h>

namespace aw
{
	namespace ph
	{
		class TolerancesScale : public physx::PxTolerancesScale
		{
		};
	}
}

#endif // GUARD