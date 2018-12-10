#include <Awesome/SDK/Resources/OBJLoader.hpp>

#include <Awesome/Core/Chrono.hpp>
#include <Awesome/Core/VectorView.hpp>

namespace aw
{
	namespace sdk
	{
		Mesh OBJLoader::Load(const std::string& filename, const VertexLayout* vertexLayout, OBJLoadParamsBit params)
		{
			InternalMeshLoadData loadData(vertexLayout);

			std::ifstream file(filename);

			Mesh mesh;
			mesh.vertexLayout = vertexLayout;

			if (file.good() == false)
			{
				LOG_ERROR("File not found (%s)", filename.c_str());
				return mesh;
			}

			LOG_LOG("Loading file \"%s\"\nPlease wait...", filename.c_str());
			//std::cout << "\nLoading " << filename << "\nPlease wait..." << std::endl;

			Chrono chrono;
			chrono.Play();

			std::string line;
			while (std::getline(file, line))
			{
				//std::cout << "ok " << std::flush;
				if (line.length() == 0)
					continue;

				//ProcessLine(std::istringstream(line), loadData, params);

				ASSERT(!ProcessLine(std::istringstream(line), loadData, params), "Invalid file \"%s\" : %s", filename.c_str(), line.c_str());

				/*if (!ProcessLine(std::istringstream(line), loadData, params))
				{
				std::cout << "Invalid file \"" << filename << "\" (" << line << ")" << std::endl;
				return false;
				}*/
			}

			auto elapsedSeconds = float(chrono.GetElapsedTime<Chrono::Seconds>());
			LOG_LOG("elapsed time: %fs", elapsedSeconds);
			LOG_LOG("vertices count: %d", uint32(loadData.vertices.size() / loadData.vertexSize));
			LOG_LOG("indices count: %d", uint32(loadData.indices.size()));

			/*std::chrono::duration<double> elapsed_seconds = end - start;

			LOG_LOG("elapsed time: %fs", elapsed_seconds.count());
			LOG_LOG("vertices count: %d", uint32(loadData.vertices.size() / loadData.vertexSize));
			LOG_LOG("indices count: %d", uint32(loadData.indices.size()));*/

			//std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
			//std::cout << "vertices count: " << loadData.vertices.size() << "\n";
			//std::cout << "indices count: " << loadData.indices.size() << "\n";

			/*for (auto& vertex : loadData.vertices)
			{
			mesh.GetVertices().insert(mesh.GetVertices().end(), vertex.begin(), vertex.end());
			}*/

			//		mesh.GetVertices().insert(mesh.GetVertices().end(), loadData.vertices.begin(), loadData.vertices.end());
			//		mesh.GetIndices().insert(mesh.GetIndices().end(), loadData.indices.begin(), loadData.indices.end());

			std::vector<uint8> indices8Bits(loadData.indices.size() * sizeof(uint32));
			memcpy(indices8Bits.data(), loadData.indices.data(), indices8Bits.size());

			/*VectorView<uint32> idcs(indices8Bits);

			for (size_t i{ 0 }; i < loadData.indices.size(); i++)
			{
				LOG_LOG("%d == %d", loadData.indices[i], idcs[i]);
			}

			VectorView<Vector3> vtcs(loadData.vertices);

			for (size_t i{ 0 }; i < vtcs.Size(); i++)
			{
				LOG_LOG("%s", vtcs[i].ToString().c_str());
			}*/

			mesh.SetVertices(std::move(loadData.vertices));
			mesh.SetIndices(std::move(indices8Bits));
			mesh.indexType = vk::IndexType::eUint32;

			return mesh;
		}

		bool OBJLoader::ProcessLine(std::istringstream&& line, InternalMeshLoadData& loadData, OBJLoadParamsBit params)
		{
			//bool useIndices = (params & DontUseIndices) == 0;
			bool loadNormals = (params & OBJLoadParamsBit::DontLoadNormals) == OBJLoadParamsBit::None;
			bool loadTextureUVs = (params & OBJLoadParamsBit::DontLoadTextureUVs) == OBJLoadParamsBit::None;
			//bool normalizeNormals = (params & NormalizeNormals) != 0;

			std::string firstToken;

			if (!(line >> firstToken))
				return false;

			if (firstToken[0] == '#')
				return true;

			if (firstToken == "v")
			{
				return ProcessPosition(std::forward<std::istringstream>(line), loadData, params);
			}
			else if (firstToken == "vt")
			{
				if (loadTextureUVs)
					return ProcessTextureUV(std::forward<std::istringstream>(line), loadData, params);
				else
					return true;
			}
			else if (firstToken == "vn")
			{
				if (loadNormals)
					return ProcessNormal(std::forward<std::istringstream>(line), loadData, params);
				else
					return true;
			}
			else if (firstToken == "vp")
			{
				return false;
			}
			else if (firstToken == "f")
			{
				return ProcessFace(std::forward<std::istringstream>(line), loadData, params);
			}
			else if (firstToken == "g")
			{
				return true; // TODO implement submeshes
			}
			else if (firstToken == "o")
			{
				return true; // TODO implement submeshes
			}
			else if (firstToken == "s")
			{
				return true;
			}
			else if (firstToken == "mtllib")
			{
				return true; // TODO implement materials
			}
			else if (firstToken == "usemtl")
			{
				return true; // TODO implement materials
			}

			return false;
		}

