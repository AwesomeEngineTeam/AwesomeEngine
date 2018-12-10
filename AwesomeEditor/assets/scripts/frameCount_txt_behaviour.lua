import("Time")

local text
local frameCount = 0

function Start()
	text = Cast(this:GetParent(), "Text2D")
end

function Update()
	frameCount = frameCount + 1
	text:SetContent("Frame count: " .. frameCount)
end
