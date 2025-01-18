util.AddNetworkString("VoiceMod_Activate")

local voiceModStates = {}

net.Receive("VoiceMod_Activate", function(len, ply)
    local isEnabled = net.ReadBool()
    voiceModStates[ply] = isEnabled

    net.Start("VoiceMod_Activate")
    net.WriteBool(isEnabled)
    net.Broadcast()

    if isEnabled then
        print(ply:Nick() .. " has activated the voice modifier.")
    else
        print(ply:Nick() .. " has deactivated the voice modifier.")
    end
end)

hook.Add("PlayerDisconnected", "VoiceMod_Cleanup", function(ply)
    voiceModStates[ply] = nil
end)
