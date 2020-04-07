#include "fwensCallback.h"
#include <stdlib.h>

Fwens* Fwens::instance = NULL;
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

Fwens::Fwens(GSClientGroupStatus_t* group) :
	m_steamcallback_HandleCallbackRequest(this, &Fwens::HandleCallbackRequest)
{}

void Fwens::HandleCallbackRequest(GSClientGroupStatus_t* pCallback)
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