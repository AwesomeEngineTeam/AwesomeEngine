#include <Awesome/SDK/Resources/GLTFImporter.hpp>
#include <Awesome/SDK/Resources/TextureLoader.hpp>
#include <Awesome/SDK/Resources/ResourceManager.hpp>
#include <Awesome/SDK/Scene/Actor.hpp>
#include <Awesome/SDK/Components/Render/MeshRenderer.hpp>

#include <Awesome/Renderer/Pipeline/StaticVertex.hpp>
#include <Awesome/Renderer/Resource/TexelBuffer.hpp>
#include <Awesome/Renderer/Resource/Model.hpp>

#include <Awesome/Maths/Matrix4.hpp>
#include <Awesome/Maths/Quaternion.hpp>

#include <Awesome/Core/File.hpp>
#include <Awesome/Core/Chrono.hpp>
#include <Awesome/Core/Color.hpp>

#include <gltf2/glTF2.hpp>
#include <algorithm>

namespace aw
{
	namespace sdk
	{
		void ProcessNode(uint32 nodeIndex, const gltf2::Asset& asset, const std::vector<uint32>& meshes, const std::vector<uint32>& materials, Scene& scene, Actor* actor, Component* parent)
		{
			auto& node = asset.nodes[nodeIndex];
			uint32 meshIndex = node.mesh;

			if (meshIndex == ~0u)
			{
				parent = actor->AddComponent<Component>(parent);
			}
			else
			{
				//Model model;

				//std::string meshName = asset.dirName + "[mesh " + std::to_string(meshIndex) + "]" + asset.meshes[meshIndex].name;

				//model.mesh = resourceManager.GetMesh(meshName);

				auto mesh = asset.meshes[meshIndex];

				//if (mesh.primitives.empty() == false)
				//	model.material = materials[mesh.primitives.front().material];
				//else
				//{
				//	THROW("No material !");
				//}

				//std::string modelName = asset.dirName + "[model " + std::to_string(nodeIndex) + " " + std::to_string(meshIndex) + " " + std::to_string(meshIndex) + "]" + asset.nodes[nodeIndex].name;

				parent = actor->AddComponent<MeshRenderer>(parent);

				ASSERT(meshes.size() == 0, "meshes.size() == 0");
				ASSERT(materials.size() == 0, "materials.size() == 0");

				static_cast<MeshRenderer*>(parent)->SetMesh(meshes[meshIndex]);
				static_cast<MeshRenderer*>(parent)->SetMaterial(materials[mesh.primitives.front().material]);
			}

			if (node.isTransformMatrix == true)
			{
				Matrix4 mat(node.matrix);
				mat.Transpose();

				Vector3 translation = mat.GetColumn(3);
				float scale = abs(mat.GetColumn(0).Norm());

				Matrix3 mat3(mat);

				mat3 *= 1 / scale;

				parent->SetLocalPosition(translation);
				parent->SetScale(scale);
				parent->SetLocalRotation(mat3.ToQuaternion());
			}
			else
			{
				parent->SetLocalPosition({ node.transform.translation[0], node.transform.translation[1], node.transform.translation[2] });
				parent->SetScale(node.transform.scale[0]);
				parent->SetLocalRotation({ node.transform.rotation[3], node.transform.rotation[0], node.transform.rotation[1], node.transform.rotation[2] });
			}

			for (uint32 childNodeIndex : node.children)
				ProcessNode(childNodeIndex, asset, meshes, materials, scene, actor, parent);
		}

