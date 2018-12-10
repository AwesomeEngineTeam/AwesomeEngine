#include <Awesome/Renderer/Resource/Mesh.hpp>
#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Memory/Buffer.hpp>
#include <Awesome/Renderer/Memory/SubBuffer.hpp>
#include <Awesome/Renderer/Memory/BufferManager.hpp>
#include <Awesome/Renderer/Pipeline/StaticVertex.hpp>
#include <Awesome/Renderer/Pipeline/StaticVertexAttribute.hpp>


#include <unordered_map>

#include <mikktspace.h>

namespace aw
{
	std::mutex Mesh::s_poolMutex;

	void Mesh::DebugVerticesPosition() const
	{
		if (vertexLayout != nullptr)
		{
			uint64 vertexDataSize = vertexLayout->GetSize();
			uint64 vertexCount = m_vertices.size() / vertexDataSize;
			uint64 positionOffset = vertexLayout->GetAttributeOffset(VertexAttribute(PositionXYZ));

			for (uint64 vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex)
				std::cout << vertexIndex << " : " << reinterpret_cast<const Vector3&>(m_vertices[vertexIndex * vertexDataSize + positionOffset]).ToString() << std::endl;
		}
		std::cout << std::endl;
	}

	struct TangentMesh
	{
		bool is16Bits{ true };
		uint8* indices{};
		uint64 indexCount{ ~0ull };
		VertexPTNTaH* vertices{ nullptr };
		uint64 vertexCount{ ~0ull };
		uint64 numFaces{ 0 };
	};

	#define GET_MESH TangentMesh* mesh = reinterpret_cast<TangentMesh*>(pContext->m_pUserData)

	template<typename IndexType>
	static int getNumFaces(const SMikkTSpaceContext * pContext)
	{
		GET_MESH;

		VectorView<IndexType> indices(mesh->indices, mesh->indexCount);

		return indices.Size() / 3;
	}

	static int getNumVerticesOfFace(const SMikkTSpaceContext * pContext, const int iFace)
	{
		return 3;
	}

	template<typename IndexType, uint32 Attribute, typename AttributeType>
	static void getAttribute(const SMikkTSpaceContext * pContext, float fvOut[], const int iFace, const int iVert)
	{
		GET_MESH;

		VectorView<IndexType> indices(mesh->indices, mesh->indexCount);

		AttributeType& attrib = mesh->vertices[indices[iFace * 3 + iVert]].template Get<Attribute, AttributeType>();

		fvOut[0] = attrib.x;
		fvOut[1] = attrib.y;
		if constexpr (Attribute != TextureUV)
			fvOut[2] = attrib.z;
	}

	template<typename IndexType>
	static void setTSpaceBasic(const SMikkTSpaceContext * pContext, const float fvTangent[], const float fSign, const int iFace, const int iVert)
	{
		GET_MESH;

		VectorView<IndexType> indices(mesh->indices, mesh->indexCount);

		mesh->vertices[indices[iFace * 3 + iVert]].template Get<TangentXYZ, Vector3>() = { fvTangent[0], fvTangent[1], fvTangent[2] };
		mesh->vertices[indices[iFace * 3 + iVert]].template Get<Handedness, float>() = -fSign;
	}

