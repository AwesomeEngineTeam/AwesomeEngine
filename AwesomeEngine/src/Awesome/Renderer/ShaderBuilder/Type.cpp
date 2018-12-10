#include <Awesome/Renderer/ShaderBuilder/Type.hpp>
#include <Awesome/Renderer/ShaderBuilder/Value.hpp>

namespace aw
{
	namespace sb
	{
		Type Type::MakeArray(Type type, const Value &size, int32 stride)
		{
			return ShaderBuilder::GetBuilder().makeArrayType(type, size, stride);
		}
	}
}