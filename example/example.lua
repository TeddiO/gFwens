require("fwens")

local groupID = "103582791434672565" -- Steam Universe group (https://steamcommunity.com/groups/steamuniverse)
-- When the player joins send a request to the module with the players SteamID64 and the 64 bit group id
-- in a string format. We get back a response whether or not Steam is alive at the time. If not, you may want
-- to requeue and try again in a bit.
hook.Add("PlayerInitialSpawn", "GrabGroupStatus", function(ply)
	local success = fwens.GetInSteamGroup(ply:SteamID64(), groupID)
end)

-- If we're successful then we'll receive a table with all the data in it. Following values returned are:
-- returnedData.steamID64
-- returnedData.groupID64
-- returnedData.isMember
-- returnedData.isOfficer
-- Do your calculations here or elsewhere in terms of awarding group bonuses, denying people from joining, whatever
-- floats your boat! Keep in mind that this hook is not called instantly and performance depends on how quickly
-- Steam can get back with a response.
hook.Add( "GroupDataReturned", "GetGroupData", function(returnedData)
	local ply = player.GetBySteamID64(returnedData.steamID64)
	if !ply || !IsValid(ply) then return end
	
	if !ply.groupStatus then
		ply.groupStatus = {}
	end

	-- Recommended way if you plan on checking multipel groups.
	ply.groupStatus[returnedData.groupID64] = 
	{
		isMember = returnedData.isMember,
		isOfficer = returnedData.isOfficer
	}
end)

