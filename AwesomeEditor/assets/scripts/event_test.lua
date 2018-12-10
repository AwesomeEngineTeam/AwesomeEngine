function OnTrigger(comp1, comp2)
	if comp1:name() ~= "AwesomeDoorCollider" then
		return
	end

	print("OnTrigger")
	print(comp1:name())
	print(comp2:name())

	-- this:GetActor():Destroy()
end

function OnCollision(comp1, comp2)
	if comp1:name() ~= "AwesomeDoorCollider" then
		return
	end

	print("OnCollision")
	print(comp1:name())
	print(comp2:name())

	-- this:GetActor():Destroy()
end

function OnDestroy()
	print("Destroying " .. this:name())
end