	void Mesh::Flush()
	{
		if (m_isCoherent == true)
			return;

		const uint64 sizeofVertexBuffer = m_vertices.size();
		const uint64 sizeofIndexBuffer = m_indices.size();

		static std::mutex mtx;
		mtx.lock();
		if (m_vertexSubBuffer.size < sizeofVertexBuffer || m_vertexSubBuffer.buffer == nullptr)
			m_vertexSubBuffer = BufferManager::Reserve(sizeofVertexBuffer, true, BufferType::Geometry);
		if (m_indexSubBuffer.size < sizeofIndexBuffer || m_indexSubBuffer.buffer == nullptr)
			m_indexSubBuffer = BufferManager::Reserve(sizeofIndexBuffer, true, BufferType::Geometry);

		SubBuffer tempVertexSubBuffer = BufferManager::Reserve(sizeofVertexBuffer, false, BufferType::Staging);
		SubBuffer tempIndexSubBuffer = BufferManager::Reserve(sizeofIndexBuffer, false, BufferType::Staging);
		mtx.unlock();

		tempVertexSubBuffer.Write(m_vertices.data());
		tempIndexSubBuffer.Write(m_indices.data());

		//Context::GetDevice().WaitGraphicsIdle();

		if (!m_flushCommandBuffer)
		{
			s_poolMutex.lock();

			vk::CommandBufferAllocateInfo tmpCmdBufAllocInfo(Context::GetCommandPool(), vk::CommandBufferLevel::ePrimary, 1);
			m_flushCommandBuffer = std::move(Context::GetDevice().Get().allocateCommandBuffersUnique(tmpCmdBufAllocInfo)[0]);

			s_poolMutex.unlock();
		}
		
		vk::BufferCopy regionVertex{ tempVertexSubBuffer.offset, m_vertexSubBuffer.offset, m_vertexSubBuffer.size };
		vk::BufferCopy regionIndex{ tempIndexSubBuffer.offset, m_indexSubBuffer.offset, m_indexSubBuffer.size };

		m_flushCommandBuffer->begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
		m_flushCommandBuffer->copyBuffer(tempVertexSubBuffer.buffer->Get(), m_vertexSubBuffer.buffer->Get(), 1, &regionVertex);
		m_flushCommandBuffer->copyBuffer(tempIndexSubBuffer.buffer->Get(), m_indexSubBuffer.buffer->Get(), 1, &regionIndex);
		m_flushCommandBuffer->end();

		vk::SubmitInfo submitInfo;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_flushCommandBuffer.get();

		//Context::GetDevice().WaitGraphicsIdle();
		Context::GetDevice().Submit(1, &submitInfo, vk::Fence());
		Context::GetDevice().WaitGraphicsIdle();
		//*/

		m_isCoherent = true;
	}

	void Mesh::Clear()
	{
		m_vertices.clear();
		m_indices.clear();

		m_vertexSubBuffer.Clear();
		m_indexSubBuffer.Clear();

		m_flushCommandBuffer.reset();

		m_isCoherent = false;
	}

