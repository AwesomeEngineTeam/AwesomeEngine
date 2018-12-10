local sound_emitter

local lastState = "Music"
local radioIsOn = true

function Start()
	sound_emitter = this:GetActor():GetSoundEmitter()

	Sound.SetState("RadioChannel", lastState)
	sound_emitter:PostEvent("TurnOnRadio")
end

function OnDestroy()
	sound_emitter:ExecuteActionOnEvent("TurnOnRadio", Sound.ActionType.Stop, 0, Sound.InterpMode.Linear)
end

function Interact()
	if not radioIsOn then
		return
	end

	if lastState == "Jingle" then
		Sound.SetState("RadioChannel", "Music")
		lastState = "Music"
	elseif lastState == "Music" then
		Sound.SetState("RadioChannel", "Jingle")
		lastState = "Jingle"
	end
end

function InteractAux()
	if radioIsOn then
		Sound.SetState("RadioChannel", "Off")
		radioIsOn = false
	else
		Sound.SetState("RadioChannel", lastState)
		radioIsOn = true
	end
end
