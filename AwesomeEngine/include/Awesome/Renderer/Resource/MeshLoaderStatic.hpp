#pragma once

#ifndef AWESOME_RENDERER_MESH_LOADER_STATIC_GUARD
#define AWESOME_RENDERER_MESH_LOADER_STATIC_GUARD

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Resource/Mesh.hpp>
#include <Awesome/Renderer/Pipeline/StaticVertexAttribute.hpp>

#include <Awesome/Maths/Vector3.hpp>

#include <unordered_map>
#include <array>
#include <fstream>

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	//seed ^= hasher(v) + 0x11111111 + (seed << 6) + (seed >> 2);
}

namespace std
{
	template <>
	struct hash<std::array<float, 8>>
	{
		size_t operator()(const std::array<float, 8>& v) const
		{
			size_t h = std::hash<float>()(v[0]);
			hash_combine(h, v[1]);
			hash_combine(h, v[2]);
			hash_combine(h, v[3]);
			hash_combine(h, v[4]);
			hash_combine(h, v[5]);
			hash_combine(h, v[6]);
			hash_combine(h, v[7]);
			return h;
		}
	};

	template <>
	struct hash<std::array<float, 5>>
	{
		size_t operator()(const std::array<float, 5>& v) const
		{
			size_t h = std::hash<float>()(v[0]);
			hash_combine(h, v[1]);
			hash_combine(h, v[2]);
			hash_combine(h, v[3]);
			hash_combine(h, v[4]);
			return h;
		}
	};

	template <>
	struct hash<std::array<float, 12>>
	{
		size_t operator()(const std::array<float, 12>& v) const
		{
			size_t h = std::hash<float>()(v[0]);
			hash_combine(h, v[1]);
			hash_combine(h, v[2]);
			hash_combine(h, v[3]);
			hash_combine(h, v[4]);
			hash_combine(h, v[5]);
			hash_combine(h, v[6]);
			hash_combine(h, v[7]);
			hash_combine(h, v[8]);
			hash_combine(h, v[9]);
			hash_combine(h, v[10]);
			hash_combine(h, v[11]);
			return h;
		}
	};

}

namespace aw
{
	enum class MeshLoadStaticParamsBit : uint8
	{
		None = 0,
		DontUseIndices = (1u << 0u),
		DontLoadNormals = (1u << 1u),
		DontLoadTextureUVs = (1u << 2u),
		ComputeTangents = (1u << 3u),
		NormalizeNormals = (1u << 4u),
		InvertY = (1u << 5u),
		MeshLoadStaticParamsBit_MAX = uint8(InvertY),
	};

	constexpr MeshLoadStaticParamsBit operator|(MeshLoadStaticParamsBit lhs, MeshLoadStaticParamsBit rhs)
	{
		return MeshLoadStaticParamsBit(static_cast<uint8>(lhs) | static_cast<uint8>(rhs));
	}

	constexpr MeshLoadStaticParamsBit operator&(MeshLoadStaticParamsBit lhs, MeshLoadStaticParamsBit rhs)
	{
		return MeshLoadStaticParamsBit(static_cast<uint8>(lhs) & static_cast<uint8>(rhs));
	}

	constexpr bool operator==(MeshLoadStaticParamsBit lhs, MeshLoadStaticParamsBit rhs)
	{
		return static_cast<uint8>(lhs) == static_cast<uint8>(rhs);
	}

	constexpr bool operator!=(MeshLoadStaticParamsBit lhs, MeshLoadStaticParamsBit rhs)
	{
		return static_cast<uint8>(lhs) != static_cast<uint8>(rhs);
	}

	/*class AWESOME_RENDERER_API MeshLoaderStatic
	{
		template<typename Vertex>
		struct InternalMeshLoadData;

	public:
		MeshLoaderStatic() = delete;
		~MeshLoaderStatic() = delete;

		// Mesh
		template<typename Vertex, MeshLoadStaticParamsBit params = MeshLoadStaticParamsBit::None>
		inline static bool Load(const char* filename, Mesh& mesh);

	private:
		template<typename Vertex, MeshLoadStaticParamsBit params = MeshLoadStaticParamsBit::None>
		inline static bool ProcessLine(std::istringstream&& line, InternalMeshLoadData<Vertex>& loadData) noexcept;
		template<typename Vertex, MeshLoadStaticParamsBit params = MeshLoadStaticParamsBit::None>
		inline static bool ProcessPosition(std::istringstream&& line, InternalMeshLoadData<Vertex>& loadData) noexcept;
		template<typename Vertex>
		inline static bool ProcessTextureUV(std::istringstream&& line, InternalMeshLoadData<Vertex>& loadData) noexcept;
		template<typename Vertex>
		inline static bool ProcessNormal(std::istringstream&& line, InternalMeshLoadData<Vertex>& loadData) noexcept;
		template<typename Vertex, MeshLoadStaticParamsBit params = MeshLoadStaticParamsBit::None>
		inline static bool ProcessFace(std::istringstream&& line, InternalMeshLoadData<Vertex>& loadData) noexcept;
		template<typename Vertex, MeshLoadStaticParamsBit params = MeshLoadStaticParamsBit::None>
		inline static void InsertVertex(const Vertex& vertex, InternalMeshLoadData<Vertex>& loadData);
		static int64 GetNextFaceIndex(std::string& faceToken);

	private:
		template<typename Vertex>
		struct InternalMeshLoadData
		{
			std::vector<Vector3> positions;
			std::vector<Vector2> textureUVs;
			std::vector<Vector3> normals;
			std::vector<Vector3> tangents;
			std::vector<Vertex> vertices;
			std::unordered_map<std::array<float, Vertex::Size>, uint32> verticesSet;
			std::vector<uint32> indices;
			uint32 higestIndex = 0;
		};
	};*/
}

//#include <Awesome/Renderer/MeshLoaderStatic.inl>

#endif //GUARD