	void Mesh::GenerateTangents()
	{
		if (vertexLayout == nullptr)
		{
			LOG_ERROR("vertexLayout is nullptr");
			return;
		}
		if (*vertexLayout != VertexPTNTaH::Layout)
		{
			LOG_ERROR("Can not generate tangents with this VertexLayout");
			return;
		}

		/*
		struct BT
		{
			Vector3 tangent{};
			Vector3 bitangent{};
		};
		VectorView<VertexPTNTaH> v(m_vertices);

		std::vector<BT> bts(v.Size());
		//auto v = reinterpret_cast<VertexPTNTaH*>(m_vertices.data());

		if (indexType == vk::IndexType::eUint16)
		{
			VectorView<uint16> tri(m_indices);

			for (int i = 0; i < tri.Size(); i += 3)
			{
				const Vector3 &P0 = v[tri[i + 0]].Get<PositionXYZ, Vector3>();
				const Vector3 &P1 = v[tri[i + 1]].Get<PositionXYZ, Vector3>();
				const Vector3 &P2 = v[tri[i + 2]].Get<PositionXYZ, Vector3>();
				const Vector2 &UV0 = v[tri[i + 0]].Get<TextureUV, Vector2>();
				const Vector2 &UV1 = v[tri[i + 1]].Get<TextureUV, Vector2>();
				const Vector2 &UV2 = v[tri[i + 2]].Get<TextureUV, Vector2>();
				const Vector3 &N = v[tri[i + 0]].Get<NormalXYZ, Vector3>();
				Vector3 Q1 = P1 - P0;
				Vector3 Q2 = P2 - P0;
				Vector2 ST1 = UV1 - UV0;
				Vector2 ST2 = UV2 - UV0;
				Vector3 T = Vector3{
					ST2.y * Q1.x + (-ST1.y) * Q2.x,
					ST2.y * Q1.y + (-ST1.y) * Q2.y,
					ST2.y * Q1.z + (-ST1.y) * Q2.z
				} / ((ST1.x * ST2.y) - (ST2.x * ST1.y));
				Vector3 B = Vector3{
					(-ST2.x) * Q1.x + ST1.x * Q2.x,
					(-ST2.x) * Q1.y + ST1.x * Q2.y,
					(-ST2.x) * Q1.z + ST1.x * Q2.z
				} / ((ST1.x * ST2.y) - (ST2.x * ST1.y));
				Vector3 Tprime = T - (N.Dot(T)) * N;
				Vector3 Bprime = B - (N.Dot(B)) * N - (Tprime.Dot(B)) * Tprime / (Tprime.Dot(Tprime));

				bts[tri[i]].tangent += Tprime;
				bts[tri[i + 1]].tangent += Tprime;
				bts[tri[i + 2]].tangent += Tprime;

				bts[tri[i]].bitangent += Bprime;
				bts[tri[i + 1]].bitangent += Bprime;
				bts[tri[i + 2]].bitangent += Bprime;
			}
		}
		else
		{
			VectorView<uint32> tri(m_indices);

			for (int i = 0; i < tri.Size(); i += 3)
			{
				const Vector3& P0 = v[tri[i + 0]].Get<PositionXYZ, Vector3>();
				const Vector3& P1 = v[tri[i + 1]].Get<PositionXYZ, Vector3>();
				const Vector3& P2 = v[tri[i + 2]].Get<PositionXYZ, Vector3>();
				const Vector2& UV0 = v[tri[i + 0]].Get<TextureUV, Vector2>();
				const Vector2& UV1 = v[tri[i + 1]].Get<TextureUV, Vector2>();
				const Vector2& UV2 = v[tri[i + 2]].Get<TextureUV, Vector2>();
				const Vector3& N = v[tri[i + 0]].Get<NormalXYZ, Vector3>();
				Vector3 Q1 = P1 - P0;
				Vector3 Q2 = P2 - P0;
				Vector2 ST1 = UV1 - UV0;
				Vector2 ST2 = UV2 - UV0;
				Vector3 T = Vector3{
					ST2.y * Q1.x + (-ST1.y) * Q2.x,
					ST2.y * Q1.y + (-ST1.y) * Q2.y,
					ST2.y * Q1.z + (-ST1.y) * Q2.z
				} / ((ST1.x * ST2.y) - (ST2.x * ST1.y));
				Vector3 B = Vector3{
					(-ST2.x) * Q1.x + ST1.x * Q2.x,
					(-ST2.x) * Q1.y + ST1.x * Q2.y,
					(-ST2.x) * Q1.z + ST1.x * Q2.z
				} / ((ST1.x * ST2.y) - (ST2.x * ST1.y));
				Vector3 Tprime = T - (N.Dot(T)) * N;
				Vector3 Bprime = B - (N.Dot(B)) * N - (Tprime.Dot(B)) * Tprime / (Tprime.Dot(Tprime));

				bts[tri[i]].tangent += Tprime;
				bts[tri[i + 1]].tangent += Tprime;
				bts[tri[i + 2]].tangent += Tprime;

				bts[tri[i]].bitangent += Bprime;
				bts[tri[i + 1]].bitangent += Bprime;
				bts[tri[i + 2]].bitangent += Bprime;
			}
		}

		for (int i = 0; i < v.Size(); i++)
		{
			const Vector3& n = v[i].Get<NormalXYZ, Vector3>();
			const Vector3& t = bts[i].tangent;
			const Vector3& b = bts[i].bitangent;

			v[i].Get<TangentXYZ, Vector3>() = (t - n * n.Dot(t)).GetNormalized();
			v[i].Get<Handedness, float>() = ((n.Cross(t)).Dot(b)) < 0.0f ? -1.0f : 1.0f;
			//v[i].Get<PositionXYZ, Vector3>() = v[i].Get<PositionXYZ, Vector3>().GetNormalized();
		}
		//*/

		TangentMesh mikkMesh;
		mikkMesh.indices = m_indices.data();
		mikkMesh.indexCount = m_indices.size();
		mikkMesh.vertices = reinterpret_cast<VertexPTNTaH*>(m_vertices.data());
		mikkMesh.vertexCount = m_vertices.size() / VertexPTNTaH::Size;

		SMikkTSpaceInterface mikk{};

		mikk.m_getNumVerticesOfFace = &getNumVerticesOfFace;
		if (indexType == vk::IndexType::eUint16)
		{
			mikk.m_getNumFaces = &getNumFaces<uint16>;
			mikk.m_getPosition = &getAttribute<uint16, PositionXYZ, Vector3>;
			mikk.m_getNormal = &getAttribute<uint16, NormalXYZ, Vector3>;
			mikk.m_getTexCoord = &getAttribute<uint16, TextureUV, Vector2>;
			mikk.m_setTSpaceBasic = &setTSpaceBasic<uint16>;
		}
		else
		{
			mikk.m_getNumFaces = &getNumFaces<uint32>;
			mikk.m_getPosition = &getAttribute<uint32, PositionXYZ, Vector3>;
			mikk.m_getNormal = &getAttribute<uint32, NormalXYZ, Vector3>;
			mikk.m_getTexCoord = &getAttribute<uint32, TextureUV, Vector2>;
			mikk.m_setTSpaceBasic = &setTSpaceBasic<uint32>;
		}

		SMikkTSpaceContext mikkContext;
		mikkContext.m_pInterface = &mikk;
		mikkContext.m_pUserData = &mikkMesh;

		if (genTangSpaceDefault(&mikkContext) == false)
		{
			LOG_ERROR("Could not generate tangents");
		}

		m_isCoherent = false;
	}

