#pragma once

#ifndef AWESOME_SDK_FBX_LOADER_GUARD
#define AWESOME_SDK_FBX_LOADER_GUARD

#include <fbxsdk.h>

#include <Awesome/SDK/Resources/ResourceManager.hpp>
#include <Awesome/SDK/Config.hpp>

#include <Awesome/Renderer/Resource/Model.hpp>

namespace aw
{
	namespace sdk
	{
		class FBXLoader
		{
			FbxManager* m_fbxMgr = nullptr;

			class Scene
			{
				FbxScene* m_handle = nullptr;

			public:
				inline Scene();
				inline ~Scene();

				inline FbxScene* Get();
			};

			class Importer
			{
				FbxImporter* m_handle = nullptr;

				std::vector<Mesh*> m_meshes;
				std::vector<MaterialHandle> m_materials;

				AWESOME_SDK_API void ProcessNode(FbxNode* node);
				AWESOME_SDK_API void ProcessMesh(FbxMesh* fbxMesh);
				AWESOME_SDK_API void ProcessPosition(FbxMesh* fbxMesh, int32 ctrlPointIndex, Vector3& position);
				AWESOME_SDK_API void ProcessNormal(FbxMesh* fbxMesh, int32 ctrlPointIndex, int32 vertexCounter, Vector3& normal);
				AWESOME_SDK_API void ProcessTangent(FbxMesh* fbxMesh, int32 ctrlPointIndex, int32 vertexCounter, Vector3& tangent);
				AWESOME_SDK_API void ProcessUV(FbxMesh* fbxMesh, int32 ctrlPointIndex, int32 vertexCounter, Vector2& uv);
				AWESOME_SDK_API void ProcessMaterial(FbxSurfaceMaterial* fbxMesh);
			public:

				inline Importer(const std::string& filename, Scene& fbxScene);
				inline ~Importer();

				inline Model Process();
			};

			inline FBXLoader();

			static inline FBXLoader& GetInstance();

		public:
			FBXLoader(const FBXLoader&) = delete;
			FBXLoader(FBXLoader&&) = delete;
			inline ~FBXLoader();

			AWESOME_SDK_API static Model Load(const std::string& filename, ResourceManager& resourceManager);

			FBXLoader& operator=(const FBXLoader&) = delete;
			FBXLoader& operator=(FBXLoader&&) = delete;
		};
	}
}

#include <Awesome/SDK/Resources/FBXLoader.inl>

#endif // GUARD