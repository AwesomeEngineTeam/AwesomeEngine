import("Input")
import("Maths")
import("Time")

-- Bindings: - cameraRotationSpeed
-- 			 - cameraTranslationSpeed
-- 			 - mouseSpeed

local camera = this:GetActor()

local clickPos = Vec2(0, 0)

local currentRotation = Vec2(0, 0)
local newRotation = Vec2(0, 0)

local captureRectSize = Vec2(10, 10)

local UpdateRotation
local UpdateTranslation

function Update(frameTime)
	-- if Input.GetKeyPressed(KeyCode.Alt) then
	-- if Input.IsMouseButtonDown(MouseButton["Middle"]) then
	-- 	Input.SetMouseCaptured(not Input.IsMouseCaptured())
	-- end
	UpdateRotation(frameTime)
	UpdateTranslation(frameTime)
end

local function IsSprinting()
	if Input.GetKeyDown(KeyCode.Space) then
		return 2.25
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

		camera:GetRootComponent():SetGlobalRotation(
			Quaternion.FromAxis(-(newRotation:x() + currentRotation:x()) * cameraRotationSpeed * (winDim:x() / winDim:y()) / 100, Vec3(0, 1, 0)) *
			Quaternion.FromAxis(-(newRotation:y() + currentRotation:y()) * cameraRotationSpeed * (winDim:x() / winDim:y()) / 100, Vec3(1, 0, 0))
		)
	end
end

UpdateTranslation = function(frameTime)
	sprint = IsSprinting()

	speed = cameraTranslationSpeed * frameTime * sprint

	if Input.GetKeyDown(KeyCode.W) then
		camera:Translate_SelfSpace(speed * Vector3.Forward())
	end
	if Input.GetKeyDown(KeyCode.S) then
		camera:Translate_SelfSpace(-speed * Vector3.Forward())
	end
	if Input.GetKeyDown(KeyCode.D) then
		camera:Translate_SelfSpace(speed * Vector3.Right())
	end
	if Input.GetKeyDown(KeyCode.A) then
		camera:Translate_SelfSpace(-speed * Vector3.Right())
	end
	if Input.GetKeyDown(KeyCode.Q) then
		camera:Translate_SelfSpace(-speed * Vector3.Up())
	end
	if Input.GetKeyDown(KeyCode.E) then
		camera:Translate_SelfSpace(speed * Vector3.Up())
	end
end
