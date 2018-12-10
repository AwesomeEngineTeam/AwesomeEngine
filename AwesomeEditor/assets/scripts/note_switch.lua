-- Bindings :	- noteToPlay

local sound_emitter = this:GetActor():GetParent():GetSoundEmitter()
local switch_root_script = this:GetActor():GetParent():GetScript()

function Interact()
	sound_emitter:PostEvent(noteToPlay)
	if switch_root_script:IsActive() then
		switch_root_script:Invoke("RegisterNote", noteToPlay)
	end
end
