#include <Awesome/Renderer/GUI/GUIManager.hpp>
#include <Awesome/Renderer/GUI/GUIBase.hpp>

#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Control/RenderTarget.hpp>
#include <Awesome/Renderer/Pipeline/StaticVertex.hpp>
#include <Awesome/Renderer/Resource/TexelBuffer.hpp>
#include <Awesome/Renderer/Resource/Texture.hpp>
#include <Awesome/Renderer/Descriptor/DescriptorSetLayoutInfo.hpp>
#include <Awesome/Renderer/Pipeline/PipelineInfo.hpp>
#include <Awesome/Renderer/Framegraph/Pass.hpp>

#include <Awesome/Windowing/Input/InputManager.hpp>

#define IMGUI_MOUSEWHEEL_UNIT 0.2f
#define GUI_SCROLL_SENSITIVITY 5

namespace aw
{
	ImGuiContext* GUIManager::s_context{ nullptr };

	SampledTexture* s_font;
	ImFontAtlas* s_fontAtlas = nullptr;

	void GUIManager::Init()
	{
		ASSERT(false, "Do not call this function");

		s_fontAtlas = new ImFontAtlas();
		s_fontAtlas->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 14);

		uint8* texels;

		TexelBuffer texelBuffer;
		s_fontAtlas->GetTexDataAsRGBA32(&texels, reinterpret_cast<int*>(&texelBuffer.width), reinterpret_cast<int*>(&texelBuffer.height));
		texelBuffer.channels = 4;

		texelBuffer.texels.resize(texelBuffer.width * texelBuffer.height * texelBuffer.channels);
		memcpy(texelBuffer.texels.data(), texels, texelBuffer.width * texelBuffer.height * texelBuffer.channels);

		s_font = new SampledTexture(); // TODO NO
		s_font->Create(vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm, std::move(texelBuffer), vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst, vk::ImageAspectFlagBits::eColor);

		//		vk::Image fontImage = s_font.GetImage();



		//auto fontImage = s_font.GetImage();
		//std::memcpy(s_fontAtlas->TexID, fontImage, sizeof(void*));
		//		s_fontAtlas->TexID = u.voidPtr;
		//s_fontAtlas->TexID = reinterpret_cast<void*&>(fontImage);

		s_context = ImGui::CreateContext(s_fontAtlas);
		ImGui::SetCurrentContext(s_context);

		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize.x = 1280.0f;
		io.DisplaySize.y = 800.0f;
		io.RenderDrawListsFn = nullptr;
		//io.KeyMap[ImGuiKey_Backspace] = int(KeyCode::Backspace);
		//io.KeyMap[ImGuiKey_Enter] = int(KeyCode::Enter);

		//pipelineInfo.AddColorBlendAttachment(true, vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOneMinusSrcAlpha, vk::BlendOp::eAdd, vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd, vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

		//auto& dset = reinterpret_cast<DescriptorSet&>(s_descriptorSet.get());
		//dset.SetImageToBinding(0, imguiDSLInfo, s_font.GetImageView(), vk::ImageLayout::eShaderReadOnlyOptimal);

