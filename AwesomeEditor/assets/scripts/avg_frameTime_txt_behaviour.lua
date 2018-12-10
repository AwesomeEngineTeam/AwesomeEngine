import("Time")

local text

local frameCountPerSec = 0
local totalFrameTime = 0

function Start()
	text = Cast(this:GetParent(), "Text2D")
end

function Update()
	frameCountPerSec = frameCountPerSec + 1

	totalFrameTime = totalFrameTime + Time.DeltaTime()

	if totalFrameTime >= 1 then
		totalFrameTime = totalFrameTime - 1
		frameCountPerSec = 1
	end

	text:SetContent("Avg frameTime: " .. (totalFrameTime / frameCountPerSec) .. "   Avg FPS: " .. (frameCountPerSec / totalFrameTime))
end
