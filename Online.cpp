#include "stdafx.h"

#define PROP_MONEY_BAG_01 0x113FD533
#define PICKUP_MONEY_CASE 0xCE6FDD6B
#define MAIN_PERSISTENT 0x5700179C
#define PICKUP_MONEY_CASE 0xCE6FDD6B
#define PICKUP_MONEY_VARIABLE 0xEA888D49

namespace Online
{
	

#pragma region About Money
	bool MoneyLoopBool = false;
	void Money(bool islogged, bool add, bool bank, int amount)
	{
		int iVar4 = 2147483647;//never Change..
		int addId = 1445302971;
		int transactionid = 312105838;
		if (add)
		{
			addId = 1445302971;
			transactionid = 312105838;
		}
		else
		{
			addId = 537254313;
			transactionid = -1645229221;
		}


		if (UNK3::_NETWORK_SHOP_BEGIN_SERVICE(&iVar4, 1474183246, transactionid, addId, amount, bank ? 4 : 1))
			if (UNK3::_NETWORK_SHOP_CHECKOUT_START(iVar4))
				if (islogged)
					NETWORKCASH::NETWORK_EARN_FROM_ROCKSTAR(amount);
		STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_ornate_heist");
		GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL("scr_ornate_heist");
		GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_heist_ornate_banknotes", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 31086, 0.2f, 0, 0, 0);


		GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL("scr_ornate_heist");
		GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_heist_ornate_banknotes", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 28422, 0.1f, 0, 0, 0);

		GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL("scr_ornate_heist");
		GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_heist_ornate_banknotes", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 60309, 0.1f, 0, 0, 0);

		GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL("scr_agencyheist");
		GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_heist_ornate_banknotes", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 65245, 0.2f, 0, 0, 0);

		GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL("scr_ornate_heist");
		GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_heist_ornate_banknotes", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 35502, 0.2f, 0, 0, 0);
	}

	bool Depoist15MBool = false;
	void Depoist15M(bool bank)
	{
		// 15M has limit 
		//less than 300M
		//globalHandle
		int iVar4 = 2147483647;//never Change..
		int isadd = 1445302971;
		UNK3::_NETWORK_SHOP_BEGIN_SERVICE(&iVar4, 1474183246, -1586170317, isadd, 15000000, bank ? 8 : 4);
		UNK3::_NETWORK_SHOP_CHECKOUT_START(iVar4);
	}

	bool moneyrain2k = false;
	void RainMoney(bool toggle)
	{
		STREAMING::REQUEST_MODEL(0x113FD533);
		Vector3 pp = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 0.0, 20.0);
		int offsetX =	(int)GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(-5, 0);
		int offsetyY =	(int)GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(-5, 0);
		int offsetX1 =	(int)GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(-6, 6);
		int offsetyY1 = (int)GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(-3, 3);
		int offsetZ =	(int)GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(-2, 2);
		int CashHash = -1666779307;  // prop_paper_bag_small = -1803909274
		int PickupHash = GAMEPLAY::GET_HASH_KEY("PICKUP_MONEY_WALLET");
		STREAMING::REQUEST_MODEL(CashHash);
		while (!STREAMING::HAS_MODEL_LOADED(CashHash)) WAIT(0);

		int AMOUNT = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(20000, 40000);
		OBJECT::CREATE_AMBIENT_PICKUP(PickupHash, pp.x + offsetX, pp.y + offsetyY, pp.z, 2500, AMOUNT, CashHash, false, true);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(CashHash);

	}

	bool moneydropp = false;
	int newmoneydelay = 1000;
	bool newmoneyBool = false;
	void newmoney(int delay) {
		int currenttime = timeGetTime();
		Ped iPed = PLAYER::PLAYER_PED_ID();
		{
			if ((timeGetTime() - currenttime > delay)) // Time between drops
			{
				Vector3 coords = ENTITY::GET_ENTITY_COORDS(iPed, FALSE);
				int PickupHash = GAMEPLAY::GET_HASH_KEY("prop_paper_bag_small");
				STREAMING::REQUEST_MODEL(PickupHash);
				while (!STREAMING::HAS_MODEL_LOADED(PickupHash)) WAIT(0);
				OBJECT::CREATE_AMBIENT_PICKUP(PickupHash, coords.x, coords.y, coords.z, 0, 2500, PickupHash, false, true);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(PickupHash);
				currenttime = timeGetTime();
			}
		}
	}
#pragma endregion


	//Function -- //This Is All Player Text Function
	void MessageToAllPlayers(char* message)
	{
		int netHandle[76];
		for (int i = 0; i < 18; i++) {
			NETWORK::NETWORK_HANDLE_FROM_PLAYER(i, &netHandle[0], 13);
			NETWORK::NETWORK_SEND_TEXT_MESSAGE(message, &netHandle[0]);
		}
	}
	//Function -- //This Is Selected Player Text Function
	void SendMessageToPlayer(int player, char* message)
	{
		int netHandle[76];
		NETWORK::NETWORK_HANDLE_FROM_PLAYER(player, &netHandle[0], 13);
		NETWORK::NETWORK_SEND_TEXT_MESSAGE(message, &netHandle[0]);
	}
}

