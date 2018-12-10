#include <Awesome/SDK/Components/Render/Text2D.hpp>

namespace aw
{
	namespace sdk
	{
		bool Text2D::UpdateContent()
		{
			if (m_fontAtlas == nullptr)
			{
				LOG_ERROR("Text2D does not have a font atlas");
				return false;
			}

			if (m_updated)
				return true;

			std::vector<uint8> vertices;
			std::vector<uint8> indices;

			auto makeText = [&](std::string str)
			{
				float x = 0.0f;
				float y = 0.0f;

				auto addChar = [&](char c)
				{
					vertices.resize(vertices.size() + 4 * VertexPT::Size);
					indices.resize(indices.size() + 6 * sizeof(uint16));

					VectorView<VertexPT> verticesView(vertices);
					VectorView<uint16> indicesView(indices);

					stbtt_aligned_quad q;

					stbtt_GetPackedQuad(m_fontAtlas->chardata[0], m_fontAtlas->atlas.width, m_fontAtlas->atlas.height, c, &x, &y, &q, 0);

					verticesView[verticesView.Size() - 4 + 0].Get<PositionXYZ, Vector3>() = Vector3(q.x0, q.y0, 0.0f);
					verticesView[verticesView.Size() - 4 + 0].Get<TextureUV, Vector2>() = Vector2(q.s0, q.t0);
					verticesView[verticesView.Size() - 4 + 1].Get<PositionXYZ, Vector3>() = Vector3(q.x1, q.y0, 0.0f);
					verticesView[verticesView.Size() - 4 + 1].Get<TextureUV, Vector2>() = Vector2(q.s1, q.t0);
					verticesView[verticesView.Size() - 4 + 2].Get<PositionXYZ, Vector3>() = Vector3(q.x1, q.y1, 0.0f);
					verticesView[verticesView.Size() - 4 + 2].Get<TextureUV, Vector2>() = Vector2(q.s1, q.t1);
					verticesView[verticesView.Size() - 4 + 3].Get<PositionXYZ, Vector3>() = Vector3(q.x0, q.y1, 0.0f);
					verticesView[verticesView.Size() - 4 + 3].Get<TextureUV, Vector2>() = Vector2(q.s0, q.t1);

					indicesView[indicesView.Size() - 6 + 0] = verticesView.Size() - 4 + 0;
					indicesView[indicesView.Size() - 6 + 1] = verticesView.Size() - 4 + 1;
					indicesView[indicesView.Size() - 6 + 2] = verticesView.Size() - 4 + 3;
					indicesView[indicesView.Size() - 6 + 3] = verticesView.Size() - 4 + 1;
					indicesView[indicesView.Size() - 6 + 4] = verticesView.Size() - 4 + 2;
					indicesView[indicesView.Size() - 6 + 5] = verticesView.Size() - 4 + 3;
				};

				for (uint64 i{ 0 }; i < str.size(); i++)
				{
					addChar(str[i]);
				}
			};

			m_mutex.lock();
			std::string content = m_content;
			m_mutex.unlock();

			makeText(content);

			m_mesh.vertexLayout = &VertexPT::Layout;
			m_mesh.indexType = vk::IndexType::eUint16;
			m_mesh.SetVertices(std::move(vertices));
			m_mesh.SetIndices(std::move(indices));

			m_mesh.Flush();
			m_updated = true;

			return true;
		}
	}
}