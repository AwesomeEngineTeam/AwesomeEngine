#pragma once

#ifndef AWESOME_PHYSICS_CONVEX_MESH_DESCRIPTOR_GUARD
#define AWESOME_PHYSICS_CONVEX_MESH_DESCRIPTOR_GUARD

#include <vector>

#include <physX/cooking/PxConvexMeshDesc.h>

#include <Awesome/Core/Types.hpp>
#include <Awesome/Maths/Vector3.hpp>

namespace aw
{
	namespace ph
	{
		enum class ConvexFlag : uint16
		{
			Indices16Bits = physx::PxConvexFlag::Enum::e16_BIT_INDICES,

			ComputeConvex = physx::PxConvexFlag::Enum::eCOMPUTE_CONVEX,

			CheckZeroAreaTriangles = physx::PxConvexFlag::Enum::eCHECK_ZERO_AREA_TRIANGLES,

			InflateConvex = physx::PxConvexFlag::Enum::eINFLATE_CONVEX,

			Quantize_Input = physx::PxConvexFlag::Enum::eQUANTIZE_INPUT,

			DisableMeshValidation = physx::PxConvexFlag::Enum::eDISABLE_MESH_VALIDATION,

			PlaneShifting = physx::PxConvexFlag::Enum::ePLANE_SHIFTING,

			FastInertiaComputation = physx::PxConvexFlag::Enum::eFAST_INERTIA_COMPUTATION,

			GPUCompatible = physx::PxConvexFlag::Enum::eGPU_COMPATIBLE,

			ShiftVertices = physx::PxConvexFlag::Enum::eSHIFT_VERTICES
		};
		inline uint16 operator|(ConvexFlag flag1, ConvexFlag flag2);

		class ConvexMeshDescriptor
		{
			physx::PxConvexMeshDesc m_handle;

		public:
			ConvexMeshDescriptor() = default;
			inline ConvexMeshDescriptor(const std::vector<Vector3>& vertices, const std::vector<uint32>& indices, uint16 flags = ConvexFlag::ComputeConvex | ConvexFlag::CheckZeroAreaTriangles);
			inline ConvexMeshDescriptor(ConvexMeshDescriptor&& other);
			inline ConvexMeshDescriptor(const ConvexMeshDescriptor& other);
			~ConvexMeshDescriptor() = default;

			inline const physx::PxConvexMeshDesc& Get() const;

			inline bool IsValid() const;

			inline const std::vector<Vector3>& Vertices() const;
			inline const std::vector<float>& Indices() const;

			inline ConvexFlag& Flags();
			inline const ConvexFlag& Flags() const;

			inline void Create(const std::vector<Vector3>& vertices, const std::vector<uint32>& indices, uint16 flags = ConvexFlag::ComputeConvex | ConvexFlag::CheckZeroAreaTriangles);

			inline ConvexMeshDescriptor& operator=(ConvexMeshDescriptor&& rhs);
			inline ConvexMeshDescriptor& operator=(const ConvexMeshDescriptor& rhs);
		};
	}
}

#include <Awesome/Physics/Descriptor/ConvexMeshDescriptor.inl>

#endif // GUARD