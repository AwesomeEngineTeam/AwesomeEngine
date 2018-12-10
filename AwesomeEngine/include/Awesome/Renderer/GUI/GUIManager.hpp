#pragma once

#ifndef AWESOME_RENDERER_GUI_MANAGER_GUARD
#define AWESOME_RENDERER_GUI_MANAGER_GUARD

#define IMGUI_TEXT_BUFFER_SIZE (1024 * 3)//Excluding the last byte used for '\0'

// #include <Awesome/Renderer/Descriptor/DescriptorPool.hpp>
// #include <Awesome/Renderer/Memory/Buffer.hpp>
// #include <Awesome/Renderer/Resource/Texture.hpp>
#include <Awesome/Renderer/Resource/Material.hpp>

#include <imgui/imgui.h>

#include <map>

namespace aw
{
	class GUIBase;

	class GUIManager
	{
		AWESOME_RENDERER_API static ImGuiContext* s_context;

	public:
		GUIManager() = delete;
		~GUIManager() = delete;

		AWESOME_RENDERER_API static void Init();
		AWESOME_RENDERER_API static void UnInit();

		AWESOME_RENDERER_API static void Update(GUIBase*, vk::CommandBuffer, uint32 windowID = 0, bool b = true);
		//AWESOME_RENDERER_API static void Draw(vk::CommandBuffer, GUIBase*);
	};
}

#endif //Guard