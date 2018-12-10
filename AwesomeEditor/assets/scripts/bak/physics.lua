import("Input")
import("Physics")

-- Bindings : - camera
--			  - reach

local Raycast
local ToRigidbody
local ApplyForce
local ApplyTorque

function Update(frameTime)
	if Input.GetKeyPressed(KeyCode.P) then
		local hit = Raycast(20)
		if hit:comp() then
			print("Raycast hit: Distance: ", hit:distance())
		else
			print("Raycast did not hit")
		end
	end

	if Input.GetKeyPressed(KeyCode.L) then
		ApplyForce(3 * Vector3.Up())
	end

	if Input.GetKeyPressed(KeyCode.K) then
		ApplyTorque(3 * Vector3.Up())
	end

	if Input.GetKeyPressed(KeyCode.Z) then
		local hit = Raycast()
		if hit:comp() then
			local rb = ToRigidbody(hit:comp())
			rb:AddForce(3 * (rb:GlobalPosition() - camera:GlobalPosition()), ForceMode.Impulse)
		end
	end
end

Raycast = function(reach)
	return Physics.Raycast(camera:GlobalPosition(), camera:GlobalRotation() * -Vector3.Forward(), reach)
end

ToRigidbody = function(component)
	return Cast(component, "Rigidbody")
end

ApplyForce = function(force)
	local hit = Raycast()
	if hit:comp() then
		local rb = ToRigidbody(hit:comp())
		rb:AddForce(force, ForceMode.Impulse)
	end
end

ApplyTorque = function(torque)
	local hit = Raycast()
	if hit:comp() then
		local rb = ToRigidbody(hit:comp())
		rb:AddTorque(torque, ForceMode.Impulse)
	end
end
