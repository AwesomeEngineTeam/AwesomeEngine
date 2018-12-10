
-- Bindings:	-level

function OnTrigger(comp1, comp2)
	if comp1:name() ~= "AwesomeDoorCollider" or comp2 == nil or comp2:name() ~= "PlayerBody" then
		return
	end

	level:GetActor():SetActive(false)
end
