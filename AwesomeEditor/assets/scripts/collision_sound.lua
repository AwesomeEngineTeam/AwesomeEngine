import("Physics")

-- Bindings:	- placement_impact

-- Declare and set this to anything but nil to disable cast related exceptions
DisableCastExceptions = 1

local sound_emitter

function Start()
	sound_emitter = this:GetActor():GetSoundEmitter()
end

function OnCollision(comp1, comp2)
	if placement_impact == false then
		placement_impact = true

		return
	end

	if comp1:name() ~= this:GetActor():name() then
		return
	end

	local rb = Cast(comp1, "Rigidbody")
	local rb2 = Cast(comp2, "Rigidbody")

	local rb2Vel = 0
	if rb2 then
		rb2Vel = rb2:linearVelocity():Norm()
	end

	sound_emitter:SetRTPCValue("ImpactForce", rb:linearVelocity():Norm() + rb2Vel, 0, Sound.InterpMode.Linear, false)

	if comp2:name() == "RefractObject" then
		sound_emitter:PostEvent("GlassImpactNotSmashed")
	else
		sound_emitter:PostEvent("PlasticImpact")
	end
end