	void Mesh::NormalizeNormals()
	{
		if (vertexLayout == nullptr)
		{
			LOG_ERROR("vertexLayout is nullptr");
			return;
		}
		if (*vertexLayout != VertexPTNTaH::Layout)
		{
			LOG_ERROR("Can not normalize normals with this VertexLayout");
			return;
		}

		VectorView<VertexPTNTaH> vertices(m_vertices);

		for (uint64 i{ 0 }; i < vertices.Size(); i++)
			vertices[i].Get<NormalXYZ, Vector3>().Normalize();

		m_isCoherent = false;
	}

	void Mesh::NormalizePositions()
	{
		if (vertexLayout == nullptr)
		{
			LOG_ERROR("vertexLayout is nullptr");
			return;
		}
		if (*vertexLayout != VertexPTNTaH::Layout)
		{
			LOG_ERROR("Can not normalize positions with this VertexLayout");
			return;
		}

		VectorView<VertexPTNTaH> vertices(m_vertices);

		for (uint64 i{ 0 }; i < vertices.Size(); i++)
			vertices[i].Get<PositionXYZ, Vector3>().Normalize();

		m_isCoherent = false;
	}

	Mesh Mesh::Quad::Create(float length)
	{
		Mesh quadMesh;

		quadMesh.m_vertices.resize(4 * VertexPTNTaH::Size);
		quadMesh.m_indices.resize(6 * sizeof(uint32));

		VectorView<VertexPTNTaH> vertices(quadMesh.m_vertices);
		VectorView<uint32> indices(quadMesh.m_indices);

		auto position = [&](size_t index) -> Vector3&
		{
			return vertices[index].Get<PositionXYZ, Vector3>();
		};

		auto normal = [&](size_t index) -> Vector3&
		{
			return vertices[index].Get<NormalXYZ, Vector3>();
		};

		auto uv = [&](size_t index) -> Vector2&
		{
			return vertices[index].Get<TextureUV, Vector2>();
		};

		position(0) = Vector3(-length * 5.0f, 0.0f, length * 5.0f);
		position(1) = Vector3(length * 5.0f, 0.0f, length * 5.0f);
		position(2) = Vector3(-length * 5.0f, 0.0f, -length * 5.0f);
		position(3) = Vector3(length * 5.0f, 0.0f, -length * 5.0f);

		indices[0] = 0; indices[1] = 1; indices[2] = 2;
		indices[3] = 1; indices[4] = 2; indices[5] = 3;

		normal(0) = Vector3::Up();
		normal(1) = Vector3::Up();
		normal(2) = Vector3::Up();
		normal(3) = Vector3::Up();

		uv(0) = Vector2(0.0f, 1.0f);
		uv(1) = Vector2(1.0f, 1.0f);
		uv(2) = Vector2(0.0f, 0.0f);
		uv(3) = Vector2(1.0f, 0.0f);
		
		quadMesh.GenerateTangents();

		return quadMesh;
	}

