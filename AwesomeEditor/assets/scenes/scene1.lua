print("hop")

Start = function()
	print("start lol");
	local actor = Editor.InstantiateActorOrphan("AAAAAAAAAAAAH")
	print("actor created");
	local meshRenderer = Editor.InstantiateMeshRendererOrphan(actor)


	local bricksIndex = Editor.AddAssetItem("textures/bricks2.jpg")

	print("bricks index = " .. bricksIndex)

	local materialIndex = Editor.AddUnlitMaterial("unlitMaterial1")

	Editor.SetMaterialDiffuse(materialIndex, bricksIndex)

	print("materialIndex = " .. materialIndex)
	print("Cube = ", PrimitivesIndex.Cube)

	meshRenderer:SetMaterial(materialIndex)
	meshRenderer:SetMesh(PrimitivesIndex.Cube)
	print("meshRenderer:GetMesh() = ", meshRenderer:GetMesh())
end