#pragma once

#ifndef AWESOME_SDK_RESOURCE_MANAGER_GUARD
#define AWESOME_SDK_RESOURCE_MANAGER_GUARD

#include <Awesome/SDK/Config.hpp>
#include <Awesome/SDK/Scene/Actor.hpp>

#include <Awesome/Renderer/Resource/MaterialManager.hpp>
#include <Awesome/Renderer/Resource/Material.hpp>
#include <Awesome/Renderer/Resource/Mesh.hpp>
#include <Awesome/Renderer/Resource/Model.hpp>
#include <Awesome/Renderer/Pipeline/StaticVertex.hpp>

#include <Awesome/Core/Logger/Logger.hpp>

#include <Awesome/Maths/Vector2.hpp>
#include <Awesome/Maths/Vector3.hpp>

#include <imgui/imgui.h>

#include <type_traits>
#include <utility>
#include <unordered_map>
#include <memory>
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>

namespace aw
{
	struct TexelBuffer;

	namespace sdk
	{
		class ResourceManager
		{
			std::unordered_map<std::string, SampledTexture> m_textures;
			std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshes;
			//std::unordered_map<std::string, std::unique_ptr<TexelBuffer>> m_texelBuffers;
			std::unordered_map<std::string, std::unique_ptr<std::vector<char>>> m_shaders;
			std::unordered_map<std::string, std::unique_ptr<Model>> m_models;
			std::unordered_map<std::string, std::unique_ptr<ImFontAtlas>> m_fontAtlases;

		public:
			MaterialManager materialManager{};

			ResourceManager() = default;
			ResourceManager(const ResourceManager&) = delete;
			ResourceManager(ResourceManager&&) = default;
			~ResourceManager() = default;

			AWESOME_SDK_API void Clear();

			template<typename Loader, typename ...Args>
			inline auto Load(const std::string& filename, Args&&... args);

			AWESOME_SDK_API void Load(const std::string& filename);

			//AWESOME_SDK_API void Import(const std::string& filename, Actor* actor = nullptr);

			AWESOME_SDK_API Mesh* Register(const std::string& name, Mesh&&);
			AWESOME_SDK_API SampledTextureHandle Register(const std::string& name, SampledTexture&&);
			//AWESOME_SDK_API TexelBuffer* Register(const std::string& name, TexelBuffer&&);
			AWESOME_SDK_API Model* Register(const std::string& name, Model&&);

			AWESOME_SDK_API Mesh* GetMesh(const std::string& name);
			AWESOME_SDK_API SampledTextureHandle GetTexture(const std::string& name);
			//AWESOME_SDK_API TexelBuffer* GetTexelBuffer(const std::string& name);

			// Shader
		public:
			AWESOME_SDK_API std::vector<char>* LoadShaderByteCodeFromFile(const std::string& filename);
			AWESOME_SDK_API std::vector<char> LoadShaderByteCodeFromFileUnmanaged(const std::string& filename);

			//Font
			//AWESOME_SDK_API TexelBuffer* LoadFontTTF(ImGuiIO& context, const std::string& filename, float font_size = 14.f);

			ResourceManager& operator=(const ResourceManager&) = delete;
			ResourceManager& operator=(ResourceManager&&) = default;
		};
	}
}

#include <Awesome/SDK/Resources/ResourceManager.inl>

#endif //GUARD