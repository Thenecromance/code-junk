#include "stdafx.h"
#include "JsonMod.h"

using json = nlohmann::json;
int EntitiesNum = 0, EntCount = 0;
std::string AutherName;


std::map<int, Object> AttachPool;

EntType JsonGetType(std::string type)
{
	if ("vehicle" == type)
		return vehicle;
	if ("ped" == type)
		return ped;
	if ("object" == type)
		return object;

	return no_entity;
}

#pragma region Vehilce Option
void LoadJsonVehilceData(const json &json, std::string entitiesnum, VehilceData * Data)
{
	Data->hash = json[entitiesnum]["Entity Hash"].get<Hash>(); // Get Entities Hash
	Data->initHandle = json[entitiesnum]["Initial Handle"].get<Hash>();//To Get Init Handle
	Data->HasParents = json[entitiesnum]["Attachments"]["Has Parent"].get<bool>();
	if (Data->HasParents)
	{
		Data->AttachTo.Bone = json[entitiesnum]["Attachments"]["Bone"].get<std::string>();
		Data->AttachTo.ParentsId = json[entitiesnum]["Attachments"]["Parent ID"].get<int>();

		Data->AttachTo.Position.x = json[entitiesnum]["Attachments"]["Position"][0].get<float>();
		Data->AttachTo.Position.y = json[entitiesnum]["Attachments"]["Position"][1].get<float>();
		Data->AttachTo.Position.z = json[entitiesnum]["Attachments"]["Position"][2].get<float>();

		Data->AttachTo.Rot.Pitch = json[entitiesnum]["Attachments"]["Rotation"][0].get<float>();
		Data->AttachTo.Rot.Roll = json[entitiesnum]["Attachments"]["Rotation"][1].get<float>();
		Data->AttachTo.Rot.Yaw = json[entitiesnum]["Attachments"]["Rotation"][2].get<float>();
	}
	Data->name = json[entitiesnum]["Display Name"].get<std::string>();
	Data->hash = json[entitiesnum]["Entity Hash"].get<Hash>();
	Data->type = json[entitiesnum]["Entity Type"].get<std::string>();

	Data->initHandle = json[entitiesnum]["Initial Handle"].get<int>();
	Data->BasicProper.Collision = json[entitiesnum]["Properties"]["Collision"].get<bool>();
	Data->BasicProper.Dynamic = json[entitiesnum]["Properties"]["Dynamic"].get<bool>();
	Data->BasicProper.Frozen = json[entitiesnum]["Properties"]["Frozen"].get<bool>();
	Data->BasicProper.OpacityLevel = json[entitiesnum]["Properties"]["Opacity Level"].get<int>();
	Data->BasicProper.Visible = json[entitiesnum]["Properties"]["Visible"].get<bool>();

	Data->VehicleProper.Pearlescent = json[entitiesnum]["Vehicle Properties"]["Colour"]["Pearlescent"].get<int>();
	Data->VehicleProper.Primary = json[entitiesnum]["Vehicle Properties"]["Colour"]["Primary"].get<int>();
	Data->VehicleProper.Secondary = json[entitiesnum]["Vehicle Properties"]["Colour"]["Secondary"].get<int>();
	Data->VehicleProper.WheelCol = json[entitiesnum]["Vehicle Properties"]["Colour"]["Wheel Colour"].get<int>();

	for (int i = 0; i < 48; i++)
	{
		Data->VehicleProper.VehMod[i] = json[entitiesnum]["Vehicle Properties"]["Mods"][i].get<int>();
	}
	Data->VehicleProper.Platename = json[entitiesnum]["Vehicle Properties"]["Plate"]["Text"].get<std::string>();
	Data->VehicleProper.PlateType = json[entitiesnum]["Vehicle Properties"]["Plate"]["Index"].get<int>();

	Data->VehicleProper.tyre.Burst = (bool)json[entitiesnum]["Vehicle Properties"]["Tyre"]["Can Burst"].get<int>();
	Data->VehicleProper.tyre.Smoke.R = json[entitiesnum]["Vehicle Properties"]["Tyre"]["Smoke Colour"]["R"].get<int>();
	Data->VehicleProper.tyre.Smoke.G = json[entitiesnum]["Vehicle Properties"]["Tyre"]["Smoke Colour"]["G"].get<int>();
	Data->VehicleProper.tyre.Smoke.B = json[entitiesnum]["Vehicle Properties"]["Tyre"]["Smoke Colour"]["B"].get<int>();
	Data->VehicleProper.tyre.Type = json[entitiesnum]["Vehicle Properties"]["Tyre"]["Wheel Type"].get<int>();

	Data->VehicleProper.WindowTint = json[entitiesnum]["Vehicle Properties"]["Window Tint"].get<int>();
}


