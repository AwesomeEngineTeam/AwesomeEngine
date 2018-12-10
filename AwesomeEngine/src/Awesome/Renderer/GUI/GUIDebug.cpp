#include <Awesome/Renderer/GUI/GUIDebug.hpp>

#include <Awesome/Renderer/GUI/GUIManager.hpp>
#include <Awesome/Renderer/Memory/AllocatorMemoryPool.hpp>

#include <imgui/imgui.h>

#define MAX_LINES_HISTORY 1000

namespace aw
{
	void GUIDebug::Update()
	{
		UpdateOutputWidget();
		UpdateMemoryWidget();
	}

	void GUIDebug::UpdateOutputWidget()
	{
		if (m_outputWidgetActive == false)
			return;

		ImGui::Begin("Output", &m_outputWidgetActive, ImGuiWindowFlags_AlwaysHorizontalScrollbar);

		for (std::string line(IMGUI_TEXT_BUFFER_SIZE, '\0'); m_outputStream.getline(line.data(), line.size(), '\n');)
			m_outputLines.push_back(line);

		m_outputStream.clear();//clears eofbit error from getline

        if(m_outputLines.size() > MAX_LINES_HISTORY)
            m_outputLines.erase(m_outputLines.begin(),m_outputLines.begin() + (m_outputLines.size() - MAX_LINES_HISTORY));

		for (auto& line : m_outputLines)
			ImGui::Text("%s", line.c_str());

		if (m_lastNbOutputLines != m_outputLines.size())
			ImGui::SetScrollY(ImGui::GetScrollMaxY());

		ImGui::End();

		m_lastNbOutputLines = m_outputLines.size();
	}

	void GUIDebug::UpdateMemoryWidget()
	{
		if (m_memoryWidgetActive == false)
			return;

		ImGui::Begin("Memory", &m_memoryWidgetActive, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("Allocation Time: %lfms", Allocator::s_allocTime);
		ImGui::Text("Deallocation Time: %lfms", Allocator::s_deallocTime);

		ImGui::NewLine();

		ImGui::Text("Total Allocation Time: %lfms", Allocator::s_allocTime + Allocator::s_deallocTime);

		ImGui::NewLine();

		//TODO Fix potentially unsafe command line
		if (m_selectedBlocks.size() != Allocator::s_pools.size())
			m_selectedBlocks.resize(Allocator::s_pools.size());

		if (ImGui::BeginCombo("Memory Pool", m_selectedPool == -1 ? "Select Memory Pool" : ("AllocatorMemoryPool " + std::to_string(Allocator::s_pools[m_selectedPool].m_id)).c_str(), 0) == true)
		{
			for (uint64 i = 0; i < Allocator::s_pools.size(); i++)
			{
				std::string poolName = "AllocatorMemoryPool " + std::to_string(Allocator::s_pools[i].m_id);
				ImGui::PushID((void*)(intptr_t)i);
				const bool item_selected = (i == m_selectedPool);
				if (ImGui::Selectable(poolName.c_str(), item_selected) == true)
					m_selectedPool = uint8(i);
				if (item_selected)
					ImGui::SetItemDefaultFocus();
				ImGui::PopID();
			}

			ImGui::EndCombo();
		}

		if (m_selectedPool == -1)
		{
			ImGui::End();
			return;
		}

		ImGui::Text("Memory Type ID: %u", Allocator::s_pools[m_selectedPool].m_memTypeId);
		ImGui::NewLine();
		ImGui::Text("Size: %lfMB", Allocator::s_pools[m_selectedPool].m_size * BYTES_TO_MB);
		ImGui::Text("Memory left: %lfMB", Allocator::s_pools[m_selectedPool].m_memoryLeft * BYTES_TO_MB);
		ImGui::Text("Number of blocks: %llu", Allocator::s_pools[m_selectedPool].m_blocks.size());

		ImGui::NewLine();

		ImVec4 red(255, 0, 0, 255);
		ImVec4 green(0, 255, 0, 255);
		ImVec4 yellow (255, 255, 0, 255);

		ImGui::PushStyleColor(ImGuiCol_Text, red);
		ImGui::BulletText("Used blocks");
		ImGui::PopStyleColor();

		ImGui::PushStyleColor(ImGuiCol_Text, green);
		ImGui::BulletText("Free blocks");
		ImGui::PopStyleColor();

		ImGui::PushStyleColor(ImGuiCol_Text, yellow);
		ImGui::BulletText("2 or more contiguous free blocks");
		ImGui::PopStyleColor();

		ImGui::NewLine();

		bool lastWasFree = false;
		std::vector<std::pair<std::string, ImVec4>> items(Allocator::s_pools[m_selectedPool].m_blocks.size());

		for (uint64 j = 0; j<Allocator::s_pools[m_selectedPool].m_blocks.size(); ++j)
		{
			if (Allocator::s_pools[m_selectedPool].m_blocks[j]->free == true)
			{
				if (lastWasFree == true)
				{
                    items[j].second = yellow;
                    items[j-1].second = yellow;
                }
				else
					items[j].second = green;

				lastWasFree = true;
			}
			else
			{
				items[j].second = red;
				lastWasFree = false;
			}

			std::ostringstream oss;
			oss << Allocator::s_pools[m_selectedPool].m_blocks[j].get();
			items[j].first = oss.str();
		}

		const char* headerName = "";
		if (ImGui::ListBoxHeader(headerName, int32(items.size()), -1) == true)
		{
			ImGuiListClipper clipper(int32(items.size()), ImGui::GetTextLineHeightWithSpacing());
			while(clipper.Step())
				for (int j = clipper.DisplayStart; j < clipper.DisplayEnd; ++j)
				{
					const bool item_selected = (j == m_selectedBlocks[m_selectedPool]);

					ImGui::PushID(j);
					ImGui::PushStyleColor(ImGuiCol_Text, items[j].second);

					if (ImGui::Selectable(items[j].first.c_str(), item_selected) == true)
						m_selectedBlocks[m_selectedPool] = j;

					if (item_selected)
						ImGui::SetItemDefaultFocus();

					ImGui::PopID();
					ImGui::PopStyleColor();
				}
			ImGui::ListBoxFooter();
		}

		ImGui::SameLine();
		ImGui::Text("Size: %lfMB\nOffset: %lfMB", Allocator::s_pools[m_selectedPool].m_blocks[m_selectedBlocks[m_selectedPool]]->size * BYTES_TO_MB, Allocator::s_pools[m_selectedPool].m_blocks[m_selectedBlocks[m_selectedPool]]->offset * BYTES_TO_MB);

		ImGui::NewLine();

		ImGui::End();
	}
}