	Mesh Mesh::Cube::Create(float length)
	{
		Mesh cubeMesh;

		cubeMesh.m_vertices.resize(24 * VertexPTNTaH::Size);

		VectorView<VertexPTNTaH> vertices(cubeMesh.m_vertices);

		auto position = [&](size_t index) -> Vector3&
		{
			return vertices[index].Get<PositionXYZ, Vector3>();
		};

		position(0) = length * Vector3(-0.5f, 0.5f, 0.5f);
		position(1) = length * Vector3(-0.5f, -0.5f, 0.5f);
		position(2) = length * Vector3(0.5f, -0.5f, 0.5f);
		position(3) = length * Vector3(0.5f, 0.5f, 0.5f);
		position(4) = length * Vector3(-0.5f, 0.5f, -0.5f);
		position(5) = length * Vector3(-0.5f, -0.5f, -0.5f);
		position(6) = length * Vector3(0.5f, -0.5f, -0.5f);
		position(7) = length * Vector3(0.5f, 0.5f, -0.5f);

		for (uint32 i{ 8 }; i < 24; i++)
			position(i) = position(i - 8);

		/*    	 _________________________
		        / 0___________________4  /|
		       / / ___________________/ / |
		      / / /| |               / /  |
		     / / / | |              / / . |			 Y
		    / / /| | |             / / /| |			 |
		   / / / | | |            / / / | |			 |
		  / / /  | | |           / / /| | |	  Z _____.
		 / /_/__________________/ / / | | |			/
		/ 3____________________7 / /  | | |		   /
		| ______________________ | |  | | |       X
		| | |    | | |_________| | |__| | |
		| | |    | 1___________| | |__5 | |
		| | |   / / ___________| | |_  / /
		| | |  / / /           | | |/ / /
		| | | / / /            | | | / /
		| | |/ / /             | | |/ /
		| | | / /              | | ' /
		| | |/_/_______________| |  /
		| 2____________________6 | /
		|________________________|/

		*/

		cubeMesh.m_indices.resize(36 * sizeof(uint32));
		VectorView<uint32> indices(cubeMesh.m_indices);

		// Z+
		indices[0] = 0; indices[1] = 1; indices[2] = 2;
		indices[3] = 2; indices[4] = 3; indices[5] = 0;

		// Y+	  																							
		indices[6] = 8; indices[7] = 11; indices[8] = 15;
		indices[9] = 15; indices[10] = 12; indices[11] = 8;

		// X+	 																							 
		indices[12] = 19; indices[13] = 18; indices[14] = 22;
		indices[15] = 22; indices[16] = 23; indices[17] = 19;

		// Z-	 																							 
		indices[18] = 7; indices[19] = 6; indices[20] = 5;
		indices[21] = 5; indices[22] = 4; indices[23] = 7;

		// Y-	 																							
		indices[24] = 10; indices[25] = 9; indices[26] = 13;
		indices[27] = 13; indices[28] = 14; indices[29] = 10;

		// X-	 																							  
		indices[30] = 17; indices[31] = 16; indices[32] = 20;
		indices[33] = 20; indices[34] = 21; indices[35] = 17;

		auto normal = [&](size_t index) -> Vector3&
		{
			return vertices[index].Get<NormalXYZ, Vector3>();
		};

		normal(0) = Vector3::Forward();
		normal(1) = Vector3::Forward();
		normal(2) = Vector3::Forward();
		normal(3) = Vector3::Forward();

		normal(4) = Vector3::Backward();
		normal(5) = Vector3::Backward();
		normal(6) = Vector3::Backward();
		normal(7) = Vector3::Backward();

		normal(8) = Vector3::Up();
		normal(9) = Vector3::Down();
		normal(10) = Vector3::Down();
		normal(11) = Vector3::Up();

		normal(12) = Vector3::Up();
		normal(13) = Vector3::Down();
		normal(14) = Vector3::Down();
		normal(15) = Vector3::Up();

		normal(16) = Vector3::Left();
		normal(17) = Vector3::Left();
		normal(18) = Vector3::Right();
		normal(19) = Vector3::Right();

		normal(20) = Vector3::Left();
		normal(21) = Vector3::Left();
		normal(22) = Vector3::Right();
		normal(23) = Vector3::Right();

		auto uv = [&](size_t index) -> Vector2&
		{
			return vertices[index].Get<TextureUV, Vector2>();
		};

		uv(0) = Vector2(0.0f, 0.0f); // 0 Z+
		uv(1) = Vector2(0.0f, 1.0f); // 1 Z+
		uv(2) = Vector2(1.0f, 1.0f); // 2 Z+
		uv(3) = Vector2(1.0f, 0.0f); // 3 Z+

		uv(4) = Vector2(1.0f, 0.0f); // 4 Z-
		uv(5) = Vector2(1.0f, 1.0f); // 5 Z-
		uv(6) = Vector2(0.0f, 1.0f); // 6 Z-
		uv(7) = Vector2(0.0f, 0.0f); // 7 Z-

		uv(8) = Vector2(1.0f, 0.0f); // 0 Y+
		uv(9) = Vector2(1.0f, 1.0f); // 1 Y-
		uv(10) = Vector2(0.0f, 1.0f); // 2 Y-
		uv(11) = Vector2(0.0f, 0.0f); // 3 Y+

		uv(12) = Vector2(1.0f, 1.0f); // 4 Y+
		uv(13) = Vector2(1.0f, 0.0f); // 5 Y-
		uv(14) = Vector2(0.0f, 0.0f); // 6 Y-
		uv(15) = Vector2(0.0f, 1.0f); // 7 Y+

		uv(16) = Vector2(1.0f, 0.0f); // 0 X-
		uv(17) = Vector2(1.0f, 1.0f); // 1 X-
		uv(18) = Vector2(0.0f, 1.0f); // 2 X+
		uv(19) = Vector2(0.0f, 0.0f); // 3 X+

		uv(20) = Vector2(0.0f, 0.0f); // 4 X-
		uv(21) = Vector2(0.0f, 1.0f); // 5 X-
		uv(22) = Vector2(1.0f, 1.0f); // 6 X+
		uv(23) = Vector2(1.0f, 0.0f); // 7 X+

		cubeMesh.GenerateTangents();

		return cubeMesh;
	}