bool SpawnVehicle(VehilceData Data, Vehicle * target)
{

	if (STREAMING::IS_MODEL_VALID(Data.hash))
	{
		STREAMING::REQUEST_MODEL(Data.hash);
		while (!STREAMING::HAS_MODEL_LOADED(Data.hash))WAIT(0);//Fuck 
		Vector3 ourCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
		float forward = 5.f;
		float heading = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
		float xVector = forward * sin(degToRad(heading)) * -1.f;
		float yVector = forward * cos(degToRad(heading));
		*target = VEHICLE::CREATE_VEHICLE(Data.hash, ourCoords.x + xVector, ourCoords.y + yVector, ourCoords.z, heading, true, true);
		RequestControlOfEnt(*target);
		VEHICLE::SET_VEHICLE_ENGINE_ON(*target, true, true, true);
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(*target);
		auto networkId = NETWORK::VEH_TO_NET(*target);
		ENTITY::_SET_ENTITY_SOMETHING(*target, true);
		if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(*target))
			NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);
		WAIT(150);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Data.hash);
		return true;
	}
	else
	{
		LOG_PRINT("%u is not a vaild model", Data.hash);
		*target = -1;
		return false;
	}
	return true;
}

void DecorateVehicle(VehilceData Data)
{
	Vehicle  targetVeh;
	if (SpawnVehicle(Data, &targetVeh))
		if (targetVeh != -1)
		{
			AttachPool.insert(std::pair<int, Object>(Data.initHandle, targetVeh));//To Load Attach Vehilce
			ENTITY::SET_ENTITY_COLLISION(targetVeh, Data.BasicProper.Collision, false);
			VEHICLE::SET_VEHICLE_IS_STOLEN(targetVeh, !Data.BasicProper.Driveable);
			ENTITY::SET_ENTITY_DYNAMIC(targetVeh, Data.BasicProper.Dynamic);
			ENTITY::FREEZE_ENTITY_POSITION(targetVeh, Data.BasicProper.Frozen);
			ENTITY::SET_ENTITY_ALPHA(targetVeh, Data.BasicProper.OpacityLevel, false);
			ENTITY::SET_ENTITY_VISIBLE(targetVeh, Data.BasicProper.Visible, false);
			VEHICLE::SET_VEHICLE_EXTRA_COLOURS(targetVeh, Data.VehicleProper.Pearlescent, Data.VehicleProper.WheelCol);
			VEHICLE::SET_VEHICLE_COLOURS(targetVeh, Data.VehicleProper.Primary, Data.VehicleProper.Secondary);
			for (int i = 0; i < 48; i++)
			{
				VEHICLE::SET_VEHICLE_MOD_KIT(targetVeh, Data.VehicleProper.VehMod[i]);
			}
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(targetVeh, (char*)Data.VehicleProper.Platename.c_str());//What????
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(targetVeh, Data.VehicleProper.PlateType);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(targetVeh, Data.VehicleProper.tyre.Burst);
			VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(targetVeh, Data.VehicleProper.tyre.Smoke.R, Data.VehicleProper.tyre.Smoke.G, Data.VehicleProper.tyre.Smoke.B);
			VEHICLE::SET_VEHICLE_WHEEL_TYPE(targetVeh, Data.VehicleProper.tyre.Type);
			VEHICLE::SET_VEHICLE_WINDOW_TINT(targetVeh, Data.VehicleProper.WindowTint);
			LOG_PRINT("Load Complete!");
		}
}
#pragma endregion