namespace BlackOption
{
	void CloneCrash(int selectPlayer)
	{
		for (int i = 0; i < 999; i++)
		{
			Entity clone = PED::CLONE_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectPlayer), 1, 1, 1);
			if (ENTITY::DOES_ENTITY_EXIST(clone))
			{
				ENTITY::ATTACH_ENTITY_TO_ENTITY(clone, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectPlayer), 31086, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 2, 1);

			}
			WAIT(0);
		}
	}

	void ObjectCrash(int selectPlayer)
	{
		for (auto id : CrashObj)
		{
			Online::attachobjects2(id, selectPlayer);
		}
	}
	//CrashObj
}
static void TRIGGER_SCRIPT_EVENT(int eventGroup, Any* args, uint64_t argCount, uint32_t bit)
{
	static auto func = reinterpret_cast<void(*)(int, Any*, uint64_t, uint32_t)>(Memory::pattern("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 81 EC ? ? ? ? 45 8B F0 41 8B F9").count(1).get(0).get<void>(0));
	int bitset = 0;
	GAMEPLAY::SET_BIT(&bitset, bit);
	if (bitset != 0)
		func(eventGroup, args, argCount, bitset);
}

#pragma region This is remote Part, but when I write this, this function hasn't be added to this menu

//is not difficult to find 
std::vector<int> spamMsg =
{
-130461309,
-302923242,
-1291682829,
220726201,
403111300,
1360610666,
1954854628,
-304814395,
1913017082,
-1445591614,
-1550586884,
-1432462772,
2110027654,
554851531,
1395926542,
1446758869,
-1847667036,
1095428883,
-53425260,
1513549393,
-2866549,
-338055497,
-1794758144,
631165512,
-324694967,
-43727610,
1641976889,
-1620534525,
1082546806,
-1232283930,
531679075,
1055860763,
-2115279011,
-1205772980,
962028193,
-916417229,
-996936862,
1946605908,
-2042974830,
-1640328198,
-1428950535,
1877019158,
-452372621,
747511033,
1615078849,
-458328083,
18508630,
885668437,
-900126131,
702987651,
436170208,
-2119652085,
2012013119,
1802792608,
394686232,
1264507432,
-1771971111,
-210487359,
-48152463,
-1369961623,
1583919327,
1723217930,
-101215037,
-1476055326,
1347970161,
1615997358,
1748623315,
-1402572071,
-1926407637,
888337179,
-2029924498,
1128583433,
2128648125,
931718501,
-359302762,
396931208,
-1622818674,
};
int fakeammount = 0;
int pos = 1;
void RemoteFunctions()
{
	Title("Remote Functions");
	//This part sometimes won't work as well
	if (menu.Option("VIP Notification"))//作战中心位置改变
	{
		//	DWORD64 args[4] = { -319074860,selectplayer ,1226312168 ,0 };//Global_1377446.f_2
		DWORD64 args[4] = { 566035618,selectplayer ,174677746 ,0 };

		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 4, 1 << selectplayer);
	}
	//This part is won't work anymore(Got Patched since b1604
	//if (menu.Option("Change of operations center"))//作战中心位置改变
	//{
	//	//DWORD64 args[4] = { -319074860,selectplayer ,-1650282162 ,0 };
	//	DWORD64 args[4] = { 566035618,selectplayer ,-821275389 ,0 };
	//	PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 4, 1 << selectplayer);
	//}
	//if (menu.Option("transfer Msg"))//转移恐霸
	//{
	//	//DWORD64 args[4] = { -319074860,selectplayer ,762099350 ,0 };
	//	DWORD64 args[4] = { 566035618,selectplayer ,-1099588997 ,0 };
	//	PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 4, 1 << selectplayer);
	//}
	if (menu.Option("Active Broadcast"))
	{
		//DWORD64 args[4] = { -319074860,selectplayer ,1471084428 ,0 };
		DWORD64 args[4] = { 566035618,selectplayer ,1383721237 ,0 };
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 4, 1 << selectplayer);
	}
	if (menu.Option("DeActive Broadcast"))
	{
		DWORD64 args[4] = { 566035618,selectplayer ,1254298341 ,0 };
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 4, 1 << selectplayer);
	}
	if (menu.Option("Force him Look foward"))
	{
		DWORD64 args[4] = { 566035618 ,selectplayer,-1432407380 ,0 };
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 4, 1 << selectplayer);

	}
	if (menu.Option("YACHT invite"))//游艇
	{
		Remote::fake_join_msg(selectplayer, Remote::PIM_INVAYACHT);//Global_1377446.f_33
	}
	if (menu.Option("Office Invite"))//办公室
	{
		Remote::fake_join_msg(selectplayer, Remote::PIM_INVAOFF);
	}
	if (menu.Option("night club invite"))//会所
	{
		Remote::fake_join_msg(selectplayer, Remote::PIM_INVACLUB);
	}
	if (menu.Option("MOD shop Invite"))//载具改装铺
	{
		Remote::fake_join_msg(selectplayer, Remote::PIM_INVOSHOP);
	}
	if (menu.Option("Garage Invite"))//办公室车库
	{
		Remote::fake_join_msg(selectplayer, Remote::PIM_INVAGAR);
	}
	if (menu.Option("CEO Kick"))//解聘
	{
		DWORD64 args[4] = { -1190833098,selectplayer,1,5 };//Global_1377446.f_523
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 4, 1 << selectplayer);
	}
	if (menu.Option("CEO Ban"))//清理门户
	{
		DWORD64 args[4] = { -1190833098,selectplayer,1,6 };
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 4, 1 << selectplayer);
	}
	if (menu.Option("CEO Kick V2"))
	{
		DWORD64 ceokick[4] = { -4156321, selectplayer, 0, 0 };
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, ceokick, 4, 1 << selectplayer);
	}
	if (menu.Option("CEO Ban V2"))
	{
		DWORD64 ceokick[4] = { 360381720, selectplayer, 0, 0 };//Global_1377446.f_545
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, ceokick, 4, 1 << selectplayer);
	}
	menu.IntOption("Session Weather", weather_id, 0, 15);
	if (menu.Option("Change Weather"))
	{
		PATTERN_NATIVE::SET_SESSION_WEATHER(1, weather_id, 76, 0);
	}
	if (menu.Option("None Host Kick"))//This is amazing
	{
		DWORD64 args[3] = { 171220178,selectplayer,-1 };//Global_1377446.f_287
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 3, 1 << selectplayer);
	}//-1076659005
	if (menu.Option("Kick to Single player"))
	{
		DWORD64 sp_crash[4] = { -1638105803, selectplayer, 0, 0 };//Global_1377446.f_594
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, sp_crash, 4, 1 << selectplayer);
	}
	if (menu.Option("Kick"))
	{
		DWORD64 kick[4] = { 769347061, selectplayer, 0, 0 };//Global_1377446.f_595
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, kick, 9, 1 << selectplayer);
	}
	menu.IntOption("Pos", pos, 1, 115);
	if (menu.Option("Teleport Him"))
	{
		DWORD64 teleport[9] = { 1000837481, selectplayer, 0, -1, 1, pos, 0, 0, 0 };//Global_1377446.f_486
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, teleport, 9, 1 << selectplayer);
	}
	if (menu.Option("Infinite Load"))
	{
		DWORD64 infiniteload[9] = { 1000837481, selectplayer, 0, -1, 1, 115, 0, 0, 0 };
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, infiniteload, 9, 1 << selectplayer);
	}
	if (menu.Option("Send to Mission"))
	{
		DWORD64 send_to_mission[9] = { -1571039706, selectplayer, 0, 0, 0, -1, 1, 1, 0 };
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, send_to_mission, 9, 1 << selectplayer);
	}
	if (menu.Option("Remote Money"))
	{
		//DWORD64 args[10] = { -1306381802,selectplayer,0,0,0,0,0,0,0,233 };//Freemode.c line 299705 10args
		DWORD64 args[10] = { -181444979,selectplayer,0,0,0,0,0,0,0,233 };
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 10, 1 << selectplayer);
	}
	menu.IntOption("Fake Msg Param", fakeammount, 0, INT_MAX);
	if (menu.Option("Fake Money Save Message"))
	{
		DWORD64 args[14] = { 713068249,selectplayer,/*事件ID*/-1550586884, fakeammount,0,0,0,0,0,0,selectplayer,0,0,0 };
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 14, 1 << selectplayer);
	}
	if (menu.Option("Fake Money Stolen Message"))
	{
		DWORD64 args[14] = { 713068249,selectplayer,/*事件ID*/-1432462772, fakeammount,0,0,0,0,0,0,selectplayer,0,0,0 };
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 14, 1 << selectplayer);
	}
	if (menu.Option("Fake Money Removed Message"))
	{
		DWORD64 args[14] = { 713068249,selectplayer,/*事件ID*/2110027654, fakeammount,0,0,0,0,0,0,selectplayer,0,0,0 };
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 14, 1 << selectplayer);
	}
	if (menu.Option("Fake Msg"))
	{
		DWORD64 args[14] = { 713068249,selectplayer,/*事件ID*/-1476055326, fakeammount,0,0,0,0,0,0,selectplayer,/*指定杀死玩家*/0,0,0 };
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 14, 1 << selectplayer);
	}
	if (menu.Option("A draw Msg"))
	{
		DWORD64 args[14] = { 713068249,selectplayer,/*事件ID*/16107197, 1,0,0,0,0,0,0,selectplayer,/*指定杀死玩家*/0,0,0 };
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 14, 1 << selectplayer);
	}
	menu.IntOption("Msg type", SpamPos, 0, spamMsg.size());
	if (menu.Option("Send Msg"))//Done
	{
		DWORD64 args[14] = { 713068249,selectplayer,/*事件ID*/spamMsg[SpamPos], 1,0,0,0,0,0,0,selectplayer,/*指定杀死玩家*/0,0,0 };
		PATTERN_NATIVE::TRIGGER_SCRIPT_REMOTE_EVENT(1, args, 14, 1 << selectplayer);
	}

}
#pragma endregion