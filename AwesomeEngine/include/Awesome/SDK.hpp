#pragma once

#ifndef AWESOME_SDK_GUARD
#define AWESOME_SDK_GUARD

#include <Awesome/SDK/Time.hpp>

#include <Awesome/SDK/ThreadManager.hpp>

#include <Awesome/SDK/Scene/Scene.hpp>
#include <Awesome/SDK/Scene/SceneManager.hpp>
#include <Awesome/SDK/Scene/Actor.hpp>

#include <Awesome/SDK/System/PhysicalSystem.hpp>
#include <Awesome/SDK/System/RenderSystem.hpp>

#include <Awesome/SDK/Components/Component.hpp>

#include <Awesome/SDK/Components/Lua/Script.hpp>

#include <Awesome/SDK/Components/Render/Camera.hpp>
#include <Awesome/SDK/Components/Render/DirectionalLight.hpp>
#include <Awesome/SDK/Components/Render/Light.hpp>
#include <Awesome/SDK/Components/Render/MeshRenderer.hpp>
#include <Awesome/SDK/Components/Render/PointLight.hpp>
#include <Awesome/SDK/Components/Render/Skybox.hpp>

#include <Awesome/SDK/Components/Physical/PlaneCollider.hpp>
#include <Awesome/SDK/Components/Physical/BoxCollider.hpp>
#include <Awesome/SDK/Components/Physical/CapsuleCollider.hpp>
#include <Awesome/SDK/Components/Physical/Rigidbody.hpp>
#include <Awesome/SDK/Components/Physical/SphereCollider.hpp>

#include <Awesome/SDK/Lua/Libraries.hpp>
#include <Awesome/SDK/Lua/LuaScript.hpp>

#include <Awesome/SDK/Resources/FBXLoader.hpp>
#include <Awesome/SDK/Resources/FontLoader.hpp>
#include <Awesome/SDK/Resources/GLTFImporter.hpp>
#include <Awesome/SDK/Resources/OBJLoader.hpp>
#include <Awesome/SDK/Resources/ResourceManager.hpp>
#include <Awesome/SDK/Resources/TextureLoader.hpp>


#include <Awesome/SDK/EngineRRID.hpp>
#include <Awesome/SDK/BaseApplication.hpp>
#include <Awesome/SDK/WindowApplication.hpp>

#endif //GUARD