void LoadJsonObjectData(const json & json, std::string entitiesnum, ObjectData * Data)
{
	Data->hash = json[entitiesnum]["Entity Hash"].get<Hash>(); // Get Entities Hash

	Data->initHandle = json[entitiesnum]["Initial Handle"].get<int>();//To Get Init Handle

	if (json[entitiesnum]["Attachments"]["Has Parent"].get<bool>())
	{
		Data->AttachTo.Bone = json[entitiesnum]["Attachments"]["Bone"].get<std::string>();
		Data->AttachTo.ParentsId = json[entitiesnum]["Attachments"]["Parent ID"].get<int>();
		Data->AttachTo.Position.x = json[entitiesnum]["Attachments"]["Position"][0].get<float>();
		Data->AttachTo.Position.y = json[entitiesnum]["Attachments"]["Position"][1].get<float>();
		Data->AttachTo.Position.z = json[entitiesnum]["Attachments"]["Position"][2].get<float>();
		Data->AttachTo.Rot.Pitch = json[entitiesnum]["Attachments"]["Rotation"][0].get<float>();
		Data->AttachTo.Rot.Roll = json[entitiesnum]["Attachments"]["Rotation"][1].get<float>();
		Data->AttachTo.Rot.Yaw = json[entitiesnum]["Attachments"]["Rotation"][2].get<float>();
	}
	Data->name = json[entitiesnum]["Display Name"].get<std::string>();
	Data->Propname = json[entitiesnum]["Prop Name"].get<std::string>();

	Data->type = json[entitiesnum]["Entity Type"].get<std::string>();

	Data->initHandle = json[entitiesnum]["Initial Handle"].get<int>();
	Data->BasicProper.Collision = json[entitiesnum]["Properties"]["Collision"].get<bool>();
	Data->BasicProper.Dynamic = json[entitiesnum]["Properties"]["Dynamic"].get<bool>();
	Data->BasicProper.Frozen = json[entitiesnum]["Properties"]["Frozen"].get<bool>();
	Data->BasicProper.OpacityLevel = json[entitiesnum]["Properties"]["Opacity Level"].get<int>();
	Data->BasicProper.Visible = json[entitiesnum]["Properties"]["Visible"].get<bool>();

}

bool SpawnObject(ObjectData Data, Object * Obj)
{
	*Obj = -1;
	if (STREAMING::IS_MODEL_IN_CDIMAGE(Data.hash)) {
		STREAMING::REQUEST_MODEL(Data.hash);
		while (!STREAMING::HAS_MODEL_LOADED(Data.hash)) WAIT(0);
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(0), 1);
		*Obj = OBJECT::CREATE_OBJECT(Data.hash, pos.x + 5.0f, pos.y + 5.0f, pos.z, true, 1, 0);
		return true;
	}
	return false;
}

void DecorateObject(ObjectData Data)
{
	Object tarObj;
	if (SpawnObject(Data, &tarObj))
	{
		if (tarObj != -1)
		{
			ENTITY::SET_ENTITY_COLLISION(tarObj, Data.BasicProper.Collision, false);
			VEHICLE::SET_VEHICLE_IS_STOLEN(tarObj, !Data.BasicProper.Driveable);
			ENTITY::SET_ENTITY_DYNAMIC(tarObj, Data.BasicProper.Dynamic);
			ENTITY::FREEZE_ENTITY_POSITION(tarObj, Data.BasicProper.Frozen);
			ENTITY::SET_ENTITY_ALPHA(tarObj, Data.BasicProper.OpacityLevel, false);
			ENTITY::SET_ENTITY_VISIBLE(tarObj, Data.BasicProper.Visible, false);
			AttachPool.insert(std::pair<int, Object>(Data.initHandle, tarObj));
		}
	}
	else
	{
		LOG_PRINT("Object Load Failed");
	}
}
/*
To Attach everything
*/
int FindObject(int Handle)
{
	std::map <int, Object>::iterator it_find;
	it_find = AttachPool.find(Handle);
	if (it_find != AttachPool.end())
	{
		return it_find->second;
	}
	else
	{
		LOG_DEBUG("Invaild Handle!");
		return -1;
	}
}
template <typename T>
void AttachEntity(T Objectdata)
{
	
	if (Objectdata.HasParents)
	{
		Vehicle ToAttached = -1;
		Object Attach = -1337;
		ToAttached = FindObject(Objectdata.AttachTo.ParentsId);

		Attach = FindObject(Objectdata.initHandle);
		
		//AttachPool.find(data.AttachTo.ParentsId)->second;//Current Object
		if (ToAttached != -1 && Attach != -1)
		{
			int Boneindex = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(
				ToAttached,
				(char*)Objectdata.AttachTo.Bone.c_str());

			ENTITY::ATTACH_ENTITY_TO_ENTITY(
				Attach,//Object
				ToAttached,//Vehicle
				Boneindex,
				Objectdata.AttachTo.Position.x,
				Objectdata.AttachTo.Position.y,
				Objectdata.AttachTo.Position.z,
				Objectdata.AttachTo.Rot.Pitch,
				Objectdata.AttachTo.Rot.Roll,
				Objectdata.AttachTo.Rot.Yaw,
				1, 0, 0, 2, 1, 1);
		}
		else
		{
			LOG_PRINT("something Wrong with initHandle Veh:%d Object %d", ToAttached, Attach);
		}
	}
}

