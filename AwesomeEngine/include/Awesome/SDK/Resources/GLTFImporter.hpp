#pragma once

#ifndef AWESOME_SDK_GLTF_IMPORTER_GUARD
#define AWESOME_SDK_GLTF_IMPORTER_GUARD

#include <Awesome/SDK/Config.hpp>
#include <Awesome/SDK/Scene/Scene.hpp>

namespace aw
{
	namespace sdk
	{
		class GLTFImporter
		{
		public:
			GLTFImporter() = delete;
			~GLTFImporter() = delete;

			AWESOME_SDK_API static void Import(const std::string& filename, Scene& scene, class Actor* actor = nullptr);
		};
	}
}

//#include <Awesome/Renderer/GLTFImporter.inl>

#endif //GUARD