		std::vector<uint32> LoadMeshes(const gltf2::Asset& asset, Scene& scene)
		{
			using namespace gltf2;

			std::vector<std::unique_ptr<std::vector<char>>> buffers;
			buffers.reserve(asset.buffers.size());

			for (auto& buffer : asset.buffers)
			{
				File bufferBinary(asset.dirName + '/' + buffer.uri, OpenMethodBit::Binary | OpenMethodBit::Read);

				if (bufferBinary.IsOpen())
				{
					auto content = std::make_unique<std::vector<char>>(buffer.byteLength);
					bufferBinary.Read(content->data(), 1, content->size());

					buffers.emplace_back(std::move(content));
				}
			}

			std::vector<uint32> meshes;
			static std::mutex mtx;
			std::function<void(uint64, uint64)> func = [&asset, &scene, &buffers, &meshes](uint64 begin, uint64 end)
			{

				for (uint64 i{ begin }; i < end; i++)
				{
					auto& mesh = asset.meshes[i];

					std::string meshName = asset.dirName + "[mesh " + std::to_string(i) + "]" + asset.meshes[i].name;

					Mesh* meshPtr = RenderSystem::GetResourceManager().GetMesh(meshName);

					if (meshPtr == nullptr)
					{
						Mesh myMesh;

						if (mesh.primitives.empty() == false)
						{
							auto& primitive = mesh.primitives.front();

							std::vector<uint8> indices;
							std::vector<uint8> vertices;

							{
								auto& indicesAccessor = asset.accessors[primitive.indices];

								auto& bufferView = asset.bufferViews[indicesAccessor.bufferView];
								auto& bufferData = buffers[bufferView.buffer];

								if (indicesAccessor.componentType == Accessor::ComponentType::UnsignedInt)
								{
									indices.resize(indicesAccessor.count * sizeof(uint32));
									myMesh.indexType = vk::IndexType::eUint32;

									std::memcpy(indices.data(), bufferData->data() + bufferView.byteOffset + indicesAccessor.byteOffset, indices.size());
								}
								else if (indicesAccessor.componentType == Accessor::ComponentType::UnsignedShort)
								{
									indices.resize(indicesAccessor.count * sizeof(uint16));
									myMesh.indexType = vk::IndexType::eUint16;

									std::memcpy(indices.data(), bufferData->data() + bufferView.byteOffset + indicesAccessor.byteOffset, indices.size());
								}
								else if (indicesAccessor.componentType == Accessor::ComponentType::UnsignedByte)
								{
									indices.resize(indicesAccessor.count * sizeof(uint16));
									myMesh.indexType = vk::IndexType::eUint16;

									std::vector<uint8> indices8(indicesAccessor.count);
									std::vector<uint16> indices16(indicesAccessor.count);

									int8* bufferBegin = bufferData->data() + bufferView.byteOffset + indicesAccessor.byteOffset;

									std::memcpy(indices8.data(), bufferBegin, indices8.size());

									for (uint64 i{ 0 }; i < indicesAccessor.count; i++)
										indices16[i] = indices8[i];

									std::memcpy(indices.data(), indices16.data(), indices.size());
								}
								else
								{
									THROW("This ComponentType (%d) is not handled for indices", indicesAccessor.componentType);
								}


							}

							bool mustGenerateTangents = true;

							for (auto& attribute : primitive.attributes)
							{
								uint32 accessorIndex = attribute.second;
								auto& accessor = asset.accessors[accessorIndex];
								auto& bufferView = asset.bufferViews[accessor.bufferView];
								auto& bufferData = buffers[bufferView.buffer];

								#if defined(max)
								#undef max
								#endif
								vertices.resize(std::max(VertexPTNTaH::Size * static_cast<size_t>(accessor.count), vertices.size()));

								auto verticesData = reinterpret_cast<VertexPTNTaH*>(vertices.data());

								if (attribute.first == "POSITION")
								{
									const char* data = bufferData->data();
									auto ptr = data + bufferView.byteOffset + accessor.byteOffset;

									if (accessor.byteOffset > bufferView.byteLength)
									{
										LOG_ERROR("accessor.byteOffset > bufferView.byteLength");
									}

									for (uint64 j{ 0 }; j < accessor.count; j++, ptr += bufferView.byteStride == 0 ? sizeof(Vector3) : bufferView.byteStride)
										verticesData[j].Get<PositionXYZ, Vector3>() = *reinterpret_cast<const Vector3*>(ptr);
								}
								else if (attribute.first == "NORMAL")
								{
									const char* data = bufferData->data();
									auto ptr = data + bufferView.byteOffset + accessor.byteOffset;

									for (uint64 j{ 0 }; j < accessor.count; j++, ptr += bufferView.byteStride == 0 ? sizeof(Vector3) : bufferView.byteStride)
										verticesData[j].Get<NormalXYZ, Vector3>() = reinterpret_cast<const Vector3*>(ptr)->GetNormalized();
								}
								else if (attribute.first == "TANGENT")
								{
									const char* data = bufferData->data();
									auto ptr = data + bufferView.byteOffset + accessor.byteOffset;

									if (accessor.type == gltf2::Accessor::Type::Vec4)
									{
										for (uint64 j{ 0 }; j < accessor.count; j++, ptr += bufferView.byteStride == 0 ? (4 * sizeof(float)) : bufferView.byteStride)
										{
											verticesData[j].Get<TangentXYZ, Vector3>() = *reinterpret_cast<const Vector3*>(ptr);
											verticesData[j].Get<Handedness, float>() = *reinterpret_cast<const float*>(ptr + 3 * sizeof(float));
										}
									}
									else
									{
										THROW("Tangent of type different of Vec4 are not implemented");
									}

									mustGenerateTangents = false;
								}
								else if (attribute.first == "TEXCOORD_0")
								{
									const char* data = bufferData->data();
									auto ptr = data + bufferView.byteOffset + accessor.byteOffset;

									for (uint64 j{ 0 }; j < accessor.count; j++, ptr += bufferView.byteStride == 0 ? sizeof(Vector2) : bufferView.byteStride)
										verticesData[j].Get<TextureUV, Vector2>() = *reinterpret_cast<const Vector2*>(ptr);
								}
								else
								{
									LOG_WARNING("Attribute not handled (%s)", attribute.first.c_str());
								}
							}

							myMesh.SetIndices(std::move(indices));
							myMesh.SetVertices(std::move(vertices));

							if (mustGenerateTangents)
								myMesh.GenerateTangents();
						}

						//myMesh.Flush();

						mtx.lock();
						meshPtr = RenderSystem::GetResourceManager().Register(meshName, std::move(myMesh));
						mtx.unlock();
					}

					mtx.lock();
					meshes.push_back(RenderSystem::RegisterMesh(meshPtr));
					mtx.unlock();
				}
			};

			//func(0, meshes.size());

			static uint32 availableThreads = std::thread::hardware_concurrency();

			if (asset.meshes.size() == 1)
				func(0, 1);
			else if (asset.meshes.size() < availableThreads)
			{
				for (uint64 i = 0; i < asset.meshes.size(); ++i)
					sdk::ThreadManager::Push(func, i, (i + 1));
			}
			else
			{
				for (uint8 i = 0; i < availableThreads; ++i)
				{
					if(i != availableThreads - 1)
						sdk::ThreadManager::Push(func, i * (asset.meshes.size() / availableThreads), (i+1) * (asset.meshes.size() / availableThreads));
					else
						sdk::ThreadManager::Push(func, i * (asset.meshes.size() / availableThreads), asset.meshes.size());
				}
			}

			sdk::ThreadManager::JoinAll();

			for(uint64 i = 0; i<meshes.size(); ++i)
				RenderSystem::GetMesh(meshes[i])->Flush();

			return meshes;
		}

