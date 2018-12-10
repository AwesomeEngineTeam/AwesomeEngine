import("Time")
import("Maths")

local light
local lightPos

function Start()
	light = this:GetActor()
end

function Update()
	lightPos = Vec3(math.cos(Time.GlobalTime()) * 5,
					math.sin(Time.GlobalTime()) * 5,
					-5)

	light:SetGlobalPosition(lightPos)
end
