#include <Awesome/SDK/Resources/ResourceManager.hpp>
#include <Awesome/SDK/Resources/OBJLoader.hpp>
#include <Awesome/SDK/Resources/GLTFImporter.hpp>
//#include <Awesome/SDK/Resources/FBXLoader.hpp>
#include <Awesome/SDK/Resources/TextureLoader.hpp>

#include <Awesome/Renderer/Resource/TexelBuffer.hpp>

#include <Awesome/Core/Algorithm.hpp>

namespace aw
{
	namespace sdk
	{
		// https://stackoverflow.com/questions/2111667/compile-time-string-hashing/2112111#2112111
		unsigned constexpr const_hash(char const *input)
		{
			return *input ?
				static_cast<unsigned int>(*input) + 33 * const_hash(input + 1) :
				5381;
		}

		void ResourceManager::Clear()
		{
			m_textures.clear();
			m_meshes.clear();
			//m_texelBuffers.clear();
			m_shaders.clear();
			m_models.clear();
			m_fontAtlases.clear();

			materialManager.~MaterialManager();
		}

		void ResourceManager::Load(const std::string& filename)
		{
			THROW("Do not call this function");

			size_t dotIndex = filename.find_last_of('.');

			if (dotIndex == std::string::npos)
			{
				LOG_ERROR("Could not load file %s", filename.c_str());
				return;
			}

			std::string extension = filename.c_str() + dotIndex + 1;
			ToLower(extension);

			auto extensionHash = const_hash(extension.c_str());

			switch (extensionHash)
			{
			case const_hash("obj"):
				LOG_LOG("ObjLoader");
				Load<OBJLoader>(filename);
				break;

			case const_hash("jpg"):
			case const_hash("jpeg"):
			case const_hash("png"):
			case const_hash("psd"):
			case const_hash("tga"):
			case const_hash("gif"):
			case const_hash("pic"):
			case const_hash("pgm"):
			case const_hash("ppm"):
				LOG_LOG("TextureLoader");
				//Load<TextureLoader>(filename);
				break;

			case const_hash("skybox"):
				LOG_LOG("TextureLoader");
				//Register(filename.c_str(), TextureLoader::LoadSkybox(filename));
				break;

			default:
				LOG_LOG("Extension not recoginzed (%s)", extension.c_str());
				break;
			}
		}

		//void ResourceManager::Import(const std::string& filename, Actor* actor)
		//{
		//	size_t dotIndex = filename.find_last_of('.');

		//	if (dotIndex == std::string::npos)
		//	{
		//		LOG_ERROR("Could not load file %s", filename.c_str());
		//		return;
		//	}

		//	std::string extension = filename.c_str() + dotIndex + 1;
		//	ToLower(extension);

		//	auto extensionHash = const_hash(extension.c_str());

		//	switch (extensionHash)
		//	{
		//	case const_hash("gltf"):
		//		LOG_LOG("GLTFImporter");
		//		GLTFImporter::Import(filename, *this, actor);
		//		break;

		//		//case const_hash("fbx"):
		//		//	LOG_LOG("FBXLoader");
		//		//	Load<fbx::Loader>(filename);
		//		//	break;

		//	default:
		//		LOG_LOG("Extension not recoginzed (%s)", extension.c_str());
		//		break;
		//	}
		//}

		Mesh* ResourceManager::Register(const std::string& name, Mesh&& mesh)
		{
			auto uMesh = std::make_unique<Mesh>(std::move(mesh));
			Mesh* meshPtr = uMesh.get();

			m_meshes[name] = std::move(uMesh);

			return meshPtr;
		}

		SampledTextureHandle ResourceManager::Register(const std::string& name, SampledTexture&& tex)
		{
			SampledTextureHandle handle(tex);

			m_textures[name] = std::move(tex);

			return handle;
		}

		//TexelBuffer* ResourceManager::Register(const std::string& name, TexelBuffer&& texelBuffer)
		//{
		//	auto uTexelBuffer = std::make_unique<TexelBuffer>(std::move(texelBuffer));
		//	TexelBuffer* texelBufferPtr = uTexelBuffer.get();

		//	m_texelBuffers[name] = std::move(uTexelBuffer);

		//	return texelBufferPtr;
		//}

		Model* ResourceManager::Register(const std::string& name, Model&& model)
		{
			auto uModel = std::make_unique<Model>(std::move(model));
			Model* modelPtr = uModel.get();

			m_models[name] = std::move(uModel);

			return modelPtr;
		}

		Mesh* ResourceManager::GetMesh(const std::string& name)
		{
			auto mesh = m_meshes.find(name);

			if (mesh == m_meshes.end())
				return nullptr;

			return mesh->second.get();
		}

		SampledTextureHandle ResourceManager::GetTexture(const std::string& name)
		{
			auto texture = m_textures.find(name);
			if (texture == m_textures.end())
				return {};

			return SampledTextureHandle(texture->second);
		}

		//TexelBuffer* ResourceManager::GetTexelBuffer(const std::string& name)
		//{
		//	auto texelBuffer = m_texelBuffers.find(name);

		//	if (texelBuffer == m_texelBuffers.end())
		//		return nullptr;

		//	return texelBuffer->second.get();
		//}

		std::vector<char>* ResourceManager::LoadShaderByteCodeFromFile(const std::string& filename)
		{
			auto it = m_shaders.find(filename);
			if (it != m_shaders.end())
			{
				return it->second.get();
			}

			auto byteCode = std::make_unique<std::vector<char>>(LoadShaderByteCodeFromFileUnmanaged(filename));

			auto shaderPtr = byteCode.get();
			m_shaders[filename] = std::move(byteCode);

			return shaderPtr;
		}

		std::vector<char> ResourceManager::LoadShaderByteCodeFromFileUnmanaged(const std::string& filename)
		{
			std::vector<char> byteCode;

			std::ifstream file(filename, std::ios::ate | std::ios::binary);

			if (!file.is_open())
			{
				LOG_WARNING("failed to open file %s", filename.c_str());
				return byteCode;
			}

			size_t fileSize = (size_t)file.tellg();
			byteCode.resize(fileSize);

			file.seekg(0);
			file.read(byteCode.data(), fileSize);
			file.close();

			return byteCode;
		}

		//TexelBuffer* ResourceManager::LoadFontTTF(ImGuiIO& context, const std::string& filename, float font_size)
		//{
		//	auto it = m_texelBuffers.find(filename);
		//	if (it != m_texelBuffers.end())
		//	{
		//		return it->second.get();
		//	}

		//	context.Fonts->AddFontFromFileTTF(filename.c_str(), font_size);

		//	uint8* texels;

		//	auto texelBuffer = std::make_unique<TexelBuffer>();
		//	context.Fonts->GetTexDataAsRGBA32(&texels, reinterpret_cast<int*>(&texelBuffer->width), reinterpret_cast<int*>(&texelBuffer->height));
		//	texelBuffer->channels = 4;

		//	texelBuffer->texels.resize(texelBuffer->width * texelBuffer->height * 4);
		//	memcpy(texelBuffer->texels.data(), texels, texelBuffer->width * texelBuffer->height * 4);

		//	TexelBuffer* texelBufferPtr = texelBuffer.get();
		//	m_texelBuffers[filename] = std::move(texelBuffer);

		//	return texelBufferPtr;
		//}
	}
}