namespace aw
{
	template<typename Vertex, MeshLoadStaticParamsBit params>
	inline bool MeshLoaderStatic::LoadObj(const char* filename, Mesh& mesh)
	{
		InternalMeshLoadData<Vertex> loadData{};

		std::ifstream file(filename);

		if (file.good() == false)
		{
			std::cout << "File not found " << filename << std::endl;
			return false;
		}

		std::cout << "\nLoading " << filename << "\nPlease wait..." << std::endl;

		auto start = std::chrono::system_clock::now();

		std::string line;
		while (std::getline(file, line))
		{
			if (line.length() == 0)
				continue;

			//std::cout << line << std::endl;

			if (!ProcessLine<Vertex, params>(std::istringstream(line), loadData))
			{
				std::cout << "Invalid file \"" << filename << "\" (" << line << ")" << std::endl;
				return false;
			}
		}

		auto end = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = end - start;

		std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
		std::cout << "vertices count: " << loadData.vertices.size() << "\n";
		std::cout << "indices count: " << loadData.indices.size() << "\n";

		//std::swap(mesh.GetVertices(), loadData.vertices);
		//std::swap(mesh.GetIndices(), loadData.indices);

		for (Vertex& vertex : loadData.vertices)
		{
			for (uint64 i = 0; i < Vertex::Size; i++)
			{
				mesh.GetVertices().push_back(vertex.data[i]);
			}
		}

		//mesh.GetVertices().resize(loadData.vertices.size() * Vertex::Size);
		//memcpy(mesh.GetVertices().data(), loadData.vertices.data(), loadData.vertices.size() * Vertex::Size * sizeof(float));

		//mesh.GetVertices().insert(mesh.GetVertices().begin(), loadData.vertices.begin(), loadData.vertices.end());
		mesh.GetIndices().insert(mesh.GetIndices().begin(), loadData.indices.begin(), loadData.indices.end());

		for (uint64 i = 0; i < loadData.indices.size(); i++)
		{
			mesh.GetIndices().push_back(loadData.indices[i]);
		}

		return true;
	}

	template<typename Vertex, MeshLoadStaticParamsBit params>
	inline bool MeshLoaderStatic::ProcessLine(std::istringstream&& line, InternalMeshLoadData<Vertex>& loadData) noexcept
	{
		constexpr bool useIndices = (params & MeshLoadStaticParamsBit::DontUseIndices) == MeshLoadStaticParamsBit::None;
		constexpr bool loadNormals = (params & MeshLoadStaticParamsBit::DontLoadNormals) == MeshLoadStaticParamsBit::None;
		constexpr bool loadTextureUVs = (params & MeshLoadStaticParamsBit::DontLoadTextureUVs) == MeshLoadStaticParamsBit::None;
		constexpr bool computeTangents = (params & MeshLoadStaticParamsBit::ComputeTangents) != MeshLoadStaticParamsBit::None;
		constexpr bool normalizeNormals = (params & MeshLoadStaticParamsBit::NormalizeNormals) != MeshLoadStaticParamsBit::None;

		std::string firstToken;

		if (!(line >> firstToken))
			return false;

		if (firstToken[0] == '#')
			return true;

		if (firstToken == "v")
		{
			return ProcessPosition<Vertex, params>(std::forward<std::istringstream>(line), loadData);
		}
		else if (firstToken == "vt")
		{
			if constexpr (loadTextureUVs)
				return ProcessTextureUV<Vertex>(std::forward<std::istringstream>(line), loadData);
			else
				return true;
		}
		else if (firstToken == "vn")
		{
			if constexpr (loadNormals)
				return ProcessNormal<Vertex>(std::forward<std::istringstream>(line), loadData);
			else
				return true;
		}
		else if (firstToken == "vp")
		{
			return false;
		}
		else if (firstToken == "f")
		{
			return ProcessFace<Vertex, params>(std::forward<std::istringstream>(line), loadData);
		}
		else if (firstToken == "g")
		{
			return true;
		}
		else if (firstToken == "o")
		{
			return true;
		}
		else if (firstToken == "s")
		{
			return true;
		}
		else if (firstToken == "mtllib")
		{
			return true;
		}
		else if (firstToken == "usemtl")
		{
			return true;
		}

		return false;
	}

	template<typename Vertex, MeshLoadStaticParamsBit params>
	inline bool MeshLoaderStatic::ProcessPosition(std::istringstream&& line, InternalMeshLoadData<Vertex>& loadData) noexcept
	{
		Vector3 pos;
		if (!(line >> pos.x >> pos.y >> pos.z))
			return false;

		if constexpr ((params & MeshLoadStaticParamsBit::InvertY) != MeshLoadStaticParamsBit::None)
			pos.y = -pos.y;
		//pos = -pos;

		loadData.positions.push_back(pos);

		return true;
	}

	template<typename Vertex>
	inline bool MeshLoaderStatic::ProcessTextureUV(std::istringstream&& line, InternalMeshLoadData<Vertex>& loadData) noexcept
	{
		Vector2 uv;
		if (!(line >> uv.x >> uv.y))
			return false;

		loadData.textureUVs.push_back(uv);

		return true;
	}

