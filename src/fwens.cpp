#include "fwens.h"
#include <stdlib.h>

Fwens* Fwens::instance = NULL;
Fwens::~Fwens() {
	if (GetSteamContextActive())
	{
		ClearSteamContext();
	}
}

Fwens::Fwens():
	m_steamcallback_HandleConnected(this, &Fwens::Steam_HandleSteamConnected),
	m_steamcallback_HandleGroupRequest(this, &Fwens::Steam_HandleGroupRequest),
	m_steamcallback_HandleDisconnected(this, &Fwens::Steam_HandleOnDisconnect),
	m_steamcallback_HandlePolicyResponse(this, &Fwens::Steam_HandleOnPolicyResponse)
{}

Fwens* Fwens::GetInstance() {
	if (instance == NULL) {
		instance = new Fwens;
	}

	return instance;
}

void Fwens::SetLuaInstance(GarrysMod::Lua::ILuaBase* ILuaBase)
{
	LUA = ILuaBase;
}

void Fwens::InitSteamAPIConnection()
{
	bool status = steamContext.Init();
	if (!status)
	{
		return;
	}
	steamContext_active = true;
}

bool Fwens::GetSteamContextActive()
{
	return steamContext_active;
}

void Fwens::ClearSteamContext() {
	steamContext.Clear();
	steamContext_active = false;
}

void Fwens::Steam_HandleOnPolicyResponse(GSPolicyResponse_t* policyResponse)
{
	bool status = GetSteamContextActive();
	if (!status)
	{
		InitSteamAPIConnection();
		return;
	}
	steamContext_active = true;
}

void Fwens::Steam_HandleSteamConnected(SteamServersConnected_t* connnected)
{
	bool status = GetSteamContextActive();
	if (!status)
	{
		InitSteamAPIConnection();
		return;
	}
	steamContext_active = true;
}

void Fwens::Steam_HandleOnDisconnect(SteamServersDisconnected_t* something)
{
	steamContext_active = false;
	bool status = GetSteamContextActive();
	if (!status)
	{
		InitSteamAPIConnection();
		return;
	}
	steamContext_active = true;
}

void Fwens::RequestUserGroupStatus(CSteamID player, CSteamID groupID)
{
	ISteamGameServer* steamGameServer = steamContext.SteamGameServer();
	steamGameServer->RequestUserGroupStatus(player, groupID);
}


void Fwens::Steam_HandleGroupRequest(GSClientGroupStatus_t* pCallback)
{
	// CSteamID.Render() no longer appears to function, we'll cast these manually.
	char userBuffer[18];
	char groupBuffer[19];

	snprintf(userBuffer, sizeof(userBuffer), "%llu", pCallback->m_SteamIDUser.ConvertToUint64());
	snprintf(groupBuffer, sizeof(groupBuffer), "%llu", pCallback->m_SteamIDGroup.ConvertToUint64());
	
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
	LUA->GetField(-1, "hook");
	LUA->GetField(-1, "Run");
		LUA->PushString("GroupDataReturned");

		LUA->CreateTable();
			LUA->PushBool(pCallback->m_bMember);
			LUA->SetField(-2, "isMember");

			LUA->PushBool(pCallback->m_bOfficer);
			LUA->SetField(-2, "isOfficer");

			LUA->PushString(userBuffer);
			LUA->SetField(-2, "steamID64");

			LUA->PushString(groupBuffer);
			LUA->SetField(-2, "groupID64");

	int returnValue = LUA->PCall(2, 0, 0);
	if (returnValue != 0) {
		// Dump our two current tables to simplify this. Can't pop because LIFO.
		LUA->Remove(1);
		LUA->Remove(1);
	
		LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "ErrorNoHalt");
			LUA->Push(-3);
			LUA->PushString("\n");
		LUA->Call(2, 0);
		LUA->Pop(2);
		
		return;
	}

	LUA->Pop(2);
}