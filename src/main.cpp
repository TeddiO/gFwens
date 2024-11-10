#include "GarrysMod/Lua/Interface.h"
#include "fwens.h"
#include <stdlib.h>
#include <stdio.h>

using namespace GarrysMod::Lua;

LUA_FUNCTION(GetInSteamGroup)
{
	if (!LUA->IsType(1, Type::String))
	{
		LUA->ArgError(1, "expected string");
		return 0;
	}

	if (!LUA->IsType(2, Type::String))
	{
		LUA->ArgError(2, "expected string");
		return 0;
	}

	const char* strSteamuser = LUA->GetString(1);
	const char* strSteamGroup = LUA->GetString(2);

	uint64 steamID64 = strtoull(strSteamuser, NULL, 10);
	uint64 groupID64 = strtoull(strSteamGroup, NULL, 10);

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

	Fwens* fwenVar = Fwens::GetInstance();
	fwenVar->RequestUserGroupStatus(player, groupID);
	return 0;
}

#ifndef GFWENS_VERSION
const char* GFWENS_VERSION = "LOCAL";
const char* GFWENS_BUILD_DATE = __DATE__;
#endif

GMOD_MODULE_OPEN()
{
	const int buffSize = 60;
	char versionBuffer[buffSize];
	snprintf(versionBuffer, buffSize, "gfwens %s (%s) loaded.", GFWENS_VERSION, GFWENS_BUILD_DATE);

	LUA->PushSpecial(SPECIAL_GLOB);
	LUA->CreateTable();

		LUA->PushCFunction(GetInSteamGroup);
		LUA->SetField(-2, "GetInSteamGroup");

		LUA->PushString(GFWENS_VERSION);
		LUA->SetField(-2, "version");

		LUA->SetField(-2, "fwens");

		LUA->GetField(-1, "print");
		LUA->PushString(versionBuffer);
		LUA->Call(1, 0);
	LUA->Pop();

	Fwens* fwenVar = Fwens::GetInstance();
	fwenVar->SetLuaInstance(LUA);

	// Exists for changelevel purposes. Will silently fail during first boot of the server.
	if (!fwenVar->GetSteamContextActive())
	{
		fwenVar->InitSteamAPIConnection();
	}

	return 0;
}

GMOD_MODULE_CLOSE()
{
	Fwens* fwenVar = Fwens::GetInstance();
	delete fwenVar;
	return 0;
}