	template<typename Vertex>
	inline bool MeshLoaderStatic::ProcessNormal(std::istringstream&& line, InternalMeshLoadData<Vertex>& loadData) noexcept
	{
		Vector3 normal;
		if (!(line >> normal.x >> normal.y >> normal.z))
			return false;

		loadData.normals.push_back(normal);

		return true;
	}

	template<typename Vertex, MeshLoadStaticParamsBit params>
	inline bool MeshLoaderStatic::ProcessFace(std::istringstream&& line, InternalMeshLoadData<Vertex>& loadData) noexcept
	{
		constexpr bool useIndices = (params & MeshLoadStaticParamsBit::DontUseIndices) == MeshLoadStaticParamsBit::None;
		constexpr bool loadNormals = (params & MeshLoadStaticParamsBit::DontLoadNormals) == MeshLoadStaticParamsBit::None;
		constexpr bool loadTextureUVs = (params & MeshLoadStaticParamsBit::DontLoadTextureUVs) == MeshLoadStaticParamsBit::None;
		constexpr bool computeTangents = (params & MeshLoadStaticParamsBit::ComputeTangents) != MeshLoadStaticParamsBit::None;
		constexpr bool normalizeNormals = (params & MeshLoadStaticParamsBit::NormalizeNormals) != MeshLoadStaticParamsBit::None;

		std::vector<Vertex> polygonVertices;
		std::string nextToken;

		while (line >> nextToken)
		{
			Vertex vertex;

			int64 index = GetNextFaceIndex(nextToken);

			if (index == 0)
				return false;

			if (index < 0)
				index = int64(loadData.positions.size()) + index;
			else if (index > 0)
				index--;

			vertex.Get<PositionXYZ, Vector3>() = loadData.positions[index];

			if constexpr (loadTextureUVs)
			{
				index = GetNextFaceIndex(nextToken);

				if (index != 0)
				{
					if (index < 0)
						index = int64(loadData.textureUVs.size()) + index;
					else if (index > 0)
						index--;

					vertex.Get<TextureUV>() = loadData.textureUVs[index];
				}
			}
			else
				GetNextFaceIndex(nextToken);

			if constexpr (loadNormals)
			{
				index = GetNextFaceIndex(nextToken);

				if (index != 0)
				{
					if (index < 0)
						index = int64(loadData.normals.size()) + index;
					else if (index > 0)
						index--;

					if (loadData.normals.size() == 0)
					{
						vertex.Get<NormalXYZ>() = { 1, 0, 0 };
					}
					else
					{
						if constexpr (normalizeNormals)
							vertex.Get<NormalXYZ>() = loadData.normals[index].GetNormalized();
						else
							vertex.Get<NormalXYZ>() = loadData.normals[index];
					}
				}
			}

			polygonVertices.push_back(vertex);
		}

		if (polygonVertices.size() < 3)
			return false;
		else if (polygonVertices.size() > 3)
		{
			InsertVertex<Vertex, params>(polygonVertices[0], loadData);
			InsertVertex<Vertex, params>(polygonVertices[2], loadData);
			InsertVertex<Vertex, params>(polygonVertices[3], loadData);
		}

		InsertVertex<Vertex, params>(polygonVertices[0], loadData);
		InsertVertex<Vertex, params>(polygonVertices[1], loadData);
		InsertVertex<Vertex, params>(polygonVertices[2], loadData);

		return true;
	}

	template<typename Vertex, MeshLoadStaticParamsBit params>
	inline void MeshLoaderStatic::InsertVertex(const Vertex& vertex, InternalMeshLoadData<Vertex>& loadData)
	{
		constexpr bool useIndices = (params & MeshLoadStaticParamsBit::DontUseIndices) == MeshLoadStaticParamsBit::None;

		if constexpr (useIndices == false)
		{
			loadData.indices.push_back(loadData.indices.size());
			loadData.vertices.push_back(vertex);
			return;
		}

		std::array<float, Vertex::Size> arr;
		memcpy(arr.data(), vertex.data, Vertex::Capacity);

		auto search = loadData.verticesSet.find(arr);
		if (search != loadData.verticesSet.end())
		{
			loadData.indices.push_back(search->second);
			return;
		}

		loadData.indices.push_back(loadData.higestIndex);

		loadData.vertices.push_back(vertex);
		loadData.verticesSet.insert({ arr, loadData.higestIndex++ });
	}
	//*/

	//int64 MeshLoaderStatic::GetNextFaceIndex(std::string& faceToken) // TODO modifying string is heavy use const char*& instead
	//{
	//	if (faceToken[0] == '/')
	//		return 0;

	//	int64 index = std::stoll(faceToken); // stoll stops when it encounter a non numeric character ("42/666/-1" -> 42)
	//	faceToken.erase(0, faceToken.find('/') + 1);

	//	return index;
	//}
}