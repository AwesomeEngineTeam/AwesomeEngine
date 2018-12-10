import("Physics")

-- Bindings :	- wreckingBall
--				- portal
--				- impulseForce

local notesBuffer = {}

local solution = {
	"DS5JingleNote",
	"G5JingleNote",
	"C6JingleNote",
	"AS5JingleNote",
}

function RegisterNote(note)
	if #notesBuffer == 4 then
		table.remove(notesBuffer, 1)
	end

	table.insert(notesBuffer, note)

	if #notesBuffer == 4 then
		for noteCount = 1, #notesBuffer do
			if solution[noteCount] ~= notesBuffer[noteCount] then
				return
			end
		end

		portal:SetActive(true)
		this:GetActor():GetSoundEmitter():PostEvent("RiddleSolved")

		-- CubeHeap
		local cubes = Scene.FindActorsWithName("CubeHeap");

		for cubeCount = 1, #cubes do
			local body = Cast(cubes[cubeCount]:FindComponentWithName("CubeHeap"), "Rigidbody")
			body:SetKinematic(false)
		end

		-- WreckingBall
		wreckingBall:GetActor():SetActive(true)

		local body = Cast(wreckingBall, "Rigidbody")

		body:SetKinematic(false)
		body:AddForce(impulseForce * Vector3.Forward(), ForceMode.Impulse)

		this:SetActive(false)
	end
end
