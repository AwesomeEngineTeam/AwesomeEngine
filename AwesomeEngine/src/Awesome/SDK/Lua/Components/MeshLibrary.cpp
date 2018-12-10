#include <Awesome/SDK/Lua/Libraries.hpp>

#define WRAP_MESH(field) LUWRA_MEMBER(MeshRenderer, field)

namespace aw
{
	namespace sdk
	{
		namespace lua
		{
			namespace mesh
			{
				const luwra::MemberMap Fields
				{
					COMPONENT_FIELDS(MeshRenderer),
					WRAP_MESH(SetMesh),
					WRAP_MESH(GetMesh),
					WRAP_MESH(SetMaterial),
					WRAP_MESH(GetMaterial),
				};

				const luwra::MemberMap MetaMethods
				{

				};
			}
		}
	}
}

#undef WRAP_MESH