	Mesh Mesh::DebugQuad::Create(float length)
	{
		Mesh debugQuadMesh;

		debugQuadMesh.m_vertices.resize(4 * VertexPTNTaH::Size);
		debugQuadMesh.m_indices.resize(8 * sizeof(uint32));

		VectorView<VertexPTNTaH> vertices(debugQuadMesh.m_vertices);
		VectorView<uint32> indices(debugQuadMesh.m_indices);

		auto position = [&](size_t index) -> Vector3&
		{
			return vertices[index].Get<PositionXYZ, Vector3>();
		};

		position(0) = Vector3(-length * 5.0f, 0.0f, length * 5.0f);
		position(1) = Vector3(length * 5.0f, 0.0f, length * 5.0f);
		position(2) = Vector3(-length * 5.0f, 0.0f, -length * 5.0f);
		position(3) = Vector3(length * 5.0f, 0.0f, -length * 5.0f);

		indices[0] = 0; indices[1] = 1;
		indices[2] = 1; indices[3] = 3;
		indices[4] = 3; indices[5] = 2;
		indices[6] = 2; indices[7] = 0;

		return debugQuadMesh;
	}

	Mesh Mesh::DebugCube::Create(float length)
	{
		Mesh debugCubeMesh;

		debugCubeMesh.m_vertices.resize(8 * VertexPTNTaH::Size);

		VectorView<VertexPTNTaH> vertices(debugCubeMesh.m_vertices);

		auto position = [&](size_t index) -> Vector3&
		{
			return vertices[index].Get<PositionXYZ, Vector3>();
		};

		position(0) = length * Vector3(-0.5f, 0.5f, 0.5f);
		position(1) = length * Vector3(-0.5f, -0.5f, 0.5f);
		position(2) = length * Vector3(0.5f, -0.5f, 0.5f);
		position(3) = length * Vector3(0.5f, 0.5f, 0.5f);
		position(4) = length * Vector3(-0.5f, 0.5f, -0.5f);
		position(5) = length * Vector3(-0.5f, -0.5f, -0.5f);
		position(6) = length * Vector3(0.5f, -0.5f, -0.5f);
		position(7) = length * Vector3(0.5f, 0.5f, -0.5f);

		debugCubeMesh.m_indices.resize(24 * sizeof(uint32));
		VectorView<uint32> indices(debugCubeMesh.m_indices);

		indices[0] = 0; indices[1] = 1;
		indices[2] = 1; indices[3] = 2;
		indices[4] = 2; indices[5] = 3;
		indices[6] = 3; indices[7] = 0;

		indices[8] = 0; indices[9] = 4;
		indices[10] = 1; indices[11] = 5;
		indices[12] = 2; indices[13] = 6;
		indices[14] = 3; indices[15] = 7;

		indices[16] = 4; indices[17] = 5;
		indices[18] = 5; indices[19] = 6;
		indices[20] = 6; indices[21] = 7;
		indices[22] = 7; indices[23] = 4;

		return debugCubeMesh;
	}

