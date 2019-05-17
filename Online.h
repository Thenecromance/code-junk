#pragma once
namespace Online
{
	void TeleportToPlayer(Player player);
	extern bool waterLoopBool[32];
	void WaterLoop(Player target);

	extern bool spectate[32];
	void specter(Player target);

	extern bool playerfireloopbool[32];
	void fireloop(Player target);

	extern bool savenewdrop2[35];
	void cashdrop2(Player target);

	extern 	bool freezed[35];
	void Freezer(Player target);

	extern 	bool fuckedhandling[32];
	void fuckhandling(Player player);

	void trapcage(Ped ped);

	extern 	bool exploder[32];
	void explodeloop(Player target);

	extern 	int attachobj[100];
	void attachobjects2(char* object, int selectplayer);

#pragma region About Money
	extern bool MoneyLoopBool;
	void Money(bool islogged, bool add, bool bank, int amount = 10000000);

	extern bool Depoist15MBool;
	void Depoist15M(bool bank);

	extern 	bool moneyrain2k;
	void RainMoney(bool toggle);

	extern 	bool moneydropp;
	extern int delay;
	extern bool newmoneyBool;
	void newmoney(int delay);
#pragma endregion

}

namespace BlackOption
{
	void CloneCrash(int selectPlayer);

	void ObjectCrash(int selectPlayer);
}

namespace Remote
{
	extern int selfplayer;
	void NonHostKick(int selectplayer);
	void ChangeSession(int selectplayer);
}