namespace aw
{
	inline void GUIBase::Draw(vk::CommandBuffer commandBuffer)
	{
		THROW("Do not call this function");

		//m_material.BindShader(commandBuffer);

		//vk::DescriptorSet dset = m_material.GetDescriptorSet();
		//uint32 offset = 0;
		//m_material.BindDescriptorSet(commandBuffer, 1, &dset, 1, &offset);

		//m_material.PushConstants(commandBuffer);

		//vk::Buffer buffer = m_buffer.Get();

		//uint64 offsets[1] = { 0 };
		//commandBuffer.bindVertexBuffers(0, 1, &buffer, offsets);
		//commandBuffer.bindIndexBuffer(buffer, m_sizeofVertexBuffer, vk::IndexType::eUint16);

		//uint64 v_offset = 0;
		//uint64 i_offset = 0;

		//for (int i = 0; i < draw_data->CmdListsCount; ++i)
		//{
		//	const ImDrawList* cmd_list = draw_data->CmdLists[i];
		//	for (int j = 0; j < cmd_list->CmdBuffer.Size; ++j)
		//	{
		//		const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[j];

		//		commandBuffer.drawIndexed(pcmd->ElemCount, 1, uint32(i_offset), uint32(v_offset), 0);
		//		i_offset += pcmd->ElemCount;
		//	}

		//	v_offset += cmd_list->VtxBuffer.Size;
		//}

		// for (auto& mesh : m_meshes)
			// mesh.Draw(commandBuffer);
	}

	inline MaterialHandle& GUIBase::GetMaterial()
	{
		return m_material;
	}

	inline const MaterialHandle& GUIBase::GetMaterial() const
	{
		return m_material;
	}
}