		bool OBJLoader::ProcessPosition(std::istringstream&& line, InternalMeshLoadData& loadData, OBJLoadParamsBit params)
		{
			float x, y, z = 0;
			if (!(line >> x >> y >> z))
				return false;

			if ((params & OBJLoadParamsBit::InvertY) != OBJLoadParamsBit::None)
				y = -y;

			loadData.positions.push_back({x, y, z});

			return true;
		}

		bool OBJLoader::ProcessTextureUV(std::istringstream&& line, InternalMeshLoadData& loadData, OBJLoadParamsBit params)
		{
			Vector2 uv;
			if (!(line >> uv.x >> uv.y))
				return false;

			if ((params & OBJLoadParamsBit::InvertV) != OBJLoadParamsBit::None)
				uv.y = 1.0f - uv.y;

			loadData.textureUVs.push_back(uv);

			return true;
		}

		bool OBJLoader::ProcessNormal(std::istringstream&& line, InternalMeshLoadData& loadData, OBJLoadParamsBit params)
		{
			float x, y, z = 0;
			if (!(line >> x >> y >> z))
				return false;

			Vector3 normal{ x, y, z };

			if ((params & OBJLoadParamsBit::NormalizeNormals) != OBJLoadParamsBit::None)
				normal.Normalize();

			loadData.normals.push_back(normal);

			return true;
		}

		bool OBJLoader::ProcessFace(std::istringstream&& line, InternalMeshLoadData& loadData, OBJLoadParamsBit params)
		{
			//bool useIndices = (params & OBJLoadParamsBit::DontUseIndices) == OBJLoadParamsBit::None;
			bool loadNormals = (params & OBJLoadParamsBit::DontLoadNormals) == OBJLoadParamsBit::None;
			bool loadTextureUVs = (params & OBJLoadParamsBit::DontLoadTextureUVs) == OBJLoadParamsBit::None;

			//std::vector<VertexData> polygonVertices;
			std::vector<VertexIndices> polygonVerticesIndices;
			std::string nextToken;

			while (line >> nextToken)
			{
				//VertexData vertex;
				VertexIndices indices;

				int64 index = GetNextFaceIndex(nextToken);

				ASSERT(index == 0, "Invalid file, OBJ format doesn't have index of 0");

				if (index < 0)
					index = int64(loadData.positions.size()) + index;
				else// if (index > 0)
					index--;

				indices.position = index;

				//ASSERT(loadData.positionOffset < 0, "Mesh's vertex layout does not support positions");
				//reinterpret_cast<Vector3&>(vertex[loadData.positionOffset]) = loadData.positions[index];
				//vertex.Get<Vec3f, VertexComponent::PositionXYZ>() = loadData.positions[index];

				if (loadTextureUVs)
				{
					index = GetNextFaceIndex(nextToken);

					if (index != 0)
					{
						if (index < 0)
							index = int64(loadData.textureUVs.size()) + index;
						else if (index > 0)
							index--;

						indices.uv = index;

						//ASSERT(loadData.uvOffset < 0, "Mesh's vertex layout does not support uvs");
						//reinterpret_cast<Vector2&>(vertex[loadData.uvOffset]) = loadData.textureUVs[index];
						//vertex.Get<Vec2f, VertexComponent::TextureUV>() = loadData.textureUVs[index];
					}
				}
				else
					GetNextFaceIndex(nextToken);

				if (loadNormals)
				{
					index = GetNextFaceIndex(nextToken);

					if (index != 0)
					{
						if (index < 0)
							index = int64(loadData.normals.size()) + index;
						else if (index > 0)
							index--;

						indices.normal = index;

						//ASSERT(loadData.normalOffset < 0, "Mesh's vertex layout does not support normals");
						//reinterpret_cast<Vector3&>(vertex[loadData.normalOffset]) = loadData.normals[index];
						//vertex.Get<Vec3f, VertexComponent::NormalXYZ>() = loadData.normals[index];

						/*if (loadData.normals.size() == 0)
						{
						vertex.Get<Vec3f, VertexComponent::NormalXYZ>() = { 1, 0, 0 };
						}
						else
						{
						vertex.Get<Vec3f, VertexComponent::NormalXYZ>() = loadData.normals[index];
						}*/
					}
				}

				//polygonVertices.push_back(vertex);
				polygonVerticesIndices.push_back(indices);
			}

			/*if (polygonVertices.size() < 3)
			return false;
			else if (polygonVertices.size() > 3) // TODO handle more than quads
			{
			InsertVertex(polygonVertices[0], loadData, params);
			InsertVertex(polygonVertices[2], loadData, params);
			InsertVertex(polygonVertices[3], loadData, params);
			}

			InsertVertex(polygonVertices[0], loadData, params);
			InsertVertex(polygonVertices[1], loadData, params);
			InsertVertex(polygonVertices[2], loadData, params);*/

			if (polygonVerticesIndices.size() < 3)
				return false;
			else if (polygonVerticesIndices.size() > 3) // TODO handle more than quads
			{
				InsertVertex(polygonVerticesIndices[0], loadData, params);
				InsertVertex(polygonVerticesIndices[2], loadData, params);
				InsertVertex(polygonVerticesIndices[3], loadData, params);
			}

			InsertVertex(polygonVerticesIndices[0], loadData, params);
			InsertVertex(polygonVerticesIndices[1], loadData, params);
			InsertVertex(polygonVerticesIndices[2], loadData, params);

			return true;
		}