		std::vector<SampledTextureHandle> LoadTextures(const gltf2::Asset& asset, Scene& scene)
		{
			auto& resourceManager = RenderSystem::GetResourceManager();

			//std::vector<TexelBuffer> images;
			//for (auto& image : asset.images)
			//{
			//	//TexelBuffer* texelBuffer = resourceManager.GetTexelBuffer(image.uri);

			//	//if (texelBuffer == nullptr)
			//	//{
			//	//	texelBuffer = resourceManager.Load<TextureLoader>(image.uri);
			//	//}

			//	//images.push_back(texelBuffer);

			//	images.push_back();
			//}

			//vk::CommandBufferAllocateInfo cpyCmdBufAllocInfo(Context::GetCommandPool(), vk::CommandBufferLevel::ePrimary, 1);
			//vk::UniqueCommandBuffer copyCmdBuffer(
			//	std::move(Context::GetDevice().Get().allocateCommandBuffersUnique(cpyCmdBufAllocInfo)[0])
			//);

			std::vector<SampledTextureHandle> textures;

			for (uint64 i{ 0 }; i < asset.textures.size(); i++)
			{
				if (asset.samplers.empty() == false)
				{
					LOG_WARNING("Samplers not handled");
				}

				std::string textureName = asset.dirName + "[texture " + std::to_string(i) + "]" + asset.textures[i].name;

				SampledTextureHandle texHandle = resourceManager.GetTexture(textureName);

				if (!texHandle.GetImage())
				{
					SampledTexture myTexture;
					myTexture.Create(
						vk::ImageType::e2D,
						vk::Format::eR8G8B8A8Unorm,
						TextureLoader::Load(asset.images[asset.textures[i].source].uri),
						vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
						vk::ImageAspectFlagBits::eColor
					);

					texHandle = resourceManager.Register(textureName, std::move(myTexture));
				}

				textures.push_back(texHandle);
			}

			return textures;
		}

