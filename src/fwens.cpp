#include "GarrysMod/Lua/Interface.h"
#include "sdk/public/steam/isteamgameserver.h"
#include "fwensCallback.h"
#include <stdlib.h>
#include <stdio.h>

using namespace GarrysMod::Lua;

LUA_FUNCTION(GetInSteamGroup)
{
	if (!LUA->IsType(1, Type::STRING))
	{
		LUA->ArgError(1, "expected string");
		return 0;
	}

	if (!LUA->IsType(2, Type::STRING))
	{
		LUA->ArgError(2, "expected string");
		return 0;
	}

	const char* strSteamuser = LUA->GetString(1);
	const char* strSteamGroup = LUA->GetString(2);

	uint64 steamID64 = _atoi64(strSteamuser);
	uint64 groupID64 = _atoi64(strSteamGroup);

	CSteamID player = CSteamID(steamID64);
	if (!player.IsValid())
	{
		LUA->ThrowError("Invalid Player SteamID64 passed. Are you sure it's correct?");
		return 0;
	}

	CSteamID groupID = CSteamID(groupID64);
	if (!groupID.IsValid())
	{
		LUA->ThrowError("Invalid Group SteamID64 passed. Are you sure it's correct?");
		return 0;
	}
	
	// If Steam isn't available, this will return false. 
	bool steamAvailable = SteamGameServer()->RequestUserGroupStatus(player, groupID);
	LUA->PushBool(steamAvailable);
	
	return 1;
}


GMOD_MODULE_OPEN()
{
	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->CreateTable();

		LUA->PushCFunction(GetInSteamGroup);
		LUA->SetField(-2, "GetInSteamGroup");

		LUA->SetField(-2, "fwens");

		LUA->GetField(-1, "print");
		LUA->PushString("gfwens v1 loaded.");
		LUA->Call(1, 0);
	LUA->Pop();

	Fwens* fwenVar = Fwens::GetInstance();
	fwenVar->SetLuaInstance(LUA);

	return 0;
}

GMOD_MODULE_CLOSE()
{
	Fwens* fwenVar = Fwens::GetInstance();
	delete fwenVar;

	return 0;
}

