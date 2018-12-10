#pragma once

#ifndef AWESOME_RENDERER_MODEL_GUARD
#define AWESOME_RENDERER_MODEL_GUARD

#include <Awesome/Renderer/Resource/Mesh.hpp>
#include <Awesome/Renderer/Resource/Material.hpp>

#include <memory>

namespace aw
{
	struct Model
	{
		uint32 material{ 0 };
		uint32 mesh{ 0 };
		uint32 pass{ 0 };

		bool operator==(const Model& o) const
		{
			return (material == o.material) && (mesh == o.mesh) && (pass == o.pass);
		}
	};

	struct ModelHash
	{
		std::size_t operator()(const Model& m) const noexcept
		{
			static_assert(sizeof(std::size_t) == sizeof(uint64), "std::size_t must be 64 bits long");

			std::size_t res = m.material;
			res = m.mesh | (res << 32u);

			std::size_t mask{ 0 };
			std::size_t p = m.pass;

			for (uint32 i{ 0 }; i < 32u; ++i)
				mask |= (p & (1u << i)) << i;

			res ^= mask | (mask << 1u);

			return res;
		}
	};
}

#endif //GUARD