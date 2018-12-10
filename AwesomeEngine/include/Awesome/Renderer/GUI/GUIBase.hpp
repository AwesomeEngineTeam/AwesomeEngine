#pragma once

#ifndef AWESOME_RENDERER_GUI_BASE_GUARD
#define AWESOME_RENDERER_GUI_BASE_GUARD

#include <Awesome/Renderer/Memory/Buffer.hpp>
#include <Awesome/Renderer/Resource/Material.hpp>

namespace aw
{
	class GUIBase
	{
		friend class GUIManager;

		MaterialHandle m_material{};
		Buffer m_buffer;
		uint64 m_sizeofVertexBuffer{ 0 };

	protected:
		virtual void Update() = 0;

	public:
		AWESOME_RENDERER_API GUIBase();
		GUIBase(const GUIBase& other) = delete;
		GUIBase(GUIBase&& other) = delete;
		virtual ~GUIBase() = default;

		inline void Draw(vk::CommandBuffer);

		inline MaterialHandle& GetMaterial();
		inline const MaterialHandle& GetMaterial() const;

		GUIBase& operator=(const GUIBase& rhs) = delete;
		GUIBase& operator=(GUIBase&& rhs) = delete;
	};
}

#include <Awesome/Renderer/GUI/GUIBase.inl>

#endif //Guard