		ImGui::StyleColorsDark();
	}

	void GUIManager::UnInit()
	{
		ImGui::DestroyContext(s_context);

		//for (auto& context : s_contexts)
		//	ImGui::DestroyContext(context.second.handle);

		//s_font.Destroy();
		//delete s_fontAtlas;

		//s_sampler.reset();
		//s_pipelineLayout.reset();

		//s_descriptorSet.reset();
		//s_descriptorPool.Destroy();
		//s_descLayout.reset();

		//s_contexts.clear();
	}
	uint64 sizeofVertexBuffer;
	void GUIManager::Update(GUIBase* gui, vk::CommandBuffer commandBuffer, uint32 windowID, bool b)
	{
		/*ASSERT(gui == nullptr, "gui == nullptr");

		ImGuiIO& io = ImGui::GetIO();

		uint64 v_offset = 0;
		uint64 i_offset = 0;

		if (b == true)
		{
			if (InputManager::GetFocusedWindowID() == windowID)
			{
				io.MousePos = InputManager::GetMouseRelativePosition();
				io.MouseDown[0] = InputManager::IsMouseButtonDown(MouseButton::Left);
				io.MouseWheel = InputManager::GetMouseWheelDelta() * IMGUI_MOUSEWHEEL_UNIT * GUI_SCROLL_SENSITIVITY; //TODO use config.lua

				for (uint32 keycode = static_cast<uint32>(KeyCode::_0), keycodeNumpad = static_cast<uint32>(KeyCode::Numpad0);
					keycode <= static_cast<uint32>(KeyCode::_9);
					++keycode, ++keycodeNumpad)
				{
					if (InputManager::GetKeyPressed(static_cast<KeyCode>(keycode)) || InputManager::GetKeyPressed(static_cast<KeyCode>(keycodeNumpad)))
						io.AddInputCharacter(static_cast<ImWchar>(keycode));
				}

				io.KeysDown[static_cast<int>(KeyCode::Backspace)] = InputManager::GetKeyPressed(KeyCode::Backspace);
				io.KeysDown[static_cast<int>(KeyCode::Enter)] = InputManager::GetKeyPressed(KeyCode::Enter);
			}

			ImGui::NewFrame();

			gui->Update();

			ImGui::Render();
			ImDrawData* draw_data = ImGui::GetDrawData();

			sizeofVertexBuffer = draw_data->TotalVtxCount * sizeof(ImDrawVert);
			for (int n = 0; n < draw_data->CmdListsCount; n++)
			{
				const ImDrawList* cmd_list = draw_data->CmdLists[n];
				gui->m_buffer.Write(cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), v_offset);
				gui->m_buffer.Write(cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), sizeofVertexBuffer + i_offset);

				v_offset += cmd_list->VtxBuffer.Size * sizeof(ImDrawVert);
				i_offset += cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx);
			}
		}

		ImDrawData* draw_data = ImGui::GetDrawData();

		//commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, context.second.pipeline.get());
		//commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, s_pipelineLayout.get(), 0, 1, reinterpret_cast<vk::DescriptorSet*>(&s_descriptorSet), 0, nullptr);

		//float pushConstants[4]{ 2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y, -1, -1 };
		//context.second.commandBuffer->pushConstants(s_pipelineLayout.get(), vk::ShaderStageFlagBits::eVertex, 0, sizeof(float) * 4, pushConstants);

		gui->m_material.BindShader(commandBuffer);

		vk::DescriptorSet dset = gui->m_material.GetDescriptorSet();
		uint32 offset = 0;
		gui->m_material.BindDescriptorSet(commandBuffer, 1, &dset, 1, &offset);

		gui->m_material.SetSampledTexture(SampledTextureHandle(*s_font), 0);

		vk::Buffer buffer = gui->m_buffer.Get();

		uint64 offsets[1] = { 0 };
		commandBuffer.bindVertexBuffers(0, 1, &buffer, offsets);
		commandBuffer.bindIndexBuffer(buffer, sizeofVertexBuffer, vk::IndexType::eUint16);

		v_offset = i_offset = 0;
		for (int i = 0; i < draw_data->CmdListsCount; ++i)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[i];
			for (int j = 0; j < cmd_list->CmdBuffer.Size; ++j)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[j];

				commandBuffer.drawIndexed(pcmd->ElemCount, 1, uint32(i_offset), uint32(v_offset), 0);
				i_offset += pcmd->ElemCount;
			}

			v_offset += cmd_list->VtxBuffer.Size;
		}*/


		/*if (gui->m_meshes.size() != draw_data->CmdListsCount)
			gui->m_meshes.resize(draw_data->CmdListsCount);

		uint64 sizeofVertexBuffer(draw_data->TotalVtxCount * sizeof(ImDrawVert));
		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			gui->m_meshes[n].vertexLayout = &VertexPTC::Layout;
			gui->m_meshes[n].indexType = vk::IndexType::eUint16;

			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			//context.second.buffer.Write(cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), v_offset);
			//context.second.buffer.Write(cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), sizeofVertexBuffer + i_offset);

			std::vector<uint8> vertices(cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
			std::vector<uint8> indices(cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));

			std::memcpy(vertices.data(), cmd_list->VtxBuffer.Data, vertices.size());
			std::memcpy(indices.data(), cmd_list->IdxBuffer.Data, indices.size());

			gui->m_meshes[n].SetVertices(std::move(vertices));
			gui->m_meshes[n].SetIndices(std::move(indices));

			gui->m_meshes[n].Flush();

			//v_offset += cmd_list->VtxBuffer.Size * sizeof(ImDrawVert);
			//i_offset += cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx);
		}*/
	}
}