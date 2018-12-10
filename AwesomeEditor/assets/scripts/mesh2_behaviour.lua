import("Input")
import("Maths")

local mesh

function Start()
	mesh = Cast(this:GetParent(), "MeshRenderer")
end

function Update(frameTime)
	if Input.GetKeyPressed(KeyCode.C) then
		mesh:SetActive(not mesh:IsActive())
	end

	-- if Input.GetKeyPressed(KeyCode.R) then
	-- 	rotatorSpeed = -rotatorSpeed
	-- end

	-- if Input.GetKeyDown(KeyCode.Y) then
	-- 	mesh:Rotate_SelfSpace(Vec3(rotatorSpeed * frameTime, 0, 0))
	-- end
	-- if Input.GetKeyDown(KeyCode.G) then
	-- 	mesh:Rotate_SelfSpace(Vec3(0, rotatorSpeed * frameTime, 0))
	-- end
	-- if Input.GetKeyDown(KeyCode.B) then
	-- 	mesh:Rotate_SelfSpace(Vec3(0, 0, rotatorSpeed * frameTime))
	-- end

	mesh:LookAt(Vec3(0, 0, -15), Vector3.Up())
end
