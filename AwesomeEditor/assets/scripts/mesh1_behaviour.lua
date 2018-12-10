import("Input")
import("Maths")

require("config")

local mesh

function Start()
	mesh = Cast(this:GetParent(), "MeshRenderer")
end

function Update(frameTime)
	if Input.GetKeyPressed(KeyCode.R) then
		rotatorSpeed = -rotatorSpeed
	end

	if Input.GetKeyDown(KeyCode.T) then
		mesh:Rotate_SelfSpaceV(Vec3(rotatorSpeed * frameTime, 0, 0))
	end
	if Input.GetKeyDown(KeyCode.F) then
		mesh:Rotate_SelfSpaceV(Vec3(0, rotatorSpeed * frameTime, 0))
	end
	if Input.GetKeyDown(KeyCode.V) then
		mesh:Rotate_SelfSpaceV(Vec3(0, 0, rotatorSpeed * frameTime))
	end
end
