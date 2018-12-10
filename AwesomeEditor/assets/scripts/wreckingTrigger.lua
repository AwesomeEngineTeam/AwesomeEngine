import("Input")
import("Time")
import("Physics")

-- Bindings :	- wreckingBall
--				- portal
--				- impulseForce

local sound_emitter = this:GetActor():GetSoundEmitter()

function OnTrigger(comp1, comp2)

	portal:SetActive(true)
	sound_emitter:PostEvent("RiddleSolved")

	-- CubeHeap
	local cubes = Scene.FindActorsWithName("CubeHeap");

	for cubeCount = 1, #cubes do
		local body = Cast(cubes[cubeCount]:FindComponentWithName("CubeHeap"), "Rigidbody")
		body:SetKinematic(false)
	end

	-- WreckingBall
	wreckingBall:GetActor():SetActive(true)

	local body = Cast(wreckingBall, "Rigidbody")

	body:SetKinematic(false)
	body:AddForce(impulseForce * Vector3.Forward(), ForceMode.Impulse)

	this:GetActor():Destroy()
end
