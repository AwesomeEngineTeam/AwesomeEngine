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

	if Input.GetKeyDown(KeyCode.U) then
		mesh:Rotate_SelfSpaceV(Vec3(rotatorSpeed * frameTime, 0, 0))
	end
	if Input.GetKeyDown(KeyCode.H) then
		mesh:Rotate_SelfSpaceV(Vec3(0, rotatorSpeed * frameTime, 0))
	end
	if Input.GetKeyDown(KeyCode.N) then
		mesh:Rotate_SelfSpaceV(Vec3(0, 0, rotatorSpeed * frameTime))
	end
end