int i = 0;
void LoadJsonVeh(std::string name)
{
	json JsonVehicle;
	std::string JsonVehFile = GetJsonFile(name);
	std::ifstream i(JsonVehFile);
	i >> JsonVehicle;
	EntCount = JsonVehicle["Other"]["Entity Count"].get<int>();
	AutherName = JsonVehicle["Headers"]["Author"].get<std::string>();
	EntType PropType;
#pragma region Basic Show notifycation
	showNotification(
		("Auther:" + AutherName + "\n Entities count: " + std::to_string(EntCount)).c_str()
	);
#pragma endregion
#pragma region Load Basic Proper
	
	AttachPool.clear();
	EntitiesNum = 0;
	for (int i = 0;
		i < 1000
		; i++)
	{
		VehilceData MyVehilce;
		ObjectData toSpawn;
		
		if (!JsonVehicle["entity_" + std::to_string(i)].is_null())//To Judge entity is exist
		{

			LOG_PRINT(("\n entity_" + std::to_string(i) + ":\n" + JsonVehicle["entity_" + std::to_string(i)]["Display Name"].get<std::string>()).c_str());
			JsonVehicle["entity_" + std::to_string(i)]["Attachments"]["Has Parent"].get<bool>();
			std::string entityname = "entity_" + std::to_string(i);
			switch (JsonGetType(JsonVehicle[entityname]["Entity Type"].get<std::string>()))
			{
			case vehicle:

				LoadJsonVehilceData(JsonVehicle, entityname, &MyVehilce);

				DecorateVehicle(MyVehilce);

				AttachEntity(MyVehilce);
				break;
			case object:

				LoadJsonObjectData(JsonVehicle, entityname, &toSpawn);

				DecorateObject(toSpawn);

				AttachEntity(toSpawn);
				//if (!toSpawn.HasParents)
				//{
				//	AttachPool.find(MyVehilce.AttachTo.ParentsId)->second;//Parents Object
				//	ENTITY::ATTACH_ENTITY_TO_ENTITY(
				//		AttachPool.find(toSpawn.initHandle)->second,//Obejct Handle
				//		AttachPool.find(MyVehilce.AttachTo.ParentsId)->second,//Vehilce Handle
				//		ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(AttachPool.find(MyVehilce.AttachTo.ParentsId)->second, (char*)toSpawn.AttachTo.Bone.c_str()),
				//		MyVehilce.AttachTo.Position.x,
				//		MyVehilce.AttachTo.Position.y,
				//		MyVehilce.AttachTo.Position.z,
				//		MyVehilce.AttachTo.Rot.Pitch,
				//		MyVehilce.AttachTo.Rot.Roll,
				//		MyVehilce.AttachTo.Rot.Yaw, 1, 0, 0, 2, 1, 1);
				//}
				break;
			case ped:
				LOG_PRINT("I didn't found about ped data, if you found, please contact me XP");
				break;
			default:
				LOG_PRINT("Unkown Type");
				break;
			}
			EntitiesNum++;
		}
		WAIT(0);
		if (EntCount <= EntitiesNum)
			break;

	}
#pragma endregion
}