	Mesh Mesh::DebugSphere::Create(float radius)
	{
		Mesh debugSphereMesh;

		debugSphereMesh.m_vertices.resize(270 * VertexPTNTaH::Size);

		VectorView<VertexPTNTaH> vertices(debugSphereMesh.m_vertices);

		debugSphereMesh.m_indices.resize(540 * sizeof(uint32));
		VectorView<uint32> indices(debugSphereMesh.m_indices);

		auto position = [&](size_t index) -> Vector3&
		{
			return vertices[index].Get<PositionXYZ, Vector3>();
		};

		uint64 currentIndex = 0;

		// deltaPhi = 0.0698138f = 2 * Pi / 90
		float cosDeltaPhi = cosf(0.0698138f);
		float sinDeltaPhi = sinf(0.0698138f);

		float cosV = 1.0f;
		float sinV = 0.0f;

		for (int i = 0; i < 89; ++i)
		{
			position(i) = radius * Vector3(0.0f, cosV, sinV);
			position(i + 90) = radius * Vector3(cosV, 0.0f, sinV);
			position(i + 180) = radius * Vector3(cosV, sinV, 0.0f);

			indices[currentIndex] = i; 
			indices[currentIndex + 180] = i + 90; 
			indices[currentIndex + 360] = i + 180;

			currentIndex++;

			indices[currentIndex] = i + 1;
			indices[currentIndex + 180] = i + 91;
			indices[currentIndex + 360] = i + 181;

			float cosV_tmp = cosV * cosDeltaPhi - sinV * sinDeltaPhi;
			float sinV_tmp = sinV * cosDeltaPhi + cosV * sinDeltaPhi;

			cosV = cosV_tmp;
			sinV = sinV_tmp;

			currentIndex++;
		}

		position(89) = radius * Vector3(0.0f, cosV, sinV);
		position(179) = radius * Vector3(cosV, 0.0f, sinV);
		position(269) = radius * Vector3(cosV, sinV, 0.0f);

		indices[currentIndex] = 89; 
		indices[currentIndex + 180] = 179; 
		indices[currentIndex + 360] = 269;

		currentIndex++;

		indices[currentIndex] = 0;
		indices[currentIndex + 180] = 90;
		indices[currentIndex + 360] = 180;

		return debugSphereMesh;
	}

