import("Input")
import("Physics")

local isActivated = false;

function OnTrigger(comp1, comp2)
	
	if isActivated == true then
		return
	end

	isActivated = true;

	print("Win")
end