import("Input")
import("Maths")

require("config")

local mesh
local sound_emitter

local UpdateMesh
local UpdateSound

function Start()
	mesh = Cast(this:GetParent(), "MeshRenderer")
	sound_emitter = this:GetActor():GetSoundEmitter()
end

function Update(frameTime)
	if Input.GetKeyPressed(KeyCode.R) then
		rotatorSpeed = -rotatorSpeed
	end

	UpdateMesh(frameTime)
	UpdateSound()
end

UpdateMesh = function(frameTime)
	if Input.GetKeyDown(KeyCode.I) then
		mesh:Rotate_SelfSpaceV(Vec3(rotatorSpeed * frameTime, 0, 0))
	end
	if Input.GetKeyDown(KeyCode.J) then
		mesh:Rotate_SelfSpaceV(Vec3(0, rotatorSpeed * frameTime, 0))
	end
	if Input.GetKeyDown(KeyCode.M) then
		mesh:Rotate_SelfSpaceV(Vec3(0, 0, rotatorSpeed * frameTime))
	end
end

UpdateSound = function()
	if Input.GetKeyPressed(KeyCode.Numpad0) then
		sound_emitter:PostEvent("Sonar")
	end
	if Input.GetKeyPressed(KeyCode.Numpad1) then
		sound_emitter:ExecuteActionOnEvent("Sonar", Sound.ActionType.Stop, 1000, Sound.InterpMode.Linear)
	end
end