	Mesh Mesh::DebugCapsule::Create(float radius, float height)
	{
		Mesh debugCapsuleMesh;

		debugCapsuleMesh.m_vertices.resize(364 * VertexPTNTaH::Size);

		VectorView<VertexPTNTaH> vertices(debugCapsuleMesh.m_vertices);

		debugCapsuleMesh.m_indices.resize(728 * sizeof(uint32));
		VectorView<uint32> indices(debugCapsuleMesh.m_indices);

		auto position = [&](size_t index) -> Vector3&
		{
			return vertices[index].Get<PositionXYZ, Vector3>();
		};

		uint64 i = 0;
		uint64 currentIndex = 0;

		// deltaPhi = 0.0698138f = 2 * Pi / 90
		float cosDeltaPhi = cosf(0.0698138f);
		float sinDeltaPhi = sinf(0.0698138f);

		float cosV = 1.0f;
		float sinV = 0.0f;

		float cyliHeight = 0.5f * height;

		while (i < 45)
		{
			position(i) = Vector3(0.0f, radius * sinV + cyliHeight, radius * cosV);
			position(i + 92) = Vector3(radius * cosV, radius * sinV + cyliHeight, 0.0f);

			indices[currentIndex] = i;
			indices[currentIndex + 184] = i + 92;

			currentIndex++;

			indices[currentIndex] = i + 1;
			indices[currentIndex + 184] = i + 93;

			float cosV_tmp = cosV * cosDeltaPhi - sinV * sinDeltaPhi;
			float sinV_tmp = sinV * cosDeltaPhi + cosV * sinDeltaPhi;

			cosV = cosV_tmp;
			sinV = sinV_tmp;

			currentIndex++; i++;
		}

		position(i) = Vector3(0.0f, radius * sinV + cyliHeight, radius * cosV);
		position(i + 92) = Vector3(radius * cosV, radius * sinV + cyliHeight, 0.0f);

		indices[currentIndex] = i;
		indices[currentIndex + 184] = i + 92;

		currentIndex++;

		indices[currentIndex] = i + 1;
		indices[currentIndex + 184] = i + 93;

		currentIndex++; i++;

		while (i < 91)
		{
			position(i) = Vector3(0.0f, radius * sinV - cyliHeight, radius * cosV);
			position(i + 92) = Vector3(radius * cosV, radius * sinV - cyliHeight, 0.0f);

			indices[currentIndex] = i;
			indices[currentIndex + 184] = i + 92;

			currentIndex++;

			indices[currentIndex] = i + 1;
			indices[currentIndex + 184] = i + 93;

			float cosV_tmp = cosV * cosDeltaPhi - sinV * sinDeltaPhi;
			float sinV_tmp = sinV * cosDeltaPhi + cosV * sinDeltaPhi;

			cosV = cosV_tmp;
			sinV = sinV_tmp;

			currentIndex++; i++;
		}

		position(i) = Vector3(0.0f, radius * sinV - cyliHeight, radius * cosV);
		position(i + 92) = Vector3(radius * cosV, radius * sinV - cyliHeight, 0.0f);

		indices[currentIndex] = i;
		indices[currentIndex + 184] = i + 92;

		currentIndex++;

		indices[currentIndex] = 0;
		indices[currentIndex + 184] = 92;

		cosV = 1.0f;
		sinV = 0.0f;

		currentIndex = 368; i = 184;

		while (i < 273)
		{
			position(i) = Vector3(radius * cosV, cyliHeight, radius * sinV);
			position(i + 90) = Vector3(radius * cosV, -cyliHeight, radius * sinV);

			indices[currentIndex] = i;
			indices[currentIndex + 180] = i + 90;

			currentIndex++;

			indices[currentIndex] = i + 1;
			indices[currentIndex + 180] = i + 91;

			float cosV_tmp = cosV * cosDeltaPhi - sinV * sinDeltaPhi;
			float sinV_tmp = sinV * cosDeltaPhi + cosV * sinDeltaPhi;

			cosV = cosV_tmp;
			sinV = sinV_tmp;

			currentIndex++; i++;
		}

		position(i) = Vector3(radius * cosV, cyliHeight, radius * sinV);
		position(i + 90) = Vector3(radius * cosV, -cyliHeight, radius * sinV);

		indices[currentIndex] = i;
		indices[currentIndex + 180] = i + 90;

		currentIndex++;

		indices[currentIndex] = 184;
		indices[currentIndex + 180] = 274;

		return debugCapsuleMesh;
	}

	Mesh Mesh::Line::Create(float scale)
	{
		Mesh lineMesh;

		lineMesh.vertexLayout = &VertexPTNTaH::Layout;
		lineMesh.indexType = vk::IndexType::eUint16;

		lineMesh.m_vertices.resize(2 * VertexPTNTaH::Size);

		VectorView<VertexPTNTaH> vertices(lineMesh.m_vertices);

		auto position = [&](size_t index) -> Vector3&
		{
			return vertices[index].Get<PositionXYZ, Vector3>();
		};

		position(0) = Vector3(0.0f, 0.0f, 0.0f);
		position(1) = Vector3(0.0f, 0.0f, scale);

		lineMesh.m_indices.resize(2 * sizeof(uint16));
		VectorView<uint16> indices(lineMesh.m_indices);

		indices[0] = 0; indices[1] = 1;

		return lineMesh;
	}

	Mesh Mesh::Line::Create(const Vector3& begin, const Vector3& end)
	{
		Mesh lineMesh;

		lineMesh.vertexLayout = &VertexPTNTaH::Layout;
		lineMesh.indexType = vk::IndexType::eUint16;

		lineMesh.m_vertices.resize(2 * VertexPTNTaH::Size);

		VectorView<VertexPTNTaH> vertices(lineMesh.m_vertices);

		auto position = [&](size_t index) -> Vector3&
		{
			return vertices[index].Get<PositionXYZ, Vector3>();
		};

		position(0) = begin;
		position(1) = end;

		lineMesh.m_indices.resize(2 * sizeof(uint16));
		VectorView<uint16> indices(lineMesh.m_indices);

		indices[0] = 0; indices[1] = 1;

		return lineMesh;
	}
}