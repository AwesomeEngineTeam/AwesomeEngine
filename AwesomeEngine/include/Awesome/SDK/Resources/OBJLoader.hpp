#pragma once

#ifndef AWESOME_SDK_OBJ_LOADER_GUARD
#define AWESOME_SDK_OBJ_LOADER_GUARD

#include <Awesome/SDK/Config.hpp>

#include <Awesome/Renderer/Resource/Mesh.hpp>
#include <Awesome/Renderer/Pipeline/StaticVertexAttribute.hpp>
#include <Awesome/Renderer/Pipeline/StaticVertex.hpp>
#include <Awesome/Renderer/Pipeline/VertexLayout.hpp>

#include <Awesome/Core/Logger/Logger.hpp>

#include <Awesome/Maths/Vector2.hpp>
#include <Awesome/Maths/Vector3.hpp>

#include <array>

#include <iostream>
#include <fstream>
#include <sstream>

namespace aw
{
	namespace sdk
	{
		enum class OBJLoadParamsBit : uint32
		{
			None = 0,
			DontUseIndices = (1u << 0),
			DontLoadNormals = (1u << 1),
			DontLoadTextureUVs = (1u << 2),
			NormalizeNormals = (1u << 3),
			InvertY = (1u << 4),
			InvertV = (1u << 5),
			MeshLoadParamsBit_MAX = InvertV,
		};

		constexpr OBJLoadParamsBit operator|(OBJLoadParamsBit lhs, OBJLoadParamsBit rhs)
		{
			return OBJLoadParamsBit(static_cast<uint32>(lhs) | static_cast<uint32>(rhs));
		}

		constexpr OBJLoadParamsBit operator&(OBJLoadParamsBit lhs, OBJLoadParamsBit rhs)
		{
			return OBJLoadParamsBit(static_cast<uint32>(lhs) & static_cast<uint32>(rhs));
		}

		class OBJLoader
		{
			struct InternalMeshLoadData;
			struct VertexIndices;

		public:
			OBJLoader() = delete;
			~OBJLoader() = delete;

			// Mesh
			AWESOME_SDK_API static Mesh Load(const std::string& filename, const VertexLayout* vertexLayout = &(VertexPTNTaH::Layout), OBJLoadParamsBit params = OBJLoadParamsBit::None);

		private:
			AWESOME_SDK_API static bool ProcessLine(std::istringstream&& line, InternalMeshLoadData& loadData, OBJLoadParamsBit params);
			AWESOME_SDK_API static bool ProcessPosition(std::istringstream&& line, InternalMeshLoadData& loadData, OBJLoadParamsBit params);
			AWESOME_SDK_API static bool ProcessTextureUV(std::istringstream&& line, InternalMeshLoadData& loadData, OBJLoadParamsBit params);
			AWESOME_SDK_API static bool ProcessNormal(std::istringstream&& line, InternalMeshLoadData& loadData, OBJLoadParamsBit params);
			AWESOME_SDK_API static bool ProcessFace(std::istringstream&& line, InternalMeshLoadData& loadData, OBJLoadParamsBit params);
			AWESOME_SDK_API static void InsertVertex(const VertexIndices& indices, InternalMeshLoadData& loadData, OBJLoadParamsBit params);
			AWESOME_SDK_API static int64 GetNextFaceIndex(std::string& faceToken);

		private:
			struct InternalMeshLoadData
			{
				//const VertexLayout& vertexLayout;
				uint32 positionOffset{ ~0u };
				uint32 uvOffset{ ~0u };
				uint32 normalOffset{ ~0u };
				uint32 vertexSize{ 0 };
				std::vector<Vector3> positions;
				std::vector<Vector2> textureUVs;
				std::vector<Vector3> normals;
				//std::vector<VertexData> vertices;
				std::vector<uint8> vertices;
				std::unordered_map<std::string, uint32> verticesIndicesSet;
				std::vector<uint32> indices;
				uint16 higestIndex = 0;

				explicit InternalMeshLoadData(const VertexLayout* vertexLayout)// : vertexLayout(_vertexLayout)
				{
					ASSERT(vertexLayout == nullptr, "vertexLayout is nullptr");

					positionOffset = vertexLayout->GetAttributeOffset(VertexAttribute(PositionXYZ));
					uvOffset = vertexLayout->GetAttributeOffset(VertexAttribute(TextureUV));
					if (uvOffset == ~0u)
						uvOffset = vertexLayout->GetAttributeOffset(VertexAttribute(TextureUVW));
					normalOffset = vertexLayout->GetAttributeOffset(VertexAttribute(NormalXYZ));
					vertexSize = vertexLayout->GetSize();
				}
			};

			struct VertexIndices
			{
				uint64 position{ ~0ull };
				uint64 uv{ ~0ull };
				uint64 normal{ ~0ull };
			};
		};
	}
}

#endif //GUARD