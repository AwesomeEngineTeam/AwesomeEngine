#pragma once

#ifndef AWESOME_RENDERER_MESH_GUARD
#define AWESOME_RENDERER_MESH_GUARD

#include <Awesome/Core/PermutableDoublet.hpp>
#include <Awesome/Core/VectorView.hpp>

#include <Awesome/Renderer/Config.hpp>

#include <Awesome/Renderer/Pipeline/VertexLayout.hpp>
#include <Awesome/Renderer/Pipeline/StaticVertex.hpp>
#include <Awesome/Renderer/Memory/SubBuffer.hpp>

#include <vector>
#include <mutex>

namespace aw
{
	using Segment = PermutableDoublet<uint32>;

	class Mesh
	{
		std::vector<uint8> m_vertices{};
		std::vector<uint8> m_indices{};

		SubBuffer m_vertexSubBuffer{};
		SubBuffer m_indexSubBuffer{};

		vk::UniqueCommandBuffer m_flushCommandBuffer{};

	public:
		//vk::IndexType indexType{ vk::IndexType::eUint16 };
		vk::IndexType indexType{ vk::IndexType::eUint32 };
		const VertexLayout* vertexLayout{ &VertexPTNTaH::Layout };

	private:
		bool m_isCoherent{ false };

		static AWESOME_RENDERER_API std::mutex s_poolMutex;

	public:
		Mesh() = default;
		Mesh(const Mesh&) = delete;
		Mesh(Mesh&&) = default;
		inline ~Mesh();

		AWESOME_RENDERER_API void DebugVerticesPosition() const;
		inline const uint64 GetVertexCount() const;

		inline void SetVertices(std::vector<uint8> newVertices);
		inline const std::vector<uint8>& GetVertices() const;
		inline std::vector<Vector3> GetInterpretedPositions() const;
		inline std::vector<uint8>& Vertices();

		inline void SetIndices(std::vector<uint8> newIndices);
		inline const std::vector<uint8>& GetIndices() const;
		inline const std::vector<uint32>& GetInterpretedIndices() const;

		inline void SetVerticesPosition(uint32 vertexDataSize, uint32 positionOffset, std::vector<Vector3> verticesPositions);

		inline void ReverseFaces();

		AWESOME_RENDERER_API void Flush();
		inline void Draw(vk::CommandBuffer commandBuffer) const;

		inline bool IsCoherent() const;

		AWESOME_RENDERER_API void Clear();

		AWESOME_RENDERER_API void GenerateTangents();
		AWESOME_RENDERER_API void NormalizeNormals();
		AWESOME_RENDERER_API void NormalizePositions();

		// Primitives__________________________________________

		enum class NormalMode : uint8
		{
			Smooth = 0,
			PerFace
		};

		struct Quad
		{
			AWESOME_RENDERER_API static Mesh Create(float length = 1.0f);
		};

		struct Cube
		{
			AWESOME_RENDERER_API static Mesh Create(float length = 1.0f);
		};

		template<uint8 precision = 3>
		struct Sphere
		{
			inline static Mesh CreateUVSphere(float radius = 0.5f);
			inline static Mesh CreateNormalizedCube(float radius = 0.5f);
			inline static Mesh CreateSpherifiedCube(float radius = 0.5f);
			inline static Mesh CreateIcosphere(float radius = 0.5f);
		};

		template<uint8 precision = 3>
		struct Hemisphere
		{
			inline static Mesh Create(float radius = 0.5f);
		};

		template<uint8 precision = 6, NormalMode normalMode = NormalMode::Smooth>
		struct Cone
		{
			inline static Mesh Create(float radius = 0.5f, float height = 1.0f);
		};

		template<uint8 precision = 6, NormalMode normalMode = NormalMode::Smooth>
		struct Cylindroid
		{
			inline static Mesh Create(float radius = 0.5f, float radius2 = 0.5f, float height = 1.0f);
		};

		template<uint8 precision = 3>
		struct Cylinder
		{
			inline static Mesh Create(float radius = 0.5f, float height = 1.0f);
		};

		template<uint8 precision = 3>
		struct Capsule
		{
			inline static Mesh Create(float radius = 0.5f, float height = 1.0f);
		};

		template<uint8 rPrecision = 16, uint8 cPrecision = 8>
		struct Torus
		{
			inline static Mesh Create(float radius = 1.0f, float c = 0.15f);
		};

		struct DebugQuad
		{
			AWESOME_RENDERER_API static Mesh Create(float length);
		};

		struct DebugCube
		{
			AWESOME_RENDERER_API static Mesh Create(float length);
		};

		struct DebugSphere
		{
			AWESOME_RENDERER_API static Mesh Create(float radius);
		};

		struct DebugCapsule
		{
			AWESOME_RENDERER_API static Mesh Create(float radius, float height);
		};

		struct Line
		{
			AWESOME_RENDERER_API static Mesh Create(float scale = 1.0f);
			AWESOME_RENDERER_API static Mesh Create(const Vector3& begin, const Vector3& end);
		};

		// ____________________________________________________

	public:
		Mesh& operator=(const Mesh&) = delete;
		Mesh& operator=(Mesh&&) = default;
	};

}

#include <Awesome/Renderer/Resource/Mesh.inl>

#endif //GUARD