if SERVER then return end

local voiceModActive = false

concommand.Add("toggle_voicemod", function()
    voiceModActive = not voiceModActive
    net.Start("VoiceMod_Activate")
    net.WriteBool(voiceModActive)
    net.SendToServer()
end)

hook.Add("PlayerStartVoice", "VoiceMod_Start", function(ply)
    if voiceModActive and ply == LocalPlayer() then
        hook.Add("Tick", "VoiceMod_ProcessAudio", function()
            -- Ajouter ici le traitement audio en C++
        end)
    end
end)

hook.Add("PlayerEndVoice", "VoiceMod_End", function(ply)
    if ply == LocalPlayer() then
        hook.Remove("Tick", "VoiceMod_ProcessAudio")
    end
end)

net.Receive("VoiceMod_Activate", function()
    local isEnabled = net.ReadBool()
    if isEnabled then
        chat.AddText(Color(0, 255, 0), "Voice modifier activated.")
    else
        chat.AddText(Color(255, 0, 0), "Voice modifier deactivated.")
    end
end)
