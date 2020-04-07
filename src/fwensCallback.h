#include "sdk/public/steam/isteamgameserver.h"
#include "GarrysMod/Lua/Interface.h"

class Fwens
{
private:
	GarrysMod::Lua::ILuaBase* LUA;

	static Fwens* instance;
	Fwens():
		m_steamcallback_HandleCallbackRequest(this, &Fwens::HandleCallbackRequest)
	{};
	
public:
	Fwens(GSClientGroupStatus_t* group);
	static Fwens* GetInstance();
	void SetLuaInstance(GarrysMod::Lua::ILuaBase* ILuaBase);

	STEAM_GAMESERVER_CALLBACK(Fwens, HandleCallbackRequest, GSClientGroupStatus_t, m_steamcallback_HandleCallbackRequest);
};