#pragma once

#ifndef AWESOME_PHYSICS_GUARD
#define AWESOME_PHYSICS_GUARD

#include <Awesome/Physics/Config.hpp>
#include <Awesome/Physics/Material.hpp>
#include <Awesome/Physics/RaycastHit.hpp>
#include <Awesome/Physics/TolerancesScale.hpp>

#include <Awesome/Physics/Control/Pvd.hpp>
#include <Awesome/Physics/Control/Context.hpp>
#include <Awesome/Physics/Control/Cooking.hpp>
#include <Awesome/Physics/Control/Instance.hpp>
#include <Awesome/Physics/Control/Extensions.hpp>
#include <Awesome/Physics/Control/Foundation.hpp>
#include <Awesome/Physics/Control/GPUContext.hpp>

#include <Awesome/Physics/Body/Rigidbody.hpp>
#include <Awesome/Physics/Body/RigidbodyDynamic.hpp>
#include <Awesome/Physics/Body/RigidbodyStatic.hpp>

#include <Awesome/Physics/Descriptor/SceneDescriptor.hpp>
#include <Awesome/Physics/Descriptor/HeightFieldSample.hpp>
#include <Awesome/Physics/Descriptor/ConvexMeshDescriptor.hpp>
#include <Awesome/Physics/Descriptor/GPUContextDescriptor.hpp>
#include <Awesome/Physics/Descriptor/HeightFieldDescriptor.hpp>
#include <Awesome/Physics/Descriptor/TriangleMeshDescriptor.hpp>

#include <Awesome/Physics/Geometry/Shape.hpp>
#include <Awesome/Physics/Geometry/Geometry.hpp>
#include <Awesome/Physics/Geometry/BoxGeometry.hpp>
#include <Awesome/Physics/Geometry/PlaneGeometry.hpp>
#include <Awesome/Physics/Geometry/SphereGeometry.hpp>
#include <Awesome/Physics/Geometry/CapsuleGeometry.hpp>
#include <Awesome/Physics/Geometry/ConvexMeshGeometry.hpp>
#include <Awesome/Physics/Geometry/HeightFieldGeometry.hpp>
#include <Awesome/Physics/Geometry/TriangleMeshGeometry.hpp>

#include <Awesome/Physics/Mesh/MeshScale.hpp>
#include <Awesome/Physics/Mesh/ConvexMesh.hpp>
#include <Awesome/Physics/Mesh/HeightField.hpp>
#include <Awesome/Physics/Mesh/TriangleMesh.hpp>

#endif // GUARD