		void OBJLoader::InsertVertex(const VertexIndices& indices, InternalMeshLoadData& loadData, OBJLoadParamsBit params)
		{
			bool useIndices = (params & OBJLoadParamsBit::DontUseIndices) == OBJLoadParamsBit::None;

			if (useIndices == false)
			{
				//loadData.indices.push_back(loadData.indices.size());

				std::vector<uint8> vertex;

				ASSERT(loadData.positionOffset < 0, "Mesh's vertex layout does not support positions");
				reinterpret_cast<Vector3&>(vertex[loadData.positionOffset]) = loadData.positions[indices.position];
				ASSERT(loadData.uvOffset < 0, "Mesh's vertex layout does not support uvs");
				reinterpret_cast<Vector2&>(vertex[loadData.uvOffset]) = loadData.textureUVs[indices.uv];
				ASSERT(loadData.normalOffset < 0, "Mesh's vertex layout does not support normals");
				reinterpret_cast<Vector3&>(vertex[loadData.normalOffset]) = loadData.normals[indices.normal];

				loadData.vertices.insert(loadData.vertices.end(), vertex.begin(), vertex.end());
				return;
			}

			//std::array<float, Vertex::Size> arr;
			//memcpy(arr.data(), vertex.data, Vertex::Capacity);

			//std::string indicesStr(reinterpret_cast<const char*>(indicesToNullTreminated));
			std::stringstream ss;
			ss << indices.position << "/" << indices.uv << "/" << indices.normal;

			std::string indicesStr(ss.str());

			auto search = loadData.verticesIndicesSet.find(indicesStr);
			if (search != loadData.verticesIndicesSet.end())
			{
				loadData.indices.push_back(search->second);
				return;
			}

			loadData.indices.push_back(loadData.higestIndex);

			std::vector<uint8> vertex;
			vertex.resize(loadData.vertexSize);

			//ASSERT(loadData.positionOffset < 0, "Mesh's vertex layout does not support positions");
			//vertex.resize(3);
			if (loadData.positionOffset != uint32(~0u))
				reinterpret_cast<Vector3&>(vertex[loadData.positionOffset]) = loadData.positions[indices.position];

			//ASSERT(loadData.uvOffset < 0, "Mesh's vertex layout does not support uvs");
			//vertex.resize(vertex.size() + 2);
			if (loadData.uvOffset != uint32(~0u))
				reinterpret_cast<Vector2&>(vertex[loadData.uvOffset]) = loadData.textureUVs[indices.uv];

			//ASSERT(loadData.normalOffset < 0, "Mesh's vertex layout does not support normals");
			//vertex.resize(vertex.size() + 3);
			if (loadData.normalOffset != uint32(~0u) && loadData.normals.size() > 0)
			{
				//std::cout << loadData.normals[indices.normal].ToString() << std::endl;
				reinterpret_cast<Vector3&>(vertex[loadData.normalOffset]) = loadData.normals[indices.normal];
			}
			else
			{
				//std::cout << loadData.positions[indices.position].ToString() << std::endl;
				if (loadData.normalOffset != uint32(~0u))
					reinterpret_cast<Vector3&>(vertex[loadData.normalOffset]) = loadData.positions[indices.position].GetNormalized();
			}

			loadData.vertices.reserve(loadData.vertices.size() + vertex.size());
			loadData.vertices.insert(loadData.vertices.end(), vertex.begin(), vertex.end());
			loadData.verticesIndicesSet.insert({ indicesStr, loadData.higestIndex++ });
		}

		int64 OBJLoader::GetNextFaceIndex(std::string& faceToken) // TODO modifying string is heavy use const char*& instead
		{
			if (faceToken[0] == '/')
				return 0;

			int64 index = std::stoll(faceToken); // stoll stops when it encounter a non numeric character ("42/666/-1" -> 42)
			faceToken.erase(0, faceToken.find('/') + 1);

			return index;
		}
	}
}