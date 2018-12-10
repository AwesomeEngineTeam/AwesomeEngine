import("Time")

local text

function Start()
	text = Cast(this:GetParent(), "Text2D")
end

function Update()
	text:SetContent("frameTime: " .. Time.DeltaTime() .. "       FPS: " .. (1 / Time.DeltaTime()))
end
