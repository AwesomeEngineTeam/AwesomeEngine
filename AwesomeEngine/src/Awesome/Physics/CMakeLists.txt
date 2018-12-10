cmake_minimum_required(VERSION 2.6)
file(GLOB_RECURSE PHYSICS_SRC
    "${PROJECT_SOURCE_DIR}/src/Awesome/Physics/*"
)

add_library(Physics SHARED ${PHYSICS_SRC})
set_target_properties(Physics PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -DAWESOME_PHYSICS_DLL_EXPORTS")

install(TARGETS Physics DESTINATION ${PROJECT_SOURCE_DIR})

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	target_link_libraries(Physics Maths Core LowLevelDEBUG LowLevelAABBDEBUG LowLevelClothDEBUG LowLevelDynamicsDEBUG LowLevelParticlesDEBUG PhysX3DEBUG_x64 PhysX3CharacterKinematicDEBUG_x64 PhysX3CommonDEBUG_x64 PhysX3CookingDEBUG_x64 PhysX3ExtensionsDEBUG PhysX3VehicleDEBUG PsFastXmlDEBUG_x64 PxFoundationDEBUG_x64 PxPvdSDKDEBUG_x64 PxTaskDEBUG_x64 SceneQueryDEBUG SimulationControllerDEBUG)
else()
	target_link_libraries(Physics Maths Core LowLevel LowLevelAABB LowLevelCloth LowLevelDynamics LowLevelParticles PhysX3_x64 PhysX3CharacterKinematic_x64 PhysX3Common_x64 PhysX3Cooking_x64 PhysX3Extensions PhysX3Vehicle PsFastXml_x64 PxFoundation_x64 PxPvdSDK_x64 PxTask_x64 SceneQuery SimulationController)
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
	set(PHYSICS_LIBS
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Physics.dll"
	)
else()
	set(PHYSICS_LIBS
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Physics.dll"
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Physics.ilk"
		"${CMAKE_SOURCE_DIR}/AwesomeEngine/build/${CMAKE_BUILD_TYPE}/Physics.pdb"
	)
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(PHYSX_DLL
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/nvToolsExt64_1.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3CharacterKinematicDEBUG_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3CommonDEBUG_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3CookingDEBUG_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3DEBUG_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3GpuDEBUG_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysXDevice64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PxFoundationDEBUG_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PxPvdSDKDEBUG_x64.dll"
	)
else()
	set(PHYSX_DLL
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/nvToolsExt64_1.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3CharacterKinematic_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3Common_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3Cooking_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysX3Gpu_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PhysXDevice64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PxFoundation_x64.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PxPvdSDK_x64.dll"
	)
endif()

add_custom_command(TARGET Physics POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PHYSICS_LIBS} "${CMAKE_SOURCE_DIR}/AwesomeEditor/build/${CMAKE_BUILD_TYPE}")

add_custom_command(OUTPUT ${PHYSX_DLL}
	COMMAND  ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/extlibs/sharedlib/${CMAKE_BUILD_TYPE}/physx" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

add_custom_target(copyPhysicsExtlibs DEPENDS ${PHYSX_DLL})
add_dependencies(Physics copyPhysicsExtlibs)