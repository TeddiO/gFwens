#include "GarrysMod/Lua/Interface.h"
#include "sdk/public/steam/steam_gameserver.h"

class Fwens
{
private:
	GarrysMod::Lua::ILuaBase* LUA = NULL;
	static Fwens* instance;
	Fwens();
	CSteamGameServerAPIContext steamContext;
	bool steamContext_active = false;
	
public:
	~Fwens();
	static Fwens* GetInstance();
	void InitSteamAPIConnection();
	bool GetSteamContextActive();
	void SetLuaInstance(GarrysMod::Lua::ILuaBase* ILuaBase);
	void RequestUserGroupStatus(CSteamID player, CSteamID groupID);
	void ClearSteamContext();	
	
	STEAM_GAMESERVER_CALLBACK(Fwens, Steam_HandleSteamConnected, SteamServersConnected_t, m_steamcallback_HandleConnected);
	STEAM_GAMESERVER_CALLBACK(Fwens, Steam_HandleGroupRequest, GSClientGroupStatus_t, m_steamcallback_HandleGroupRequest);
	STEAM_GAMESERVER_CALLBACK(Fwens, Steam_HandleOnDisconnect, SteamServersDisconnected_t, m_steamcallback_HandleDisconnected);
	STEAM_GAMESERVER_CALLBACK(Fwens, Steam_HandleOnPolicyResponse, GSPolicyResponse_t, m_steamcallback_HandlePolicyResponse);
};
