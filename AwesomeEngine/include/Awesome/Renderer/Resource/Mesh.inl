namespace aw
{
	Mesh::~Mesh()
	{
		s_poolMutex.lock();
		m_flushCommandBuffer.reset();
		s_poolMutex.unlock();
	}

	inline const uint64 Mesh::GetVertexCount() const
	{
		if (vertexLayout == nullptr)
			return ~0u;

		return m_vertices.size() / vertexLayout->GetSize();
	}

	inline void Mesh::SetVertices(std::vector<uint8> newVertices)
	{
		m_vertices = std::move(newVertices);
		m_isCoherent = false;
	}

	inline const std::vector<uint8>& Mesh::GetVertices() const
	{
		return m_vertices;
	}

	inline std::vector<Vector3> Mesh::GetInterpretedPositions() const
	{
		std::vector<Vector3> poss;

		if (vertexLayout)
		{
			uint32 posOffset = vertexLayout->GetAttributeOffset(VertexAttribute(PositionXYZ));

			if (posOffset != ~0u)
			{
				uint32 vertexDataNb = m_vertices.size();
				uint32 vertexDataSize = vertexLayout->GetSize();
				uint32 vertexNb = vertexDataNb / vertexDataSize;
				poss.reserve(vertexNb);

				for (uint32 i = 0; i < vertexNb; ++i)
					poss.push_back(reinterpret_cast<const Vector3&>(m_vertices[i * vertexDataSize + posOffset]));
			}
		}

		return poss;
	}

	inline std::vector<uint8>& Mesh::Vertices()
	{
		return m_vertices;
	}

	inline void Mesh::SetIndices(std::vector<uint8> newIndices)
	{
		m_indices = std::move(newIndices);
		m_isCoherent = false;
	}

	inline const std::vector<uint8>& Mesh::GetIndices() const
	{
		return m_indices;
	}

	inline const std::vector<uint32>& Mesh::GetInterpretedIndices() const
	{
		return reinterpret_cast<const std::vector<uint32>&>(m_indices);
	}

	inline void Mesh::SetVerticesPosition(uint32 vertexDataSize, uint32 positionOffset, std::vector<Vector3> verticesPositions)
	{
		if (positionOffset == ~0u)
			return;

		uint32 vertexNb = verticesPositions.size();

		m_vertices.resize(vertexNb * vertexDataSize);

		for (uint32 v = 0; v < vertexNb; ++v)
		{
			std::swap(reinterpret_cast<Vector3&>(m_vertices[v * vertexDataSize + positionOffset]), verticesPositions[v]);
		}

		m_isCoherent = false;
	}

	void Mesh::ReverseFaces()
	{
		uint8 gap = 3 * sizeof(uint32);
		for (uint64 i = 0; i < m_indices.size(); i += gap)
			std::swap(reinterpret_cast<uint32&>(m_indices[i]), reinterpret_cast<uint32&>(m_indices[i + 2 * sizeof(uint32)]));

		if (vertexLayout)
		{
			uint32 normalOffset = vertexLayout->GetAttributeOffset(VertexAttribute(NormalXYZ));

			if (normalOffset != ~0u)
			{
				uint32 vertexDataSize = vertexLayout->GetSize();
				uint32 vertexDataNb = m_vertices.size();
				uint32 vertexNb = vertexDataNb / vertexDataSize;

				for (uint32 i = 0; i < vertexNb; ++i)
				{
					Vector3& normal = reinterpret_cast<Vector3&>(m_vertices[normalOffset + i * vertexDataSize]);
					normal = -normal;
				}
			}
		}
	}

	inline void Mesh::Draw(vk::CommandBuffer commandBuffer) const
	{
		vk::Buffer buffer = m_vertexSubBuffer.buffer->Get();
		commandBuffer.bindVertexBuffers(0, 1, &buffer, &m_vertexSubBuffer.offset);
		commandBuffer.bindIndexBuffer(m_indexSubBuffer.buffer->Get(), m_indexSubBuffer.offset, indexType);
		if (indexType == vk::IndexType::eUint16)
			commandBuffer.drawIndexed(uint32(m_indices.size() / sizeof(uint16)), 1, 0, 0, 0);
		else
			commandBuffer.drawIndexed(uint32(m_indices.size() / sizeof(uint32)), 1, 0, 0, 0);
	}

	inline bool Mesh::IsCoherent() const
	{
		return m_isCoherent;
	}

	template<uint8 precision>
	inline Mesh Mesh::Sphere<precision>::CreateUVSphere(float radius)
	{
		Mesh uvSphere;

		uint32 longitude = (1 << (precision + 2));
		uint32 latitude = (1 << (precision + 1)) - 1;
		uint64 vertexNb = 2 * longitude + latitude * (longitude + 1);

		uvSphere.m_vertices.resize(vertexNb * VertexPTNTaH::Size);
		uvSphere.m_indices.resize((6 * longitude * latitude) * sizeof(uint32));

		VectorView<VertexPTNTaH> vertices(uvSphere.m_vertices);
		VectorView<uint32> indices(uvSphere.m_indices);

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

		uint64 vertexId = 0;
		uint64 index = 0;

		uint16 n = 0;
		float r = 1.0f / static_cast<float>(longitude);
		float u0 = 0.5f * r;

		float deltaPhi = M_2PI * r;
		float cosDeltaPhi = cosf(deltaPhi);
		float sinDeltaPhi = sinf(deltaPhi);

		float deltaTheta = M_PI / static_cast<float>(latitude + 1) ;
		float cosDeltaTheta = cosf(deltaTheta);
		float sinDeltaTheta = sinf(deltaTheta);

		float cosPhi = cosDeltaPhi;
		float sinPhi = sinDeltaPhi;

		float cosTheta = cosDeltaTheta;
		float sinTheta = sinDeltaTheta;

		// Seam points
		{
			for (int i = 1; i < latitude; ++i)
			{
				Vector3 d(-sinTheta, cosTheta, 0.0f);
				position(vertexId) = radius * d;
				normal(vertexId) = d;
				uv(vertexId) = Vector2(0.0f, 0.5f - asinf(d.y) / M_PI);

				float cosTheta_tmp = cosTheta * cosDeltaTheta - sinTheta * sinDeltaTheta;
				float sinTheta_tmp = sinTheta * cosDeltaTheta + cosTheta * sinDeltaTheta;

				cosTheta = cosTheta_tmp;
				sinTheta = sinTheta_tmp;

				vertexId++;
			}

			Vector3 d(-sinTheta, cosTheta, 0.0f);
			position(vertexId) = radius * d;
			normal(vertexId) = d;
			uv(vertexId) = Vector2(0.0f, 0.5f - asinf(d.y) / M_PI);

			vertexId++;
		}

		for (int i = 0; i < longitude; ++i)
		{
			cosTheta = cosDeltaTheta;
			sinTheta = sinDeltaTheta;

			float u = (n + 1) * r;

			// North Pole
			{
				position(vertexId) = radius * Vector3::Up();
				normal(vertexId) = Vector3::Up();
				uv(vertexId) = Vector2(u0 + n * r, 0.0f);

				indices[index] = vertexId; index++;
				indices[index] = vertexId - latitude; index++;
				indices[index] = vertexId + 2; index++;

				vertexId++;
			}

			// South Pole
			{
				position(vertexId) = radius * Vector3::Down();
				normal(vertexId) = Vector3::Down();
				uv(vertexId) = Vector2(u0 + n * r, 1.0f);

				indices[index] = vertexId; index++;
				indices[index] = vertexId + latitude; index++;
				indices[index] = vertexId - 2; index++;

				vertexId++;
			}

			for (int j = 1; j < latitude; ++j)
			{
				Vector3 d(-sinTheta * cosPhi, cosTheta, sinTheta * sinPhi);
				position(vertexId) = radius * d;
				normal(vertexId) = d;
				uv(vertexId) = Vector2(u, 0.5f - asinf(d.y) / M_PI);

				indices[index] = vertexId - (latitude + 2); index++;
				indices[index] = vertexId + 1; index++;
				indices[index] = vertexId; index++;

				indices[index] = vertexId - (latitude + 2); index++;
				indices[index] = vertexId - (latitude + 1); index++;
				indices[index] = vertexId + 1; index++;

				float cosTheta_tmp = cosTheta * cosDeltaTheta - sinTheta * sinDeltaTheta;
				float sinTheta_tmp = sinTheta * cosDeltaTheta + cosTheta * sinDeltaTheta;

				cosTheta = cosTheta_tmp;
				sinTheta = sinTheta_tmp;

				vertexId++;
			}

			Vector3 d(-sinTheta * cosPhi, cosTheta, sinTheta * sinPhi);
			position(vertexId) = radius * d;
			normal(vertexId) = d;
			uv(vertexId) = Vector2(u, 0.5f - asinf(d.y) / M_PI);

			vertexId++;

			float cosTheta_tmp = cosPhi * cosDeltaPhi - sinPhi * sinDeltaPhi;
			float sinTheta_tmp = sinPhi * cosDeltaPhi + cosPhi * sinDeltaPhi;

			cosPhi = cosTheta_tmp;
			sinPhi = sinTheta_tmp;

			n++;
		}

		uvSphere.GenerateTangents();

		return uvSphere;
	}

	template<uint8 precision>
	inline Mesh Mesh::Hemisphere<precision>::Create(float radius)
	{
		Mesh uvHemisphere;

		uint32 longitude = 1 << (precision + 2);
		uint32 latitude = 1 << precision;
		uint64 vertexNb = longitude + latitude * (longitude + 1);

		uvHemisphere.m_vertices.resize(vertexNb * VertexPTNTaH::Size);
		uvHemisphere.m_indices.resize((3 * longitude * (2 * latitude - 1)) * sizeof(uint32));

		VectorView<VertexPTNTaH> vertices(uvHemisphere.m_vertices);
		VectorView<uint32> indices(uvHemisphere.m_indices);

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

		uint64 vertexId = 0;
		uint64 index = 0;

		uint16 n = 0;
		float r = 1.0f / static_cast<float>(longitude);
		float u0 = 0.5f * r;

		float deltaPhi = M_2PI * r;
		float cosDeltaPhi = cosf(deltaPhi);
		float sinDeltaPhi = sinf(deltaPhi);

		float deltaTheta = M_PI2 / static_cast<float>(latitude);
		float cosDeltaTheta = cosf(deltaTheta);
		float sinDeltaTheta = sinf(deltaTheta);

		float cosPhi = cosDeltaPhi;
		float sinPhi = sinDeltaPhi;

		float cosTheta = cosDeltaTheta;
		float sinTheta = sinDeltaTheta;

		// Seam points
		{
			for (int i = 1; i < latitude; ++i)
			{
				Vector3 d(-sinTheta, cosTheta, 0.0f);
				position(vertexId) = radius * d;
				normal(vertexId) = d;
				uv(vertexId) = Vector2(0.0f, 0.5f - asinf(d.y) / M_PI);

				float cosTheta_tmp = cosTheta * cosDeltaTheta - sinTheta * sinDeltaTheta;
				float sinTheta_tmp = sinTheta * cosDeltaTheta + cosTheta * sinDeltaTheta;

				cosTheta = cosTheta_tmp;
				sinTheta = sinTheta_tmp;

				vertexId++;
			}

			position(vertexId) = radius * Vector3::Left();
			normal(vertexId) = Vector3::Left();
			uv(vertexId) = Vector2(0.0f, 0.5f);

			vertexId++;
		}

		for (int i = 0; i < longitude; ++i)
		{
			cosTheta = cosDeltaTheta;
			sinTheta = sinDeltaTheta;

			float u = (n + 1) * r;

			// Pole
			{
				position(vertexId) = radius * Vector3::Up();
				normal(vertexId) = Vector3::Up();
				uv(vertexId) = Vector2(u0 + n * r, 0.0f);

				indices[index] = vertexId; index++;
				indices[index] = vertexId - latitude; index++;
				indices[index] = vertexId + 1; index++;

				vertexId++;
			}

			for (int j = 1; j < latitude; ++j)
			{
				Vector3 d(-sinTheta * cosPhi, cosTheta, sinTheta * sinPhi);
				position(vertexId) = radius * d;
				normal(vertexId) = d;
				uv(vertexId) = Vector2(u, 0.5f - asinf(d.y) / M_PI);

				indices[index] = vertexId - (latitude + 1); index++;
				indices[index] = vertexId + 1; index++;
				indices[index] = vertexId; index++;

				indices[index] = vertexId - (latitude + 1); index++;
				indices[index] = vertexId - latitude; index++;
				indices[index] = vertexId + 1; index++;

				float cosTheta_tmp = cosTheta * cosDeltaTheta - sinTheta * sinDeltaTheta;
				float sinTheta_tmp = sinTheta * cosDeltaTheta + cosTheta * sinDeltaTheta;

				cosTheta = cosTheta_tmp;
				sinTheta = sinTheta_tmp;

				vertexId++;
			}

			Vector3 d(-cosPhi, 0.0f, sinPhi);
			position(vertexId) = radius * d;
			normal(vertexId) = d;
			uv(vertexId) = Vector2(u, 0.5f);

			vertexId++;

			float cosTheta_tmp = cosPhi * cosDeltaPhi - sinPhi * sinDeltaPhi;
			float sinTheta_tmp = sinPhi * cosDeltaPhi + cosPhi * sinDeltaPhi;

			cosPhi = cosTheta_tmp;
			sinPhi = sinTheta_tmp;

			n++;
		}

		uvHemisphere.GenerateTangents();

		return uvHemisphere;
	}

	template<uint8 precision, Mesh::NormalMode normalMode>
	inline Mesh Mesh::Cone<precision, normalMode>::Create(float radius, float height)
	{
		Mesh coneMesh;

		if constexpr (normalMode == NormalMode::PerFace)
		{
			uint32 longitude = (1 << (precision + 2));
			uint64 vertexNb = 6 * longitude;

			coneMesh.m_vertices.resize(vertexNb * VertexPTNTaH::Size);
			coneMesh.m_indices.resize(vertexNb * sizeof(uint32));

			VectorView<VertexPTNTaH> vertices(coneMesh.m_vertices);
			VectorView<uint32> indices(coneMesh.m_indices);

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

			uint64 vertexId = 0;

			uint16 n = 0;
			float r = 1.0f / static_cast<float>(longitude);
			float uTop0 = 0.5f * r;
			float uBase0 = 0.0f;

			float hypothenuse = sqrtf(radius * radius + height * height);
			float cosAlpha = fabs(height) / hypothenuse;
			float sinAlpha = Sign(height) * fabs(radius) / hypothenuse;

			float deltaPhi = M_2PI * r;
			float cosDeltaPhi = cosf(deltaPhi);
			float sinDeltaPhi = sinf(deltaPhi);
			float norm = 2.0f * cosf(deltaPhi * 0.5f);

			float cosV1 = 1.0f;
			float sinV1 = 0.0f;

			float cosV2 = cosDeltaPhi;
			float sinV2 = sinDeltaPhi;

			float topHeight = height * 0.5f;

			Vector3 baseNormal = Sign(height) * Vector3::Down();

			for (int i = 0; i < longitude; ++i)
			{
				float x = -(cosV1 + cosV2) / norm;
				float z = (sinV1 + sinV2) / norm;
				Vector3 faceNormal(x * cosAlpha, -x * sinAlpha, z);

				///_________________________________________________________________________________

				position(vertexId) = Vector3(0.0f, topHeight, 0.0f);
				normal(vertexId) = faceNormal;
				uv(vertexId) = Vector2(uTop0 + n * r, 0.0f);
				indices[vertexId] = vertexId;
				vertexId++;

				position(vertexId) = Vector3(-radius * cosV1, -topHeight, radius * sinV1);
				normal(vertexId) = faceNormal;
				uv(vertexId) = Vector2(uBase0 + n * r, 1.0f);
				indices[vertexId] = vertexId;
				vertexId++;

				position(vertexId) = Vector3(-radius * cosV2, -topHeight, radius * sinV2);
				normal(vertexId) = faceNormal;
				uv(vertexId) = Vector2(uBase0 + (n + 1) * r, 1.0f);
				indices[vertexId] = vertexId;
				vertexId++;

				///_________________________________________________________________________________

				position(vertexId) = Vector3(0.0f, -topHeight, 0.0f);
				normal(vertexId) = baseNormal;
				uv(vertexId) = Vector2(uTop0 + n * r, 0.0f);
				indices[vertexId] = vertexId;
				vertexId++;

				position(vertexId) = position(vertexId - 3);
				normal(vertexId) = baseNormal;
				uv(vertexId) = uv(vertexId - 3);
				indices[vertexId] = vertexId;
				vertexId++;

				position(vertexId) = position(vertexId - 3);
				normal(vertexId) = baseNormal;
				uv(vertexId) = uv(vertexId - 3);
				indices[vertexId] = vertexId;
				vertexId++;

				///_________________________________________________________________________________

				float cosV1_tmp = cosV1 * cosDeltaPhi - sinV1 * sinDeltaPhi;
				float sinV1_tmp = sinV1 * cosDeltaPhi + cosV1 * sinDeltaPhi;
				float cosV2_tmp = cosV2 * cosDeltaPhi - sinV2 * sinDeltaPhi;
				float sinV2_tmp = sinV2 * cosDeltaPhi + cosV2 * sinDeltaPhi;

				cosV1 = cosV1_tmp;
				sinV1 = sinV1_tmp;
				cosV2 = cosV2_tmp;
				sinV2 = sinV2_tmp;

				n++;
			}
		}
		else if (normalMode == NormalMode::Smooth)
		{
			uint32 longitude = (1 << (precision + 2));
			uint64 vertexNb = 4 * longitude + 2;

			coneMesh.m_vertices.resize(vertexNb * VertexPTNTaH::Size);
			coneMesh.m_indices.resize(6 * longitude * sizeof(uint32));

			VectorView<VertexPTNTaH> vertices(coneMesh.m_vertices);
			VectorView<uint32> indices(coneMesh.m_indices);

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

			uint64 vertexId = 0;
			uint64 index = 0;

			uint16 n = 0;
			float r = 1.0f / static_cast<float>(longitude);
			float uTop0 = 0.5f * r;
			float uBase0 = 0.0f;

			float hypothenuse = sqrtf(radius * radius + height * height);
			float cosAlpha = fabs(height) / hypothenuse;
			float sinAlpha = Sign(height) * fabs(radius) / hypothenuse;

			float deltaPhi = M_2PI * r;
			float cosDeltaPhi = cosf(deltaPhi);
			float sinDeltaPhi = sinf(deltaPhi);

			float cosV = 1.0f;
			float sinV = 0.0f;

			float topHeight = height * 0.5f;

			Vector3 baseNormal = Sign(height) * Vector3::Down();
			Vector3 topNormal = Sign(height) * Vector3::Up();

			// Seam points
			{
				position(vertexId) = Vector3(-radius * cosV, -topHeight, radius * sinV);
				normal(vertexId) = Vector3(-cosAlpha * cosV, sinAlpha * cosV, sinV);
				uv(vertexId) = Vector2(0.0f, 1.0f);
				vertexId++;

				position(vertexId) = position(vertexId - 1);
				normal(vertexId) = baseNormal;
				uv(vertexId) = uv(vertexId - 1);
				vertexId++;

				///_________________________________________________________________________________

				float cosV_tmp = cosV * cosDeltaPhi - sinV * sinDeltaPhi;
				float sinV_tmp = sinV * cosDeltaPhi + cosV * sinDeltaPhi;

				cosV = cosV_tmp;
				sinV = sinV_tmp;
			}

			for (int i = 0; i < longitude; ++i)
			{
				Vector2 centerUV(uTop0 + n * r, 0.0f);

				position(vertexId) = Vector3(0.0f, topHeight, 0.0f);
				normal(vertexId) = topNormal;
				uv(vertexId) = centerUV;
				vertexId++;

				position(vertexId) = Vector3(0.0f, -topHeight, 0.0f);
				normal(vertexId) = baseNormal;
				uv(vertexId) = centerUV;
				vertexId++;

				position(vertexId) = Vector3(-radius * cosV, -topHeight, radius * sinV);
				normal(vertexId) = Vector3(-cosAlpha * cosV, sinAlpha * cosV, sinV);
				uv(vertexId) = Vector2(uBase0 + (n + 1) * r, 1.0f);
				vertexId++;

				position(vertexId) = position(vertexId - 1);
				normal(vertexId) = baseNormal;
				uv(vertexId) = uv(vertexId - 1);
				vertexId++;

				indices[index] = vertexId - 6; index++;
				indices[index] = vertexId - 2; index++;
				indices[index] = vertexId - 4; index++;

				indices[index] = vertexId - 5; index++;
				indices[index] = vertexId - 3; index++;
				indices[index] = vertexId - 1; index++;

				float cosV_tmp = cosV * cosDeltaPhi - sinV * sinDeltaPhi;
				float sinV_tmp = sinV * cosDeltaPhi + cosV * sinDeltaPhi;

				cosV = cosV_tmp;
				sinV = sinV_tmp;

				n++;
			}
		}

		coneMesh.GenerateTangents();

		return coneMesh;
	}

	template<uint8 precision, Mesh::NormalMode normalMode>
	inline Mesh Mesh::Cylindroid<precision, normalMode>::Create(float radius, float radius2, float height)
	{
		Mesh cylindroidMesh;

		if constexpr (normalMode == Mesh::NormalMode::PerFace)
		{
			uint32 longitude = (1 << (precision + 2));
			uint64 vertexNb = 10 * longitude;

			cylindroidMesh.m_vertices.resize(vertexNb * VertexPTNTaH::Size);
			cylindroidMesh.m_indices.resize(12 * longitude * sizeof(uint32));

			VectorView<VertexPTNTaH> vertices(cylindroidMesh.m_vertices);
			VectorView<uint32> indices(cylindroidMesh.m_indices);

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

			uint64 vertexId = 0;
			uint64 index = 0;

			uint32 n = 0;
			float r = 1.0f / static_cast<float>(longitude);
			float uTop0 = 0.5f * r;
			float uBase0 = 0.0f;

			float base = fabs(radius2 - radius);
			float hypothenuse = sqrtf(base * base + height * height);
			float cosAlpha = fabs(height) / hypothenuse;
			float sinAlpha = fabs(radius) / hypothenuse;

			float deltaPhi = M_2PI * r;
			float cosDeltaPhi = cosf(deltaPhi);
			float sinDeltaPhi = sinf(deltaPhi);
			float norm = 2.0f * cosf(deltaPhi * 0.5f);

			float cosV1 = 1.0f;
			float sinV1 = 0.0f;

			float cosV2 = cosDeltaPhi;
			float sinV2 = sinDeltaPhi;

			float topHeight = height * 0.5f;

			for (uint32 i = 0; i < longitude; ++i)
			{
				float x = -(cosV1 + cosV2) / norm;
				float z = (sinV1 + sinV2) / norm;
				Vector3 faceNormal(x * cosAlpha, -x * sinAlpha, z);

				///_________________________________________________________________________________

				position(vertexId) = Vector3(0.0f, topHeight, 0.0f);
				normal(vertexId) = Vector3::Up();
				uv(vertexId) = Vector2(uTop0 + n * r, 1.0f);
				indices[index] = vertexId;
				vertexId++; index++;

				position(vertexId) = Vector3(-radius * cosV1, topHeight, radius * sinV1);
				normal(vertexId) = Vector3::Up();
				uv(vertexId) = Vector2(uBase0 + n * r, 0.0f);
				indices[index] = vertexId;
				vertexId++; index++;

				position(vertexId) = Vector3(-radius * cosV2, topHeight, radius * sinV2);
				normal(vertexId) = Vector3::Up();
				uv(vertexId) = Vector2(uBase0 + (n + 1) * r, 0.0f);
				indices[index] = vertexId;
				vertexId++; index++;

				///_________________________________________________________________________________

				position(vertexId) = position(vertexId - 2);
				normal(vertexId) = faceNormal;
				uv(vertexId) = uv(vertexId - 2);
				indices[index] = vertexId;		index++;
				indices[index] = vertexId + 2;	index++;
				indices[index] = vertexId + 3;	index++;
				vertexId++;

				position(vertexId) = position(vertexId - 2);
				normal(vertexId) = faceNormal;
				uv(vertexId) = uv(vertexId - 2);
				indices[index] = vertexId;		index++;
				indices[index] = vertexId - 1;	index++;
				indices[index] = vertexId + 2;	index++;
				vertexId++;

				///_________________________________________________________________________________

				position(vertexId) = Vector3(-radius2 * cosV1, -topHeight, radius2 * sinV1);
				normal(vertexId) = faceNormal;
				uv(vertexId) = Vector2(uBase0 + n * r, 1.0f);
				vertexId++;

				position(vertexId) = Vector3(-radius2 * cosV2, -topHeight, radius2 * sinV2);
				normal(vertexId) = faceNormal;
				uv(vertexId) = Vector2(uBase0 + (n + 1) * r, 1.0f);
				vertexId++;

				///_________________________________________________________________________________

				position(vertexId) = position(vertexId - 2);
				normal(vertexId) = Vector3::Down();
				uv(vertexId) = uv(vertexId - 2);
				indices[index] = vertexId;
				vertexId++; index++;

				position(vertexId) = position(vertexId - 2);
				normal(vertexId) = Vector3::Down();
				uv(vertexId) = uv(vertexId - 2);
				indices[index] = vertexId;
				vertexId++; index++;

				position(vertexId) = Vector3(0.0f, -topHeight, 0.0f);;
				normal(vertexId) = Vector3::Down();
				uv(vertexId) = Vector2(uTop0 + n * r, 0.0f);
				indices[index] = vertexId;
				vertexId++; index++;

				///_________________________________________________________________________________

				float cosV1_tmp = cosV1 * cosDeltaPhi - sinV1 * sinDeltaPhi;
				float sinV1_tmp = sinV1 * cosDeltaPhi + cosV1 * sinDeltaPhi;
				float cosV2_tmp = cosV2 * cosDeltaPhi - sinV2 * sinDeltaPhi;
				float sinV2_tmp = sinV2 * cosDeltaPhi + cosV2 * sinDeltaPhi;

				cosV1 = cosV1_tmp;
				sinV1 = sinV1_tmp;
				cosV2 = cosV2_tmp;
				sinV2 = sinV2_tmp;

				n++;
			}
		}
		else if (normalMode == NormalMode::Smooth)
		{
			uint32 longitude = (1 << (precision + 2));
			uint64 vertexNb = 6 * longitude + 4;

			cylindroidMesh.m_vertices.resize(vertexNb * VertexPTNTaH::Size);
			cylindroidMesh.m_indices.resize(12 * longitude * sizeof(uint32));

			VectorView<VertexPTNTaH> vertices(cylindroidMesh.m_vertices);
			VectorView<uint32> indices(cylindroidMesh.m_indices);

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

			uint64 vertexId = 0;
			uint64 index = 0;

			uint16 n = 0;
			float r = 1.0f / static_cast<float>(longitude);
			float uTop0 = 0.5f * r;
			float uBase0 = 0.0f;

			float base = fabs(radius2 - radius);
			float hypothenuse = sqrtf(base * base + height * height);
			float cosAlpha = fabs(height) / hypothenuse;
			float sinAlpha = Sign(height) * fabs(radius) / hypothenuse;

			float deltaPhi = M_2PI * r;
			float cosDeltaPhi = cosf(deltaPhi);
			float sinDeltaPhi = sinf(deltaPhi);

			float cosV = 1.0f;
			float sinV = 0.0f;

			float topHeight = height * 0.5f;

			Vector3 baseNormal = Sign(height) * Vector3::Down();
			Vector3 topNormal = Sign(height) * Vector3::Up();

			// Seam points
			{
				Vector3 sideNormal(-cosAlpha * cosV, sinAlpha * cosV, sinV);

				position(vertexId) = Vector3(-radius * cosV, topHeight, radius * sinV);
				normal(vertexId) = topNormal;
				uv(vertexId) = Vector2(0.0f, 0.0f);
				vertexId++;

				position(vertexId) = position(vertexId - 1);
				normal(vertexId) = sideNormal;
				uv(vertexId) = uv(vertexId - 1);
				vertexId++;

				position(vertexId) = Vector3(-radius2 * cosV, -topHeight, radius2 * sinV);
				normal(vertexId) = sideNormal;
				uv(vertexId) = Vector2(0.0f, 1.0f);
				vertexId++;

				position(vertexId) = position(vertexId - 1);
				normal(vertexId) = baseNormal;
				uv(vertexId) = uv(vertexId - 1);
				vertexId++;

				///_________________________________________________________________________________

				float cosV_tmp = cosV * cosDeltaPhi - sinV * sinDeltaPhi;
				float sinV_tmp = sinV * cosDeltaPhi + cosV * sinDeltaPhi;

				cosV = cosV_tmp;
				sinV = sinV_tmp;
			}

			for (int i = 0; i < longitude; ++i)
			{
				float uT = uTop0 + n * r;
				float uB = uBase0 + (n + 1) * r;
				Vector3 sideNormal(-cosAlpha * cosV, sinAlpha * cosV, sinV);

				position(vertexId) = Vector3(0.0f, topHeight, 0.0f);
				normal(vertexId) = topNormal;
				uv(vertexId) = Vector2(uT, 1.0f);
				vertexId++;

				position(vertexId) = Vector3(0.0f, -topHeight, 0.0f);
				normal(vertexId) = baseNormal;
				uv(vertexId) = Vector2(uT, 0.0f);
				vertexId++;

				position(vertexId) = Vector3(-radius * cosV, topHeight, radius * sinV);
				normal(vertexId) = topNormal;
				uv(vertexId) = Vector2(uB, 0.0f);
				vertexId++;

				position(vertexId) = position(vertexId - 1);
				normal(vertexId) = sideNormal;
				uv(vertexId) = uv(vertexId - 1);
				vertexId++;

				position(vertexId) = Vector3(-radius2 * cosV, -topHeight, radius2 * sinV);
				normal(vertexId) = sideNormal;
				uv(vertexId) = Vector2(uB, 1.0f);
				vertexId++;

				position(vertexId) = position(vertexId - 1);
				normal(vertexId) = baseNormal;
				uv(vertexId) = uv(vertexId - 1);
				vertexId++;

				indices[index] = vertexId - 10; index++;
				indices[index] = vertexId - 4; index++;
				indices[index] = vertexId - 6; index++;

				indices[index] = vertexId - 9; index++;
				indices[index] = vertexId - 8; index++;
				indices[index] = vertexId - 2; index++;

				indices[index] = vertexId - 9; index++;
				indices[index] = vertexId - 2; index++;
				indices[index] = vertexId - 3; index++;

				indices[index] = vertexId - 7; index++;
				indices[index] = vertexId - 5; index++;
				indices[index] = vertexId - 1; index++;

				float cosV_tmp = cosV * cosDeltaPhi - sinV * sinDeltaPhi;
				float sinV_tmp = sinV * cosDeltaPhi + cosV * sinDeltaPhi;

				cosV = cosV_tmp;
				sinV = sinV_tmp;

				n++;
			}
		}

		cylindroidMesh.GenerateTangents();

		return cylindroidMesh;
	}

	template<uint8 precision>
	inline Mesh Mesh::Cylinder<precision>::Create(float radius, float height)
	{
		return Mesh::Cylindroid<precision, Mesh::NormalMode::Smooth>::Create(radius, radius, height);
	}

	template<uint8 precision>
	inline Mesh Mesh::Capsule<precision>::Create(float radius, float height)
	{
		Mesh capsuleMesh;

		uint32 longitude = 1 << (precision + 2);
		uint32 latitude = 1 << precision;

		capsuleMesh.m_vertices.resize(2 * (longitude + latitude * (longitude + 1)) * VertexPTNTaH::Size);
		capsuleMesh.m_indices.resize(12 * longitude * latitude * sizeof(uint32));

		VectorView<VertexPTNTaH> vertices(capsuleMesh.m_vertices);
		VectorView<uint32> indices(capsuleMesh.m_indices);

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

		uint64 vertexId = 0;
		uint64 index = 0;
		uint64 gapLon = 2 * latitude + 2;
		uint64 gapLat = 2 * latitude;

		uint16 n = 0;
		float r = 1.0f / static_cast<float>(longitude);
		float u0 = 0.5f * r;
		float vRatio = 1.0f / (2.0f * fabs(radius) + fabs(height));

		float deltaPhi = M_2PI * r;
		float cosDeltaPhi = cosf(deltaPhi);
		float sinDeltaPhi = sinf(deltaPhi);

		float deltaTheta = M_PI / static_cast<float>(2 * latitude);
		float cosDeltaTheta = cosf(deltaTheta);
		float sinDeltaTheta = sinf(deltaTheta);

		float cosPhi = cosDeltaPhi;
		float sinPhi = sinDeltaPhi;

		float cosTheta = cosDeltaTheta;
		float sinTheta = sinDeltaTheta;

		float topCylindreHeight = 0.5f * height;
		Vector3 top(0.0f, radius + height * 0.5f, 0.0f);
		Vector3 bottom(0.0f, -(radius + height * 0.5f), 0.0f);

		// Seam points
		{
			// First Hemi
			{
				for (int i = 1; i < latitude; ++i)
				{
					Vector3 pos(radius * -sinTheta, radius * cosTheta + topCylindreHeight, 0.0f);
					position(vertexId) = pos;
					normal(vertexId) = Vector3(-sinTheta, cosTheta, 0.0f);
					uv(vertexId) = Vector2(0.0f, 0.5f - pos.y * vRatio);

					float cosTheta_tmp = cosTheta * cosDeltaTheta - sinTheta * sinDeltaTheta;
					float sinTheta_tmp = sinTheta * cosDeltaTheta + cosTheta * sinDeltaTheta;

					cosTheta = cosTheta_tmp;
					sinTheta = sinTheta_tmp;

					vertexId++;
				}

				position(vertexId) = Vector3(-radius, topCylindreHeight, 0.0f);
				normal(vertexId) = Vector3::Left();
				uv(vertexId) = Vector2(0.0f, 0.5f - topCylindreHeight * vRatio);

				vertexId++;
			}

			// Second Hemi
			{
				for (int i = 1; i < latitude; ++i)
				{
					Vector3 pos(radius * -sinTheta, radius * cosTheta - topCylindreHeight, 0.0f);
					position(vertexId) = pos;
					normal(vertexId) = Vector3(-sinTheta, cosTheta, 0.0f);
					uv(vertexId) = Vector2(0.0f, 0.5f - pos.y * vRatio);

					float cosTheta_tmp = cosTheta * cosDeltaTheta - sinTheta * sinDeltaTheta;
					float sinTheta_tmp = sinTheta * cosDeltaTheta + cosTheta * sinDeltaTheta;

					cosTheta = cosTheta_tmp;
					sinTheta = sinTheta_tmp;

					vertexId++;
				}

				Vector3 pos(radius * -sinTheta, radius * cosTheta - topCylindreHeight, 0.0f);
				position(vertexId) = pos;
				normal(vertexId) = Vector3(-sinTheta, cosTheta, 0.0f);
				uv(vertexId) = Vector2(0.0f, 0.5f - pos.y * vRatio);

				vertexId++;
			}
		}

		for (int i = 0; i < longitude; ++i)
		{
			cosTheta = cosDeltaTheta;
			sinTheta = sinDeltaTheta;

			float u = (n + 1) * r;

			// North Pole
			{
				position(vertexId) = top;
				normal(vertexId) = Vector3::Up();
				uv(vertexId) = Vector2(u0 + n * r, 0.0f);

				indices[index] = vertexId; index++;
				indices[index] = vertexId - gapLat; index++;
				indices[index] = vertexId + 2; index++;

				vertexId++;
			}

			// South Pole
			{
				position(vertexId) = bottom;
				normal(vertexId) = Vector3::Down();
				uv(vertexId) = Vector2(u0 + n * r, 1.0f);

				indices[index] = vertexId; index++;
				indices[index] = vertexId + gapLat; index++;
				indices[index] = vertexId - 2; index++;

				vertexId++;
			}

			// First Hemi
			{
				for (int j = 1; j < latitude; ++j)
				{
					Vector3 d(-sinTheta * cosPhi, cosTheta, sinTheta * sinPhi);
					Vector3 pos = radius * d; pos.y = pos.y + topCylindreHeight;
					position(vertexId) = pos;
					normal(vertexId) = d;
					uv(vertexId) = Vector2(u, 0.5f - pos.y * vRatio);

					indices[index] = vertexId - gapLon; index++;
					indices[index] = vertexId + 1; index++;
					indices[index] = vertexId; index++;

					indices[index] = vertexId - gapLon; index++;
					indices[index] = vertexId - gapLon + 1; index++;
					indices[index] = vertexId + 1; index++;

					float cosTheta_tmp = cosTheta * cosDeltaTheta - sinTheta * sinDeltaTheta;
					float sinTheta_tmp = sinTheta * cosDeltaTheta + cosTheta * sinDeltaTheta;

					cosTheta = cosTheta_tmp;
					sinTheta = sinTheta_tmp;

					vertexId++;
				}

				Vector3 d(-cosPhi, 0.0f, sinPhi);
				Vector3 pos(-radius * cosPhi, topCylindreHeight, radius * sinPhi);
				position(vertexId) = pos;
				normal(vertexId) = d;
				uv(vertexId) = Vector2(u, 0.5f - pos.y * vRatio);

				indices[index] = vertexId - gapLon; index++;
				indices[index] = vertexId + 1; index++;
				indices[index] = vertexId; index++;

				indices[index] = vertexId - gapLon; index++;
				indices[index] = vertexId - gapLon + 1; index++;
				indices[index] = vertexId + 1; index++;

				vertexId++;
			}

			// Second Hemi
			{
				for (int j = 1; j < latitude; ++j)
				{
					Vector3 d(-sinTheta * cosPhi, cosTheta, sinTheta * sinPhi);
					Vector3 pos = radius * d; pos.y = pos.y - topCylindreHeight;
					position(vertexId) = pos;
					normal(vertexId) = d;
					uv(vertexId) = Vector2(u, 0.5f - pos.y * vRatio);

					indices[index] = vertexId - gapLon; index++;
					indices[index] = vertexId + 1; index++;
					indices[index] = vertexId; index++;

					indices[index] = vertexId - gapLon; index++;
					indices[index] = vertexId - gapLon + 1; index++;
					indices[index] = vertexId + 1; index++;

					float cosTheta_tmp = cosTheta * cosDeltaTheta - sinTheta * sinDeltaTheta;
					float sinTheta_tmp = sinTheta * cosDeltaTheta + cosTheta * sinDeltaTheta;

					cosTheta = cosTheta_tmp;
					sinTheta = sinTheta_tmp;

					vertexId++;
				}

				Vector3 d(-sinTheta * cosPhi, cosTheta, sinTheta * sinPhi);
				Vector3 pos = radius * d; pos.y = pos.y - topCylindreHeight;
				position(vertexId) = pos;
				normal(vertexId) = d;
				uv(vertexId) = Vector2(u, 0.5f - pos.y * vRatio);

				vertexId++;
			}

			float cosTheta_tmp = cosPhi * cosDeltaPhi - sinPhi * sinDeltaPhi;
			float sinTheta_tmp = sinPhi * cosDeltaPhi + cosPhi * sinDeltaPhi;

			cosPhi = cosTheta_tmp;
			sinPhi = sinTheta_tmp;

			n++;
		}

		capsuleMesh.GenerateTangents();

		return capsuleMesh;
	}

	template<uint8 rPrecision, uint8 cPrecision>
	inline Mesh Mesh::Torus<rPrecision, cPrecision>::Create(float radius, float c)
	{
		Mesh torusMesh;

		std::vector<VertexPTNTaH> quadStripVertices;

		for (int i{ 0 }; i < rPrecision; i++)
		{
			for (int j = 0; j <= cPrecision; j++)
			{
				for (int k = 0; k <= 1; k++)
				{
					float s = (i + k) % rPrecision + 0.5;
					float t = j % (cPrecision + 1);

					float x = (c + radius * cosf(s * (M_PI * 2.0) / rPrecision)) * cosf(t * (M_PI * 2.0) / cPrecision);
					float y = (c + radius * cosf(s * (M_PI * 2.0) / rPrecision)) * sinf(t * (M_PI * 2.0) / cPrecision);
					float z = radius * sinf(s * (M_PI * 2.0) / rPrecision);

					float cx = cosf(t * (M_PI * 2.0) / cPrecision);
					float cy = sinf(t * (M_PI * 2.0) / cPrecision);

					float u = (i + k) / static_cast<float>(rPrecision);
					float v = t / static_cast<float>(cPrecision);

					VertexPTNTaH vertex;

					vertex.Get<PositionXYZ, Vector3>() = { 2.0f * x, 2.0f * y, 2.0f * z };
					vertex.Get<NormalXYZ, Vector3>() = (vertex.Get<PositionXYZ, Vector3>() - Vector3(cx, cy, 0.0f)).Normalize();
					vertex.Get<TextureUV, Vector2>() = { u, v };

					quadStripVertices.push_back(vertex);
				}
			}
		}

		std::vector<VertexPTNTaH> triangleListVertices;

		for (uint64 i{ 0 }; i < quadStripVertices.size() - 2; i += 2)
		{
			triangleListVertices.push_back(quadStripVertices[i + 0]);
			triangleListVertices.push_back(quadStripVertices[i + 2]);
			triangleListVertices.push_back(quadStripVertices[i + 1]);

			triangleListVertices.push_back(quadStripVertices[i + 1]);
			triangleListVertices.push_back(quadStripVertices[i + 2]);
			triangleListVertices.push_back(quadStripVertices[i + 3]);
		}

		torusMesh.m_vertices.resize(triangleListVertices.size() * VertexPTNTaH::Size);
		std::memcpy(torusMesh.m_vertices.data(), triangleListVertices.data(), torusMesh.m_vertices.size());

		torusMesh.m_indices.resize(triangleListVertices.size() * sizeof(uint32));

		VectorView<uint32> indices(torusMesh.m_indices);

		for (uint64 i{ 0 }; i < indices.Size(); i++)
		{
			indices[i] = i;
		}

		torusMesh.GenerateTangents();

		return torusMesh;
	}
}