		std::vector<uint32> LoadMaterials(const gltf2::Asset& asset, Scene& scene)
		{
			auto& resourceManager = RenderSystem::GetResourceManager();

			std::vector<uint32> materials;

			for (uint64 i{ 0 }; i < asset.materials.size(); i++)
			{
				auto& material = asset.materials[i];
				Material mat{};

				if (material.emissiveTexture.index != ~0u)
					mat.materialInfo.hasEmissiveMap = true;
				if (material.normalTexture.index != ~0u)
					mat.materialInfo.hasNormalMap = true;
				if (material.occlusionTexture.index != ~0u)
					mat.materialInfo.hasOcclusionMap = true;
				if (material.pbr.baseColorTexture.index != ~0u)
					mat.materialInfo.hasDiffuseMap = true;
				if (material.pbr.metallicRoughnessTexture.index != ~0u)
					mat.materialInfo.hasMetalicRoughnessMap = true;

				resourceManager.materialManager.CreateMaterial(mat, sb::ShaderBuilder::MakeShaderProgram);

				if (material.emissiveTexture.index != ~0u)
				{
					const int32 textureIndex = material.emissiveTexture.index;
					std::string textureName = asset.dirName + "[texture " + std::to_string(textureIndex) + "]" + asset.textures[textureIndex].name;

					mat.SetSampledTexture(resourceManager.GetTexture(textureName), mat.GetEmissiveBinding());
					//mat.SetEmissive(resourceManager.GetTexture(textureName));
				}
				if (material.normalTexture.index != ~0u)
				{
					const int32 textureIndex = material.normalTexture.index;
					std::string textureName = asset.dirName + "[texture " + std::to_string(textureIndex) + "]" + asset.textures[textureIndex].name;

					mat.SetSampledTexture(resourceManager.GetTexture(textureName), mat.GetNormalBinding());
					//mat.SetNormal(resourceManager.GetTexture(textureName));
				}
				if (material.occlusionTexture.index != ~0u)
				{
					const int32 textureIndex = material.occlusionTexture.index;
					std::string textureName = asset.dirName + "[texture " + std::to_string(textureIndex) + "]" + asset.textures[textureIndex].name;

					mat.SetSampledTexture(resourceManager.GetTexture(textureName), mat.GetOcclusionBinding());
					//mat.SetOcclusion(resourceManager.GetTexture(textureName));
				}
				if (material.pbr.baseColorTexture.index != ~0u)
				{
					const int32 textureIndex = material.pbr.baseColorTexture.index;
					std::string textureName = asset.dirName + "[texture " + std::to_string(textureIndex) + "]" + asset.textures[textureIndex].name;

					mat.SetSampledTexture(resourceManager.GetTexture(textureName), mat.GetDiffuseBinding());
					//mat.SetDiffuse(resourceManager.GetTexture(textureName));
				}
				if (material.pbr.metallicRoughnessTexture.index != ~0u)
				{
					const int32 textureIndex = material.pbr.metallicRoughnessTexture.index;
					std::string textureName = asset.dirName + "[texture " + std::to_string(textureIndex) + "]" + asset.textures[textureIndex].name;

					mat.SetSampledTexture(resourceManager.GetTexture(textureName), mat.GetMetalicRoughnessBinding());
					//mat.SetMetalicRoughness(resourceManager.GetTexture(textureName));
				}

				mat.materialPipelineInfo.SetAlphaCutoff(material.alphaCutoff);
				mat.materialInfo.isTransparent = (material.alphaMode == gltf2::Material::AlphaMode::Blend);
				if (material.doubleSided)
					mat.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eNone);
				else
					mat.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eBack);

				mat.SetEmissiveFactor(material.emissiveFactor);
				auto factor = mat.GetEmissiveFactor();
				mat.SetBaseColorFactor(material.pbr.baseColorFactor);
				mat.SetMetallicFactor(material.pbr.metallicFactor);
				mat.SetRoughnessFactor(material.pbr.roughnessFactor);

				//std::string materialName = asset.dirName + "[material " + std::to_string(i) + "]" + asset.materials[i].name;

				materials.push_back(RenderSystem::RegisterMaterial(std::move(mat)));
			}

