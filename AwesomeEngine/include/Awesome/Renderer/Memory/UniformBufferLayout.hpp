#pragma once

#ifndef AWESOME_RENDERER_UNIFORM_BUFFER_LAYOUT_GUARD
#define AWESOME_RENDERER_UNIFORM_BUFFER_LAYOUT_GUARD

#include <Awesome/Core/Types.hpp>

#include <Awesome/Renderer/Config.hpp>

#include <vector>
#include <string>
#include <string_view>

namespace aw
{
	enum class UniformBufferAttribute : uint32
	{
		Invalid = 0,
		Vec3f,
		Vec4f,
		Mat4f,
	};

	class UniformBufferLayout
	{
	public:
		struct Member
		{
			UniformBufferAttribute attribute{ UniformBufferAttribute::Invalid };
			std::string name{};
		};

	private:
		std::vector<Member> m_attributes;
		std::string m_name{};
		uint64 m_arraySize{ 0 };
		bool m_isArray{ false };

	public:
		UniformBufferLayout() = default;
		inline UniformBufferLayout(std::vector<Member> attributes, std::string_view name, bool isArray = false, uint64 arraySize = 0);
		UniformBufferLayout(const UniformBufferLayout&) = default;
		UniformBufferLayout(UniformBufferLayout&&) = default;
		~UniformBufferLayout() = default;

		AWESOME_RENDERER_API uint32 GetSize() const;

		AWESOME_RENDERER_API const UniformBufferLayout::Member& GetAttributeAt(uint32 index) const;

		AWESOME_RENDERER_API std::string GetName() const;
		inline bool IsArray() const;
		inline uint64 GetArraySize() const;

		UniformBufferLayout& operator=(const UniformBufferLayout&) = default;
		UniformBufferLayout& operator=(UniformBufferLayout&&) = default;
	};
}

#include <Awesome/Renderer/Memory/UniformBufferLayout.inl>

#endif //GUARD