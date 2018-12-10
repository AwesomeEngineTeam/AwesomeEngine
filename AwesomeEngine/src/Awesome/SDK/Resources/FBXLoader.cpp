#include <Awesome/SDK/Resources/FBXLoader.hpp>

namespace aw
{
	namespace sdk
	{
		Model FBXLoader::Load(const std::string& filename, ResourceManager& resourceManager)
		{
			Scene scene;
			Importer imp(filename, scene);

			return imp.Process();
		}

		void FBXLoader::Importer::ProcessNode(FbxNode* node)
		{
			FbxNodeAttribute* att = node->GetNodeAttribute();

			if (att)
			{
				FbxNodeAttribute::EType type = att->GetAttributeType();

				switch (type)
				{
				case FbxNodeAttribute::eMesh:
					ProcessMesh(node->GetMesh());
					break;
				case FbxNodeAttribute::eNone:
				case FbxNodeAttribute::eNull:
					break;
				default:
					LOG_LOG("Attribute not implemented yet: %d", type);
					break;
				}
			}

			int32 childCount = node->GetChildCount();

			for (int32 i = 0; i < childCount; i++)
			{
				FbxNode* child = node->GetChild(i);
				ProcessNode(child);
			}
		}

		void FBXLoader::Importer::ProcessMesh(FbxMesh* fbxMesh)
		{
			if (fbxMesh->GetElementMaterial()->GetMappingMode() == FbxLayerElement::eAllSame)
				ProcessMaterial(fbxMesh->GetNode()->GetMaterial(0));
			else
				LOG_WARNING("Fbx: Multiple Material on single mesh. Not implemented yet");

			//Mesh* mesh = new Mesh(&VertexPTNTaH::Layout);
			//mesh->SetVertices(std::vector<uint8>(fbxMesh->GetControlPointsCount() * VertexPTNTaH::Size));
			//mesh->SetIndices(std::vector<uint32>(fbxMesh->GetIndi));
			//m_meshes.push_back(mesh);

			//auto geometricTranslation = fbxMesh->GetNode()->GetGeometricTranslation(FbxNode::eSourcePivot);
			//m_currInfos->transform.position = reinterpret_cast<Vector3&>(geometricTranslation);
			//auto geometricRotation = fbxMesh->GetNode()->GetGeometricRotation(FbxNode::eSourcePivot);
			//m_currInfos->transform.rotation = reinterpret_cast<Quaternion&>(geometricRotation);
			//m_currInfos->transform.scale = fbxMesh->GetNode()->GetGeometricScaling(FbxNode::eSourcePivot)[0];

			int vertexCounter = 0;
			int32 triCount = fbxMesh->GetPolygonCount();

			for (int i = 0; i < triCount; i++)
			{
				int32 polygonSize = fbxMesh->GetPolygonSize(i);
				for (uint32 j = 0; j < polygonSize; j++)
				{
					//int ctrlPointIndex = fbxMesh->GetPolygonVertex(i, j);

					//ProcessPosition(fbxMesh, ctrlPointIndex, m_currInfos->mesh.vertices[vertexCounter].position);
					//ProcessNormal(fbxMesh, ctrlPointIndex, vertexCounter, m_currInfos->mesh.vertices[vertexCounter].normal);
					//ProcessTangent(fbxMesh, ctrlPointIndex, vertexCounter, m_currInfos->mesh.vertices[vertexCounter].tangent);
					//ProcessUV(fbxMesh, ctrlPointIndex, vertexCounter, m_currInfos->mesh.vertices[vertexCounter].uv);

					//m_currInfos->mesh.indices.push_back(vertexCounter++);
				}
			}
		}

		void FBXLoader::Importer::ProcessPosition(FbxMesh* fbxMesh, int32 ctrlPointIndex, Vector3& position)
		{
			fbxsdk::FbxVector4 pos = fbxMesh->GetControlPointAt(ctrlPointIndex);
			position = Vector3(static_cast<float>(pos[0]), static_cast<float>(pos[1]), static_cast<float>(pos[2]));
		}

		void FBXLoader::Importer::ProcessNormal(FbxMesh* fbxMesh, int32 ctrlPointIndex, int32 vertexCounter, Vector3& normal)
		{
			if (fbxMesh->GetElementNormalCount() < 1)
				return;

			FbxGeometryElementNormal* vertexNormal = fbxMesh->GetElementNormal();

			int32 index = 0;

			if (vertexNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				if (vertexNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
					index = ctrlPointIndex;
				else if (vertexNormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					index = vertexNormal->GetIndexArray().GetAt(ctrlPointIndex);
				else
				{
					ASSERT(true, "FBX ReferenceMode not supported yet!");
				}
			}
			else if (vertexNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				if (vertexNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
					index = vertexCounter;
				else if (vertexNormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
				else
				{
					ASSERT(true, "FBX ReferenceMode not supported yet!");
				}
			}
			else
			{
				ASSERT(true, "FBX MappingMode not supported yet!");
			}

			normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}

		void FBXLoader::Importer::ProcessTangent(FbxMesh* fbxMesh, int32 ctrlPointIndex, int32 vertexCounter, Vector3& tangent)
		{
			if (fbxMesh->GetElementTangentCount() < 1)
				return;

			FbxGeometryElementTangent* vertexTangent = fbxMesh->GetElementTangent();

			int32 index = 0;

			if (vertexTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				if (vertexTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
					index = ctrlPointIndex;
				else if (vertexTangent->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					index = vertexTangent->GetIndexArray().GetAt(ctrlPointIndex);
				else
				{
					ASSERT(true, "FBX ReferenceMode not supported yet!");
				}
			}
			else if (vertexTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				if (vertexTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
					index = vertexCounter;
				else if (vertexTangent->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					index = vertexTangent->GetIndexArray().GetAt(vertexCounter);
				else
				{
					ASSERT(true, "FBX ReferenceMode not supported yet!");
				}
			}
			else
			{
				ASSERT(true, "FBX MappingMode not supported yet!");
			}

			tangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			tangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			tangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}

		void FBXLoader::Importer::ProcessUV(FbxMesh* fbxMesh, int32 ctrlPointIndex, int32 vertexCounter, Vector2& uv)
		{
			if (fbxMesh->GetElementUVCount() < 1)
				return;

			FbxGeometryElementUV* vertexUV = fbxMesh->GetElementUV();

			int32 index = 0;

			if (vertexUV->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				if (vertexUV->GetReferenceMode() == FbxGeometryElement::eDirect)
					index = ctrlPointIndex;
				else if (vertexUV->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					index = vertexUV->GetIndexArray().GetAt(ctrlPointIndex);
				else
				{
					ASSERT(true, "FBX ReferenceMode not supported yet!");
				}
			}
			else if (vertexUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				if (vertexUV->GetReferenceMode() == FbxGeometryElement::eDirect)
					index = vertexCounter;
				else if (vertexUV->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					index = vertexUV->GetIndexArray().GetAt(vertexCounter);
				else
				{
					ASSERT(true, "FBX ReferenceMode not supported yet!");
				}
			}
			else
			{
				ASSERT(true, "FBX MappingMode not supported yet!");
			}

			uv.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			uv.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
		}

		void FBXLoader::Importer::ProcessMaterial(FbxSurfaceMaterial* fbxMesh)
		{
			LOG_WARNING("Fbx: Process Material not implemented yet!");
		}
	}
}