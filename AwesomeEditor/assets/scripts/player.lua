import("Input")
import("Physics")

-- Bindings :	- reach
--				- force
-- 				- mouseSpeed
--				- maxSpeed
--				- sprintSpeed
--				- velocityDrag
--				- cameraRotationSpeed
--				- camera

-- Declare and set this to anything but nil to disable cast related exceptions
DisableCastExceptions = 1

local actor = this:GetActor()
local forward

local clickPos = Vec2(0, 0)

local currentRotation = Vec2(0, 0)
local newRotation = Vec2(0, 0)

local captureRectSize = Vec2(10, 10)

local grabbed_obj = nil

local ToRigidbody
local Raycast

local UpdateGrab
local UpdateRotation
local UpdateTranslation

function Update(frameTime)
	-- if Input.GetKeyPressed(KeyCode.Alt) then
	-- if Input.IsMouseButtonDown(MouseButton["Middle"]) then
	-- 	Input.SetMouseCaptured(not Input.IsMouseCaptured())
	-- end

	UpdateRotation(frameTime)
	UpdateTranslation(frameTime)

	UpdateGrab()

	if Input.GetKeyPressed(KeyCode.F) then
		local hit = Raycast(reach)
		if hit:comp() then
			hit:comp():GetActor():GetScript():Invoke("Interact")
		end
	end

	if Input.GetKeyPressed(KeyCode.T) then
		local hit = Raycast(reach)
		if hit:comp() then
			hit:comp():GetActor():GetScript():Invoke("InteractAux")
		end
	end
end

UpdateGrab = function()
	if Input.GetKeyPressed(KeyCode.E) then
		if grabbed_obj then
			grabbed_obj:SetKinematic(false)
			grabbed_obj = nil
		else
			local hit = Raycast(reach)
			if hit:comp() then
				local rb = ToRigidbody(hit:comp())
				if rb and not rb:IsKinematic() then
					grabbed_obj = rb
					grabbed_obj:SetKinematic(true)
				end
			end
		end
	end

	if grabbed_obj then
		grabbed_obj:GetActor():GetRootComponent():SetGlobalPosition(camera:GlobalPosition() + (forward * 7.5))
	end

	if Input.GetMouseReleased(MouseButton.Left) then
		if grabbed_obj then
			grabbed_obj:SetKinematic(false)
			grabbed_obj:AddForce(forward * force, ForceMode.Impulse)
			grabbed_obj = nil
		end
	end
end

local function IsSprinting()
	if Input.GetKeyDown(KeyCode.Space) then
		return sprintSpeed
	else
		return 1
	end
end

UpdateRotation = function(frameTime)
	Input.HideMouse()

	local winDim = Input.GetFocusedWindowDimensions()
	local captureRectPos = Vec2((winDim:x() - captureRectSize:x()) / 2, (winDim:y() - captureRectSize:y()) / 2)

	local mousePos32 = Input.GetMousePosition()

	if Input.GetMousePressed(MouseButton.Right) then
		-- clickPos = mousePos32
		clickPos:x(mousePos32:x());
		clickPos:y(mousePos32:y());
	end

	if clickPos:x() < 0 or clickPos:x() > winDim:x() or
		clickPos:y() < 0 or clickPos:y() > winDim:y() 	then
		return
	end

	if Input.GetMouseReleased(MouseButton.Right) then
		currentRotation = newRotation + currentRotation
	end

	if Input.GetMouseDown(MouseButton.Right) then
		local newPos = Vec2(mousePos32:x(), mousePos32:y())

		if mousePos32:x() < captureRectPos:x() - 1 then
			newPos:x((captureRectPos:x() + captureRectSize:x()))
		elseif mousePos32:x() > (captureRectPos:x() + captureRectSize:x()) then
			newPos:x(captureRectPos:x())
		end

		if mousePos32:y() < captureRectPos:y() - 1 then
			newPos:y((captureRectPos:y() + captureRectSize:y()))
		elseif mousePos32:y() > (captureRectPos:y() + captureRectSize:y()) then
			newPos:y(captureRectPos:y())
		end

		if mousePos32:x() ~= newPos:x() or mousePos32:y() ~= newPos:y() then
			currentRotation = currentRotation + Vec2(
				(mousePos32:x() - clickPos:x()) / (captureRectSize:x()),
				(mousePos32:y() - clickPos:y()) / (captureRectSize:y())
			)

			clickPos:x(newPos:x())
			clickPos:y(newPos:y())

			mousePos32:x(newPos:x());
			mousePos32:y(newPos:y());

			Input.SetMousePositionV(newPos)
		end

		newRotation = Vec2(
			(mousePos32:x() - clickPos:x()) / (captureRectSize:x()),
			(mousePos32:y() - clickPos:y()) / (captureRectSize:y())
		)

		camera:SetGlobalRotation(
			Quaternion.FromAxis(-(newRotation:x() + currentRotation:x()) * cameraRotationSpeed * (winDim:x() / winDim:y()) / 100, Vec3(0, 1, 0)) *
			Quaternion.FromAxis(-(newRotation:y() + currentRotation:y()) * cameraRotationSpeed * (winDim:x() / winDim:y()) / 100, Vec3(1, 0, 0))
		)
	end
end

UpdateTranslation = function(frameTime)
	local velocity = Vec3(0, 0, 0)

	forward = camera:GlobalRotation() * -Vector3.Forward() -- - Forward because camera has inverted Z
	forward:Normalize()

	local rb = ToRigidbody(actor:GetRootComponent())

	if Input.GetKeyDown(KeyCode.W) then
		velocity = velocity - Vector3.Forward();
	end
	if Input.GetKeyDown(KeyCode.S) then
		velocity = velocity + Vector3.Forward();
	end
	if Input.GetKeyDown(KeyCode.D) then
		velocity = velocity + Vector3.Right();
	end
	if Input.GetKeyDown(KeyCode.A) then
		velocity = velocity - Vector3.Right();
	end

	if velocity ~= Vector3.Zero() and (rb:linearVelocity() + velocity):SquaredNorm() < maxSpeed * maxSpeed * IsSprinting() then
		velocity = camera:GlobalRotation() * velocity;
		velocity:y(0)
		velocity:Normalize()
		rb:AddForce(velocity, ForceMode.Impulse)
	else
		velocity = rb:linearVelocity()
		velocity = velocity - velocity * velocityDrag * frameTime
		velocity:y(rb:linearVelocity():y())
		rb:linearVelocity(velocity)
	end
end

ToRigidbody = function(component)
	return Cast(component, "Rigidbody")
end

Raycast = function(reach)
	return Physics.Raycast(camera:GlobalPosition() + 2 * forward, forward, reach)
end
