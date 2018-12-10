import("Maths")

print("hop")

Start = function()
	local textures = {}
	local materials = {}
	local meshes = {}
	local actors = {}
	local components = {}

	textures[0] = 0
	textures[1] = 1
	textures[2] = 2
	textures[3] = 3
	textures[4] = 4
	textures[5] = 5
	textures[6] = 6
	textures[7] = 7
	textures[8] = 8
	textures[9] = 9
	textures[10] = 10
	meshes[0] = 0
	meshes[1] = 1
	meshes[2] = 2
	meshes[3] = 3
	meshes[4] = 4
	meshes[5] = 5
	meshes[6] = 6
	meshes[7] = 7
	meshes[8] = 8
	meshes[9] = 9
	meshes[10] = 10
	meshes[11] = 11
	meshes[12] = 12
	meshes[13] = 13

	textures[14] = Editor.AddAssetItem("textures/bricks2.jpg")

	materials[20] = Editor.AddUnlitMaterial("unlitMaterial1.mat")
	Editor.SetMaterialDiffuse(materials[20], 14)

	actors["00000215E73F1A70"] = Editor.InstantiateActorOrphan("AAAAAAAAAAAAH")
		components["00000215E5A7C400"] = Editor.InstantiateMeshRendererOrphan(actors["00000215E73F1A70"])
		components["00000215E5A7C400"]:SetMaterial(materials[20])
		components["00000215E5A7C400"]:SetMesh(meshes[2])
		--components["00000215E5A7C400"]:SetMeshPosition(Vec3(0, 0, 0))
		--components["00000215E5A7C400"]:SetMeshRotation(Quat(1, 0, 0, 0))
		--components["00000215E5A7C400"]:SetMeshScale(Vec3(1, 1, 1))
		components["00000215E5A7C400"]:SetLocalPosition(Vec3(0, 0, 0))
		components["00000215E5A7C400"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E5A7C400"]:SetScale(1)

	actors["00000215E73F6770"] = Editor.InstantiateActorOrphan("Debug Text")
		-- component->typeId = 9437184
		components["00000215E7546D10"] = Editor.InstantiateScript(actors["00000215E73F6770"], components["00000215E72301E0"], "assets/scripts/frameTime_txt_behaviour.lua")
		components["00000215E7546D10"]:SetLocalPosition(Vec3(0, 0, 0))
		components["00000215E7546D10"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E7546D10"]:SetScale(1)
		-- component->typeId = 9437184
		components["00000215E74B89D0"] = Editor.InstantiateScript(actors["00000215E73F6770"], components["00000215E72346E0"], "assets/scripts/avg_frameTime_txt_behaviour.lua")
		components["00000215E74B89D0"]:SetLocalPosition(Vec3(0, 0, 0))
		components["00000215E74B89D0"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E74B89D0"]:SetScale(1)
		-- component->typeId = 9437184
		components["00000215E75C9990"] = Editor.InstantiateScript(actors["00000215E73F6770"], components["00000215E722FD90"], "assets/scripts/frameCount_txt_behaviour.lua")
		components["00000215E75C9990"]:SetLocalPosition(Vec3(0, 0, 0))
		components["00000215E75C9990"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E75C9990"]:SetScale(1)

	actors["00000215E73F5220"] = Editor.InstantiateActorOrphan("Moving Light")
		components["00000215E75C9CF0"] = Editor.InstantiateScriptOrphan(actors["00000215E73F5220"], "assets/scripts/light.lua")
		components["00000215E75C9CF0"]:SetLocalPosition(Vec3(0, 0, 0))
		components["00000215E75C9CF0"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E75C9CF0"]:SetScale(1)
		-- component->typeId = 3145729

	actors["00000215E73F5430"] = Editor.InstantiateActorOrphan("Static Light")
		-- component->typeId = 3145729
		-- component->typeId = 3145729
		-- component->typeId = 3145730

	actors["00000215E73F5C70"] = Editor.InstantiateActorOrphan("Cube 1")
		components["00000215E5A7D2C0"] = Editor.InstantiateMeshRendererOrphan(actors["00000215E73F5C70"])
		components["00000215E5A7D2C0"]:SetMaterial(materials[21])
		components["00000215E5A7D2C0"]:SetMesh(meshes[2])
		--components["00000215E5A7D2C0"]:SetMeshPosition(Vec3(0, 0, 0))
		--components["00000215E5A7D2C0"]:SetMeshRotation(Quat(1, 0, 0, 0))
		--components["00000215E5A7D2C0"]:SetMeshScale(Vec3(1, 2, 1))
		components["00000215E5A7D2C0"]:SetLocalPosition(Vec3(-1.5, 0, -5))
		components["00000215E5A7D2C0"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E5A7D2C0"]:SetScale(1)

	actors["00000215E73F6820"] = Editor.InstantiateActorOrphan("Sphere 1")
		components["00000215E5A7DDD0"] = Editor.InstantiateMeshRendererOrphan(actors["00000215E73F6820"])
		components["00000215E5A7DDD0"]:SetMaterial(materials[21])
		components["00000215E5A7DDD0"]:SetMesh(meshes[4])
		--components["00000215E5A7DDD0"]:SetMeshPosition(Vec3(0, 0, 0))
		--components["00000215E5A7DDD0"]:SetMeshRotation(Quat(1, 0, 0, 0))
		--components["00000215E5A7DDD0"]:SetMeshScale(Vec3(1, 1, 1))
		components["00000215E5A7DDD0"]:SetLocalPosition(Vec3(0, 0, -5))
		components["00000215E5A7DDD0"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E5A7DDD0"]:SetScale(1)
		components["00000215E75CE300"] = Editor.InstantiateScript(actors["00000215E73F6820"], components["00000215E5A7DDD0"], "assets/scripts/mesh1_behaviour.lua")
		components["00000215E75CE300"]:SetLocalPosition(Vec3(0, 0, 0))
		components["00000215E75CE300"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E75CE300"]:SetScale(1)
		components["00000215E5A7CB60"] = Editor.InstantiateMeshRenderer(actors["00000215E73F6820"], components["00000215E5A7DDD0"])
		components["00000215E5A7CB60"]:SetMaterial(materials[25])
		components["00000215E5A7CB60"]:SetMesh(meshes[4])
		--components["00000215E5A7CB60"]:SetMeshPosition(Vec3(0, 0, 0))
		--components["00000215E5A7CB60"]:SetMeshRotation(Quat(1, 0, 0, 0))
		--components["00000215E5A7CB60"]:SetMeshScale(Vec3(1, 1, 1))
		components["00000215E5A7CB60"]:SetLocalPosition(Vec3(0, 0, -5))
		components["00000215E5A7CB60"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E5A7CB60"]:SetScale(1)
		components["00000215E75CE660"] = Editor.InstantiateScript(actors["00000215E73F6820"], components["00000215E5A7CB60"], "assets/scripts/mesh2_behaviour.lua")
		components["00000215E75CE660"]:SetLocalPosition(Vec3(0, 0, 0))
		components["00000215E75CE660"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E75CE660"]:SetScale(1)
		components["00000215E5A7F040"] = Editor.InstantiateMeshRenderer(actors["00000215E73F6820"], components["00000215E5A7CB60"])
		components["00000215E5A7F040"]:SetMaterial(materials[26])
		components["00000215E5A7F040"]:SetMesh(meshes[4])
		--components["00000215E5A7F040"]:SetMeshPosition(Vec3(0, 0, 0))
		--components["00000215E5A7F040"]:SetMeshRotation(Quat(1, 0, 0, 0))
		--components["00000215E5A7F040"]:SetMeshScale(Vec3(1, 1, 1))
		components["00000215E5A7F040"]:SetLocalPosition(Vec3(0, 0, -5))
		components["00000215E5A7F040"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E5A7F040"]:SetScale(1)
		components["00000215E752FBA0"] = Editor.InstantiateScript(actors["00000215E73F6820"], components["00000215E5A7F040"], "assets/scripts/mesh3_behaviour.lua")
		components["00000215E752FBA0"]:SetLocalPosition(Vec3(0, 0, 0))
		components["00000215E752FBA0"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E752FBA0"]:SetScale(1)
		components["00000215E5A7CF10"] = Editor.InstantiateMeshRenderer(actors["00000215E73F6820"], components["00000215E5A7F040"])
		components["00000215E5A7CF10"]:SetMaterial(materials[27])
		components["00000215E5A7CF10"]:SetMesh(meshes[4])
		--components["00000215E5A7CF10"]:SetMeshPosition(Vec3(0, 0, 0))
		--components["00000215E5A7CF10"]:SetMeshRotation(Quat(1, 0, 0, 0))
		--components["00000215E5A7CF10"]:SetMeshScale(Vec3(1, 1, 1))
		components["00000215E5A7CF10"]:SetLocalPosition(Vec3(0, 0, -5))
		components["00000215E5A7CF10"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E5A7CF10"]:SetScale(1)
		components["00000215E752FF00"] = Editor.InstantiateScript(actors["00000215E73F6820"], components["00000215E5A7CF10"], "assets/scripts/mesh4_behaviour.lua")
		components["00000215E752FF00"]:SetLocalPosition(Vec3(0, 0, 0))
		components["00000215E752FF00"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E752FF00"]:SetScale(1)
		-- component->typeId = 8388608
		-- component->typeId = 11534336

	actors["00000215E73F4CA0"] = Editor.InstantiateActorOrphan("Hemisphere")
		components["00000215E5A7DA20"] = Editor.InstantiateMeshRendererOrphan(actors["00000215E73F4CA0"])
		components["00000215E5A7DA20"]:SetMaterial(materials[21])
		components["00000215E5A7DA20"]:SetMesh(meshes[10])
		--components["00000215E5A7DA20"]:SetMeshPosition(Vec3(0, 0, 0))
		--components["00000215E5A7DA20"]:SetMeshRotation(Quat(1, 0, 0, 0))
		--components["00000215E5A7DA20"]:SetMeshScale(Vec3(1, 1, 1))
		components["00000215E5A7DA20"]:SetLocalPosition(Vec3(0, 0, -10))
		components["00000215E5A7DA20"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E5A7DA20"]:SetScale(1)

	actors["00000215E73F40F0"] = Editor.InstantiateActorOrphan("Cone")
		components["00000215E5A7F3F0"] = Editor.InstantiateMeshRendererOrphan(actors["00000215E73F40F0"])
		components["00000215E5A7F3F0"]:SetMaterial(materials[21])
		components["00000215E5A7F3F0"]:SetMesh(meshes[9])
		--components["00000215E5A7F3F0"]:SetMeshPosition(Vec3(0, 0, 0))
		--components["00000215E5A7F3F0"]:SetMeshRotation(Quat(1, 0, 0, 0))
		--components["00000215E5A7F3F0"]:SetMeshScale(Vec3(1, 1, 1))
		components["00000215E5A7F3F0"]:SetLocalPosition(Vec3(0, -1.2, -5))
		components["00000215E5A7F3F0"]:SetLocalRotation(Quat(0, 0, 0, 1))
		components["00000215E5A7F3F0"]:SetScale(1)

	actors["00000215E73F52D0"] = Editor.InstantiateActorOrphan("Cylindre")
		components["00000215E5A7E180"] = Editor.InstantiateMeshRendererOrphan(actors["00000215E73F52D0"])
		components["00000215E5A7E180"]:SetMaterial(materials[21])
		components["00000215E5A7E180"]:SetMesh(meshes[8])
		--components["00000215E5A7E180"]:SetMeshPosition(Vec3(0, 0, 0))
		--components["00000215E5A7E180"]:SetMeshRotation(Quat(1, 0, 0, 0))
		--components["00000215E5A7E180"]:SetMeshScale(Vec3(1, 1, 1))
		components["00000215E5A7E180"]:SetLocalPosition(Vec3(1.5, 0, -5))
		components["00000215E5A7E180"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E5A7E180"]:SetScale(1)

	actors["00000215E73F5900"] = Editor.InstantiateActorOrphan("Capsule")
		components["00000215E5A7E530"] = Editor.InstantiateMeshRendererOrphan(actors["00000215E73F5900"])
		components["00000215E5A7E530"]:SetMaterial(materials[21])
		components["00000215E5A7E530"]:SetMesh(meshes[6])
		--components["00000215E5A7E530"]:SetMeshPosition(Vec3(0, 0, 0))
		--components["00000215E5A7E530"]:SetMeshRotation(Quat(1, 0, 0, 0))
		--components["00000215E5A7E530"]:SetMeshScale(Vec3(1, 1, 1))
		components["00000215E5A7E530"]:SetLocalPosition(Vec3(0, 2, -5))
		components["00000215E5A7E530"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E5A7E530"]:SetScale(1)

	actors["00000215E73F6090"] = Editor.InstantiateActorOrphan("Player")
		-- component->typeId = 1048576
		-- component->typeId = 3145729
		-- component->typeId = 7340032
		components["00000215E75D0FB0"] = Editor.InstantiateScript(actors["00000215E73F6090"], components["00000215EBCF1260"], "assets/scripts/player.lua")
		components["00000215E75D0FB0"]:SetLocalPosition(Vec3(0, 0, 0))
		components["00000215E75D0FB0"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E75D0FB0"]:SetScale(1)

	actors["00000215E73F4E00"] = Editor.InstantiateActorOrphan("Dynamic Body")
		-- component->typeId = 4194304
		components["00000215E76205D0"] = Editor.InstantiateMeshRenderer(actors["00000215E73F4E00"], components["00000215EBCEF6D0"])
		components["00000215E76205D0"]:SetMaterial(materials[0])
		components["00000215E76205D0"]:SetMesh(meshes[6])
		--components["00000215E76205D0"]:SetMeshPosition(Vec3(0, 0, 0))
		--components["00000215E76205D0"]:SetMeshRotation(Quat(1, 0, 0, 0))
		--components["00000215E76205D0"]:SetMeshScale(Vec3(1, 1, 1))
		components["00000215E76205D0"]:SetLocalPosition(Vec3(0, 0, 0))
		components["00000215E76205D0"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E76205D0"]:SetScale(1)

	actors["00000215E73F43B0"] = Editor.InstantiateActorOrphan("Static Body")
		-- component->typeId = 5242883

	actors["00000215E73F5850"] = Editor.InstantiateActorOrphan("Trigger Body")
		-- component->typeId = 5242882

	actors["00000215E73F6350"] = Editor.InstantiateActorOrphan("Plane Body")
		-- component->typeId = 5242881

	actors["00000215E73F4F60"] = Editor.InstantiateActorOrphan("GUI Actor")

	actors["00000215E73F6AE0"] = Editor.InstantiateActorOrphan("Sound Actor")
		components["00000215E7544AB0"] = Editor.InstantiateScriptOrphan(actors["00000215E73F6AE0"], "assets/scripts/sound.lua")
		components["00000215E7544AB0"]:SetLocalPosition(Vec3(0, 0, 0))
		components["00000215E7544AB0"]:SetLocalRotation(Quat(1, 0, 0, 0))
		components["00000215E7544AB0"]:SetScale(1)
		-- component->typeId = 8388608


end