			return materials;
		}

		void DumpAsset(const gltf2::Asset& asset)
		{
			using namespace gltf2;

			LOG_LOG("asset");
			LOG_LOG("    copyright: %s", asset.metadata.copyright.c_str());
			LOG_LOG("    generator: %s", asset.metadata.generator.c_str());
			LOG_LOG("    version: %s", asset.metadata.version.c_str());
			LOG_LOG("    minVersion: %s", asset.metadata.minVersion.c_str());

			LOG_LOG("    extensionRequired");
			for (auto& it : asset.extensionRequired)
				LOG_LOG("        %s", it.c_str());

			LOG_LOG("    extensionsUsed");
			for (auto& it : asset.extensionsUsed)
				LOG_LOG("        %s", it.c_str());

			LOG_LOG("");

			for (auto& buffer : asset.buffers)
			{
				LOG_LOG("    buffer");
				LOG_LOG("        name %s", buffer.name.c_str());
				LOG_LOG("        byteLength %d", buffer.byteLength);
				LOG_LOG("        uri %s", buffer.uri.c_str());
				LOG_LOG("        data %s", buffer.data);
			}

			LOG_LOG("");

			for (auto& bufferView : asset.bufferViews)
			{
				LOG_LOG("    bufferView");
				LOG_LOG("        name %s", bufferView.name.c_str());
				LOG_LOG("        buffer %d", bufferView.buffer);
				LOG_LOG("        byteLength %d", bufferView.byteLength);
				LOG_LOG("        byteOffset %d", bufferView.byteOffset);
				LOG_LOG("        byteStride %d", bufferView.byteStride);
				LOG_LOG("        byteLength %d", bufferView.byteLength);
				LOG_LOG("        target %d", bufferView.target);
			}

			LOG_LOG("");

			for (auto& accessor : asset.accessors)
			{
				LOG_LOG("    accessor");
				LOG_LOG("        name %s", accessor.name.c_str());
				LOG_LOG("        bufferView %d", accessor.bufferView);
				LOG_LOG("        byteOffset %d", accessor.byteOffset);
				LOG_LOG("        count %d", accessor.count);
				LOG_LOG("        normalized %d", accessor.normalized);
				LOG_LOG("        componentType %d", accessor.componentType);
				LOG_LOG("        type %d", accessor.type);
			}

			auto scene = asset.scenes[asset.scene];

			LOG_LOG("");
			LOG_LOG("    default scene");
			LOG_LOG("        name %s", scene.name.c_str());

			LOG_LOG("");

			for (auto nodeIndex : scene.nodes)
			{
				const Node& node = asset.nodes[nodeIndex];

				LOG_LOG("    node #%d", nodeIndex);
				LOG_LOG("        name %s", node.name.c_str());
				LOG_LOG("        camera %d", node.camera);

				if (node.isTransformMatrix == true)
				{
					Matrix4 matrix(node.matrix);
					LOG_LOG("        matrix %s", matrix.ToString().c_str());
				}
				else
				{
					Quaternion rotation(node.transform.rotation[3], node.transform.rotation[0], node.transform.rotation[1], node.transform.rotation[2]);
					Vector3 scale(node.transform.scale[0], node.transform.scale[1], node.transform.scale[2]);
					Vector3 translation(node.transform.translation[0], node.transform.translation[1], node.transform.translation[2]);

					LOG_LOG("        translation %s", translation.ToString().c_str());
					LOG_LOG("        rotation %s", rotation.ToString().c_str());
					LOG_LOG("        scale %s", scale.ToString().c_str());
				}
				LOG_LOG("        skin %d", node.skin);
				LOG_LOG("        weight (not used yet)");
				LOG_LOG("        mesh %d", node.mesh);

				for (auto childIndex : node.children)
				{
					const Node& node = asset.nodes[childIndex];

					LOG_LOG("        child node #%d", childIndex);
					LOG_LOG("            name %s", node.name.c_str());
					LOG_LOG("            camera %d", node.camera);

					if (node.isTransformMatrix == true)
					{
						Matrix4 matrix(node.matrix);
						LOG_LOG("            matrix %s", matrix.ToString().c_str());
					}
					else
					{
						Quaternion rotation(node.transform.rotation[3], node.transform.rotation[0], node.transform.rotation[1], node.transform.rotation[2]);
						Vector3 scale(node.transform.scale[0], node.transform.scale[1], node.transform.scale[2]);
						Vector3 translation(node.transform.translation[0], node.transform.translation[1], node.transform.translation[2]);

						LOG_LOG("            translation %s", translation.ToString().c_str());
						LOG_LOG("            rotation %s", rotation.ToString().c_str());
						LOG_LOG("            scale %s", scale.ToString().c_str());
					}

					LOG_LOG("            skin %d", node.skin);
					LOG_LOG("            weight (not used yet)");
					LOG_LOG("            mesh %d", node.mesh);
				}
			}

			LOG_LOG("");

			std::list<Mesh> meshes;

			for (auto& mesh : asset.meshes)
			{
				LOG_LOG("    mesh");
				LOG_LOG("        name %s", mesh.name.c_str());
				LOG_LOG("        primitives");
				for (auto& primitive : mesh.primitives)
				{
					LOG_LOG("            mode %d", primitive.mode);
					LOG_LOG("            material %d", primitive.material);
					LOG_LOG("            indices %d", primitive.indices);
					LOG_LOG("            attributes");
					for (auto& attribute : primitive.attributes)
						LOG_LOG("                [%s] = %d", attribute.first.c_str(), attribute.second);

					LOG_LOG("            targets");
					for (auto& target : primitive.targets)
					{
						LOG_LOG("                target");
						for (auto& attribute : target)
							LOG_LOG("                    [%s] = %d", attribute.first.c_str(), attribute.second);
					}
				}
			}

			LOG_LOG("");

			for (auto& image : asset.images)
			{
				LOG_LOG("    image");
				LOG_LOG("        name %s", image.name.c_str());
				LOG_LOG("        mimeType %s", image.mimeType.c_str());
				LOG_LOG("        uri %s", image.uri.c_str());
				LOG_LOG("        bufferView %d", image.bufferView);
				if (image.bufferView != ~0u)
				{
					auto& bufferView = asset.bufferViews[image.bufferView];
					LOG_LOG("            name %s", bufferView.name.c_str());
					LOG_LOG("            buffer %d", bufferView.buffer);
					LOG_LOG("            byteLength %d", bufferView.byteLength);
					LOG_LOG("            byteOffset %d", bufferView.byteOffset);
					LOG_LOG("            byteStride %d", bufferView.byteStride);
					LOG_LOG("            byteLength %d", bufferView.byteLength);
					LOG_LOG("            target %d", bufferView.target);
				}
			}

			LOG_LOG("");

			for (auto& texture : asset.textures)
			{
				LOG_LOG("    texture");
				LOG_LOG("        name %s", texture.name.c_str());
				LOG_LOG("        source %d", texture.source);
				LOG_LOG("        sampler %d", texture.sampler);
			}

			LOG_LOG("");

			for (auto& material : asset.materials)
			{
				LOG_LOG("    material");
				LOG_LOG("        name %s", material.name.c_str());
				LOG_LOG("        alphaCutoff %f", material.alphaCutoff);
				LOG_LOG("        alphaMode %d", material.alphaMode);
				LOG_LOG("        doubleSided %d", material.doubleSided);
				LOG_LOG("        emissiveFactor %f", material.emissiveFactor);
				LOG_LOG("        emissiveTexture %d %d", material.emissiveTexture.index, material.emissiveTexture.texCoord);
				LOG_LOG("        normalTexture %d %d", material.normalTexture.index, material.normalTexture.texCoord);
				LOG_LOG("        occlusionTexture %d %d", material.occlusionTexture.index, material.occlusionTexture.texCoord);
				LOG_LOG("        pbr.baseColorFactor %f", material.pbr.baseColorFactor);
				LOG_LOG("        pbr.baseColorTexture %d %d", material.pbr.baseColorTexture.index, material.pbr.baseColorTexture.texCoord);
				LOG_LOG("        pbr.metallicFactor %f", material.pbr.metallicFactor);
				LOG_LOG("        pbr.metallicRoughnessTexture %d %d", material.pbr.metallicRoughnessTexture.index, material.pbr.metallicRoughnessTexture.texCoord);
				LOG_LOG("        pbr.roughnessFactor %f", material.pbr.roughnessFactor);
			}
		}

		void GLTFImporter::Import(const std::string& filename, Scene& scene, Actor* actor)
		{
			using namespace gltf2;

			LOG_LOG("Loading file \"%s\"\nPlease wait...", filename.c_str());

			Chrono chrono;
			chrono.Play();

			Asset asset;

			try
			{
				asset = load(filename);
				//DumpAsset(asset);

				auto meshes = LoadMeshes(asset, scene);
				auto textures = LoadTextures(asset, scene);

				auto elapsedSeconds = float(chrono.GetElapsedTime<Chrono::Seconds>());
				LOG_LOG("elapsed time: %fs", elapsedSeconds);

				if (actor == nullptr)
					return;

				auto materials = LoadMaterials(asset, scene);

				for (uint32 childNodeIndex : asset.scenes[asset.scene].nodes)
					ProcessNode(childNodeIndex, asset, meshes, materials, scene, actor, nullptr);
			}
			catch (std::exception e)
			{
				LOG_ERROR(e.what());

				return;
			}
			catch (...)
			{
				return;
			}

		}
	}
}