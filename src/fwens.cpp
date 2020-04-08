#include "Fwens.h"
#include <stdlib.h>

Fwens* Fwens::instance = NULL;
Fwens::~Fwens() {
	if (this->GetSteamContextActive())
	{
		this->ClearSteamContext();
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
	bool status = this->GetSteamContextActive();
	if (!status)
	{
		this->InitSteamAPIConnection();
		return;
	}
	steamContext_active = true;
}

void Fwens::Steam_HandleSteamConnected(SteamServersConnected_t* connnected)
{
	bool status = this->GetSteamContextActive();
	if (!status)
	{
		this->InitSteamAPIConnection();
		return;
	}
	steamContext_active = true;
}

void Fwens::Steam_HandleOnDisconnect(SteamServersDisconnected_t* something)
{
	steamContext_active = false;
	bool status = this->GetSteamContextActive();
	if (!status)
	{
		this->InitSteamAPIConnection();
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
	// CSteamID.Render() no longer appears to function, we'll cast these manually in a bit.
	char userBuffer[64];
	char groupBuffer[64];

	
	this->LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
	this->LUA->GetField(-1, "hook");
	this->LUA->GetField(-1, "Run");
	this->LUA->PushString("GroupDataReturned");

	this->LUA->CreateTable();
	this->LUA->PushBool(pCallback->m_bMember);
	this->LUA->SetField(-2, "isMember");

	this->LUA->PushBool(pCallback->m_bOfficer);
	this->LUA->SetField(-2, "isOfficer");

	this->LUA->PushString(_i64toa(pCallback->m_SteamIDUser.ConvertToUint64(), userBuffer, 10));
	this->LUA->SetField(-2, "steamID64");

	this->LUA->PushString(_i64toa(pCallback->m_SteamIDGroup.ConvertToUint64(), groupBuffer, 10));
	this->LUA->SetField(-2, "groupID64");

	//Fix error handling here! Otherwise forever breaking badly
	int returnValue = this->LUA->PCall(2, 0, 0);

	if (returnValue != 0) {
		this->LUA->Remove(-2);
		return;
	}

	this->LUA->Pop(2);
}