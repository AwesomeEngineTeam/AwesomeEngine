import("Input")

function Start()
	local sound_emitter = this:GetActor():GetSoundEmitter()

	if sound_emitter ~= nil then
		Sound.MuteBackgroundMusic(true)
		sound_emitter:PostEvent("Music")
	end
end

function Update()
	if Input.GetKeyPressed(KeyCode.Numpad2) then
		Sound.SetState("Environment", "Ground")
	elseif Input.GetKeyPressed(KeyCode.Numpad3) then
		Sound.SetState("Environment", "Underwater")
	elseif Input.GetKeyPressed(KeyCode.Numpad4) then
		Sound.MuteBackgroundMusic(not Sound.GetBackgroundMusicMute())
	end
end
