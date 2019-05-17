#include "stdafx.h"
#include<fstream>
#include <sys/types.h>   


std::string ModVehfolder;
std::string XmlModVehfolder;
std::string JsonModVehfolder;

void CreateFolder(std::string path)
{
	if (!CreateDirectoryA(path.c_str(), NULL))
	{
		return;
	}
}


int st(std::string txt) { return atoi(txt.c_str()); }

//Vector6 pPos, aPos;
std::string FloatToString(float flt) {
	std::ostringstream ss;
	ss << flt;
	std::string str(ss.str());
	return str;
}
float MStringToFloat(std::string str) {
	return ::atof(str.c_str());
}

void AttachToVehilce(Any obj, Any veh, float x = 0, float y = 0, float z = 0, float rx = 0, float ry = 0, float rz = 0)
{
	ENTITY::ATTACH_ENTITY_TO_ENTITY(obj, veh, 0, x, y, z, rx, ry, rz, 1, 0, 0, 2, 1, 1);
}

Vehicle SpawnVehicleH(DWORD model, Vector3 coords, bool tpinto = 0, float heading = 0.0f) {
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model)) {
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
		Vehicle veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z, heading, 1, 1);
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh);
		if (tpinto) {
			ENTITY::SET_ENTITY_HEADING(veh, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
		}
		//VEHICLE::SET_VEHICLE_IS_STOLEN(veh, 0);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
		return veh;

	}
	return -1;
}

Vehicle SpawnVehicle(char* modelg, Vector3 coords, bool tpinto = 0, float heading = 0.0f) {

	Hash model = GAMEPLAY::GET_HASH_KEY(modelg);
	if (STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
		Vector3 ourCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
		float forward = 5.f;
		float heading = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
		float xVector = forward * sin(degToRad(heading)) * -1.f;
		float yVector = forward * cos(degToRad(heading));
		Vehicle veh = VEHICLE::CREATE_VEHICLE(model, ourCoords.x + xVector, ourCoords.y + yVector, ourCoords.z, heading, true, true);
		RequestControlOfEnt(veh);
		VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, true);
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh);
		DECORATOR::DECOR_SET_INT(veh, "MPBitset", 0);
		auto networkId = NETWORK::VEH_TO_NET(veh);
		ENTITY::_SET_ENTITY_SOMETHING(veh, true);
		if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(veh))
			NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);
		WAIT(150);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
		return veh;
	}
	return NULL;

	/*	DWORD model = GAMEPLAY::GET_HASH_KEY(modelg);
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model)) {
	STREAMING::REQUEST_MODEL(model);
	while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
	Vehicle veh = CREATE_VEHICLEB(model, coords.x, coords.y, coords.z, heading, 1, 1);
	SET_VEHICLE_ON_GROUND_PROPERLY(veh);
	if (tpinto) {
	SET_ENTITY_HEADING(veh, GET_ENTITY_HEADING(PLAYER_PED_ID()));
	SET_PED_INTO_VEHICLE(PLAYER_PED_ID(), veh, -1);
	}
	SET_VEHICLE_IS_STOLEN(veh, 0);
	return veh;
	SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}*/

}
Object SpawnPed(char* name, int selectplayer)
{
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectplayer), 1);
	if (STREAMING::IS_MODEL_IN_CDIMAGE($(name))) {
		STREAMING::REQUEST_MODEL($(name));
		while (!STREAMING::HAS_MODEL_LOADED($(name))) WAIT(0);
		//	Object obj = CREATE_OBJECT_NO_OFFSET(GET_HASH_KEY(name), coords.x, coords.y, coords.z, 1, 0, 0);
		Object obj = PED::CREATE_PED(PED_TYPE_MISSION, $(name), pos.x, pos.y, pos.z, true, 1, 0);
		return obj;
	}
	else
	{
		return -1;
	}

}
Object SpawnObject(char* name, int selectplayer)
{
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectplayer), 1);
	//	Object obj = CREATE_OBJECT_NO_OFFSET(GET_HASH_KEY(name), coords.x, coords.y, coords.z, 1, 0, 0);
	Object obj = OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY(name), pos.x, pos.y, pos.z, true, 1, 0);
	return obj;
}
Object AttachObjectToVehicleByHash(Hash hash, float x, float y, float z, float rotx, float roty, float rotz, Vehicle veh) {

	if (STREAMING::IS_MODEL_IN_CDIMAGE(hash)) {
		STREAMING::REQUEST_MODEL(hash);
		while (!STREAMING::HAS_MODEL_LOADED(hash)) WAIT(0);
		Vector3 co = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
		Object obj = OBJECT::CREATE_OBJECT_NO_OFFSET(hash, co.x, co.y, co.z, 1, 0, 0);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(obj);
		ENTITY::FREEZE_ENTITY_POSITION(obj, 1);
		ENTITY::ATTACH_ENTITY_TO_ENTITY(obj, veh, 0, x, y, z, rotx, roty, rotz, 1, 0, 1, 0, 2, 1);
		return obj;
	}
	else {
		LOG_PRINT("Invaild Model %s", hash);
		return -1;
	}
}

Object Attach(Hash hash, Vector3 objPos, int Boneindex, float x, float y, float z, float rotx, float roty, float rotz, Vehicle veh, int type)
{
	Object obj;
	if (STREAMING::IS_MODEL_IN_CDIMAGE(hash)) {
		STREAMING::REQUEST_MODEL(hash);
		while (!STREAMING::HAS_MODEL_LOADED(hash)) WAIT(0);
		switch (type)
		{
		case 1:
			obj = PED::CREATE_PED(PED_TYPE_SPECIAL, hash, objPos.x, objPos.y, objPos.z, 0.0f, false, false);
			LOG_DEBUG("Create Ped");
			break;
		case 2:
			obj = VEHICLE::CREATE_VEHICLE(hash, objPos.x, objPos.y, objPos.z, 0.0f, false, false);
			LOG_DEBUG("Create Vehicle");
			break;
		case 3:
			LOG_DEBUG("Create Object");
			obj = OBJECT::CREATE_OBJECT_NO_OFFSET(hash, objPos.x, objPos.y, objPos.z, 0, 0, 0);//通过载具骨骼创建
			break;
		default:
			LOG_DEBUG("Unknown Entities");
			break;
		}
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(obj);
		ENTITY::FREEZE_ENTITY_POSITION(obj, 1);
		ENTITY::ATTACH_ENTITY_TO_ENTITY(obj, veh, Boneindex, x, y, z, rotx, roty, rotz, 1, 0, 1, 0, 2, 1);
		return obj;
	}
	else {
		LOG_PRINT("Invaild Model %s", hash);
		return -1;
	}
}
void AttachPed(Hash hash, Vector3 objPos, int Boneindex, float x, float y, float z, float rotx, float roty, float rotz, Vehicle veh)
{
	if (STREAMING::IS_MODEL_IN_CDIMAGE(hash)) {
		STREAMING::REQUEST_MODEL(hash);
		while (!STREAMING::HAS_MODEL_LOADED(hash))
			WAIT(0);

	}
	else
	{
		LOG_PRINT("Invaild Model %d", hash);
	}
}
void RequestControlOfEnt(DWORD entity)
{
	int tick = 0;

	while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity) && tick <= 150)
	{
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
		tick++;
	}
}

std::string ReadPresetKey(std::string name, std::string section, std::string key) {
	std::string iniPath = ModVehfolder + "\\" + name + ".ini";
	char retVal[255];
	GetPrivateProfileStringA(section.c_str(), key.c_str(), "", retVal, 255, iniPath.c_str());
	return retVal;
}
bool DoesPresetKeyExists(std::string name, std::string section, std::string key) {
	return ReadPresetKey(name, section, key).length() > 0;
}

std::vector<int> ModTypes = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 23, 24, 26, 25, 48, 37, 27, 28, 30, 33, 34, 35, 38, 39, 45 };

void SetModVehFolder()
{
	ModVehfolder = Paths::GetModuleFolder(Paths::GetOurModuleHandle()) + modDir + "\\Mod Vehicle" + "\\Ini Mod";
	LOG_PRINT((char*)("IniPath Found:" + ModVehfolder).c_str());
	XmlModVehfolder = Paths::GetModuleFolder(Paths::GetOurModuleHandle()) + modDir + "\\Mod Vehicle" + "\\Xml Mod";
	LOG_PRINT((char*)("XmlPath Found:" + XmlModVehfolder).c_str());
	JsonModVehfolder = Paths::GetModuleFolder(Paths::GetOurModuleHandle()) + modDir + "\\Mod Vehicle" + "\\Json Mod";
	LOG_PRINT((char*)("Json Path Found:" + JsonModVehfolder).c_str());
}

void LoadIniVeh(std::string name)
{
	std::string path = ModVehfolder + "\\" + name + ".ini";
	LOG_PRINT("Start to load %s", name.c_str());
	CSimpleIniA Mod;
	CSimpleIniA::TNamesDepend sections;
	Mod.SetUnicode();
	Mod.LoadFile(path.c_str());
	Mod.GetAllSections(sections);
	CSimpleIni::TNamesDepend::const_iterator iSection = sections.begin();
	const char *pszSection;
	const char *pItem;
	const char *pszVal;
	Vehicle veh;


#pragma region ELUSIVE Section
	if (std::strcmp(Mod.GetValue("0", "Model", "Elusive"), "Elusive"))
	{
		//LOG_PRINT("get Elusive Type");
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false)) veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
		Color tyre, neon;
		Hash Model = static_cast<DWORD>(atoll(Mod.GetValue("Vehicle", "Model")));
		int primary = atoi(Mod.GetValue("Vehicle", "PrimaryPaint", "255"));
		int secondary = atoi(Mod.GetValue("Vehicle", "SecondaryPaint", "255"));
		tyre.R = atoi(Mod.GetValue("Vehicle", "SmokeR", "255"));
		tyre.G = atoi(Mod.GetValue("Vehicle", "SmokeG", "255"));
		tyre.B = atoi(Mod.GetValue("Vehicle", "SmokeB", "255"));
		int whelltype = atoi(Mod.GetValue("Vehicle", "WheelsType", "0"));
		neon.R = atoi(Mod.GetValue("Vehicle", "NeonR", "255"));
		neon.G = atoi(Mod.GetValue("Vehicle", "NeonG", "255"));
		neon.B = atoi(Mod.GetValue("Vehicle", "NeonB", "255"));
		std::string text = Mod.GetValue("Vehicle", "PlateText", "PRJ");
		float dirlvl = MStringToFloat(Mod.GetValue("Vehicle", "dirt level", "0"));
		veh = SpawnVehicleH(Model, ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0, 5, 0), 1);
		RequestControlOfEnt(veh);
		VEHICLE::SET_VEHICLE_COLOURS(veh, primary, secondary);
		VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, tyre.R, tyre.G, tyre.B);
		VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, neon.R, neon.G, neon.B);
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, (char*)text.c_str());
		VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, dirlvl);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, whelltype);
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, 0);
		WAIT(10);

		for (; iSection != sections.end(); ++iSection)
		{
			pszSection = iSection->pItem;
			if (!std::strcmp(pszSection, "Vehicle"))
			{
				continue;
			}
			Hash ObjModel = static_cast<DWORD>(atoll(Mod.GetValue(pszSection, "model")));
			float xoffset = MStringToFloat(Mod.GetValue(pszSection, "X"));
			float yoffset = MStringToFloat(Mod.GetValue(pszSection, "Y"));
			float zoffset = MStringToFloat(Mod.GetValue(pszSection, "Z"));
			float pitch = MStringToFloat(Mod.GetValue(pszSection, "RotX"));
			float roll = MStringToFloat(Mod.GetValue(pszSection, "RotY"));
			float yaw = MStringToFloat(Mod.GetValue(pszSection, "RotZ"));
			int collision = st(Mod.GetValue(pszSection, "collision", "0"));
			//LOG_DEBUG(Mod.GetValue(pszSection, "Model"));
			AttachObjectToVehicleByHash(ObjModel, xoffset, yoffset, zoffset, pitch, roll, yaw, veh);
		}
	}
#pragma endregion
#pragma region Jordan D3SK1NG NIGGER Type
	else
	{
		//	LOG_PRINT("Jordan Type");

		Color tyre, neon;
		Hash Model = static_cast<DWORD>(atoll(Mod.GetValue("Vehicle", "model")));
		int primary = atoi(Mod.GetValue("Vehicle", "primary paint", "255"));
		int secondary = atoi(Mod.GetValue("Vehicle", "secondary paint", "255"));
		tyre.R = atoi(Mod.GetValue("Vehicle", "tyre smoke red", "255"));
		tyre.G = atoi(Mod.GetValue("Vehicle", "tyre smoke green", "255"));
		tyre.B = atoi(Mod.GetValue("Vehicle", "tyre smoke blue", "255"));
		int whelltype = atoi(Mod.GetValue("Vehicle", "wheel colour", "0"));
		neon.R = atoi(Mod.GetValue("Vehicle", "neon red", "255"));
		neon.G = atoi(Mod.GetValue("Vehicle", "neon grenn", "255"));
		neon.B = atoi(Mod.GetValue("Vehicle", "neon blue", "255"));
		std::string text = Mod.GetValue("Vehicle", "plate text", "PRJ");
		float dirlvl = MStringToFloat(Mod.GetValue("Vehicle", "dirt level"));
		Vehicle veh = SpawnVehicleH(Model, ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0, 5, 0), 1);
		RequestControlOfEnt(veh);
		VEHICLE::SET_VEHICLE_COLOURS(veh, primary, secondary);
		VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, tyre.R, tyre.G, tyre.B);
		VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, neon.R, neon.G, neon.B);
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, (char*)text.c_str());
		VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, dirlvl);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, whelltype);
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, 0);
		WAIT(10);
		for (; iSection != sections.end(); ++iSection)
		{
			pszSection = iSection->pItem;
			std::string sec = pszSection;
			if (!std::strcmp(pszSection, "Vehicle"))
			{
				continue;
			}
			if (sec.find("Attached Object") != -1)
			{
				Hash ObjModel = static_cast<DWORD>(atoll(Mod.GetValue(pszSection, "model")));
				//	LOG_DEBUG("Object Modle %lld", ObjModel);
				float xoffset = MStringToFloat(Mod.GetValue(pszSection, "x offset"));
				float yoffset = MStringToFloat(Mod.GetValue(pszSection, "y offset"));
				float zoffset = MStringToFloat(Mod.GetValue(pszSection, "z offset"));
				float pitch = MStringToFloat(Mod.GetValue(pszSection, "pitch"));
				float roll = MStringToFloat(Mod.GetValue(pszSection, "roll"));
				float yaw = MStringToFloat(Mod.GetValue(pszSection, "yaw"));
				int collision = st(Mod.GetValue(pszSection, "collision"));
				//LOG_PRINT(Mod.GetValue(pszSection, "model name"));
				AttachObjectToVehicleByHash(ObjModel, xoffset, yoffset, zoffset, pitch, roll, yaw, veh);
			}
			WAIT(0);

		}
#pragma endregion
	}
}

std::string GetJsonFile(std::string name)
{
	return JsonModVehfolder + "\\" + name + ".json";
}

TiXmlDocument XMLMod;
TiXmlElement * rootElement;
TiXmlElement * firstElement;
Hash ModelHash;
char *endptr;
Vector3 VehBonePos;
Vehicle veh;
Hash ObjModel;
int modeltype = 0;
int BoneIndex = 0;
int InitHandle;
std::map <int, Object> ObjectPool;/*
unsigned long HextoDec(std::string hex)
{
	unsigned long    nValude = 0;
	LOG_DEBUG("Hex:%s", hex);
	sscanf_s(hex.c_str(), "%x", &nValude);
	LOG_DEBUG("Dex: %x", nValude);
	return nValude;
}*/
void LoadXmlVeh(std::string name)
{
	std::string xmlPath = XmlModVehfolder + "\\" + name + ".xml";
	LOG_PRINT("Load File Path:%s", xmlPath);
	LOG_PRINT("Start to load XML MOD Vehicle \t FileName: %s", (char*)name.c_str());

	if (XMLMod.LoadFile((char*)xmlPath.c_str()))
	{
		ObjectPool.clear();//Clean up Create Object Pool
		//指向 <Vehicle>
		rootElement = XMLMod.RootElement();
		//LOG_PRINT("Root Value:%s", rootElement->Value());

		//指向 <Model Hash>
		firstElement = rootElement->FirstChild("ModelHash")->ToElement();
		LOG_DEBUG("Get Mod Hash:%s", firstElement->GetText());
		ModelHash = StringtoDec(firstElement->GetText());
		LOG_DEBUG("Model Dec Hash:%d", ModelHash);
		//static_cast<DWORD>(atoll());//获取 ModelHash 载具用
		veh = SpawnVehicleH(ModelHash, ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0, 5, 0), 1);
		LOG_DEBUG("is Visible :%s", rootElement->FirstChild("IsVisible")->ToElement()->GetText());
		if (!strcmp(rootElement->FirstChild("IsVisible")->ToElement()->GetText(), "true"))
			ENTITY::SET_ENTITY_VISIBLE(veh, true, 0);
		else
			ENTITY::SET_ENTITY_VISIBLE(veh, false, 0);
		ENTITY::SET_ENTITY_ALPHA(veh, atoi(rootElement->FirstChild("OpacityLevel")->ToElement()->GetText()), 0);//设置隐形
		//IsVisible
		if (rootElement->Attribute("menyoo_ver") != nullptr)
		{
			InitHandle = atoi(rootElement->FirstChild("InitialHandle")->ToElement()->GetText());
		}
		else
		{
			InitHandle = veh;
		}

		LOG_DEBUG("InitHandle : %d", InitHandle);
		ObjectPool.insert(std::pair<int, Object>(InitHandle, veh));
		RequestControlOfEnt(veh);
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, 0);
		WAIT(10);
		LOG_DEBUG("Start to Attach!");
		TiXmlHandle XmlHandle(&XMLMod);
		TiXmlElement *AttachObj = XmlHandle.FirstChild("Vehicle").FirstChild("SpoonerAttachments").FirstChild("Attachment").ToElement();
		while (AttachObj != NULL)//Start Attach Object
		{
			LOG_DEBUG("attach Object Hash Name:%s", AttachObj->FirstChild("HashName")->ToElement()->GetText());
			ObjModel = StringtoDec(AttachObj->FirstChild("ModelHash")->ToElement()->GetText());//?
			LOG_DEBUG("ObjModel %d", ObjModel);
			BoneIndex = atoi(AttachObj->FirstChild("Attachment")->FirstChild("BoneIndex")->ToElement()->GetText());
			LOG_DEBUG("BoneIndex %d", BoneIndex);
			VehBonePos = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(veh, BoneIndex); //获取到世界坐标
			LOG_DEBUG("Bone Pos:%f,%f,%f", VehBonePos.x, VehBonePos.y, VehBonePos.z);
			int ObjHandle = atoi(AttachObj->FirstChild("InitialHandle")->ToElement()->GetText());

			//ObjModel = $(AttachObj->FirstChild("HashName")->ToElement()->GetText());
			float xoffset = MStringToFloat(AttachObj->FirstChild("Attachment")->FirstChild("X")->ToElement()->GetText());
			float yoffset = MStringToFloat(AttachObj->FirstChild("Attachment")->FirstChild("Y")->ToElement()->GetText());
			float zoffset = MStringToFloat(AttachObj->FirstChild("Attachment")->FirstChild("Z")->ToElement()->GetText());
			float pitch = MStringToFloat(AttachObj->FirstChild("Attachment")->FirstChild("Pitch")->ToElement()->GetText());
			float roll = MStringToFloat(AttachObj->FirstChild("Attachment")->FirstChild("Roll")->ToElement()->GetText());
			float yaw = MStringToFloat(AttachObj->FirstChild("Attachment")->FirstChild("Yaw")->ToElement()->GetText());
			int collision = 0;
			modeltype = atoi(AttachObj->FirstChild("Type")->ToElement()->GetText());
			Object obj;
			//TODO:something wrong with request Model
			if (STREAMING::IS_MODEL_IN_CDIMAGE(ObjModel)) {
				if (STREAMING::IS_MODEL_VALID(ObjModel))
				{
					LOG_PRINT("Model is vaild, %s", AttachObj->FirstChild("HashName")->ToElement()->GetText());
				}
				else
				{
					ObjModel = $(AttachObj->FirstChild("HashName")->ToElement()->GetText());
				}
				STREAMING::REQUEST_MODEL(ObjModel);
				while (!STREAMING::HAS_MODEL_LOADED(ObjModel)) WAIT(0);
				switch (modeltype)
				{
				case 1:
					obj = PED::CREATE_PED(26, ObjModel, VehBonePos.x, VehBonePos.y, VehBonePos.z, 0.0f, false, false);
					LOG_DEBUG("Create Ped");
					break;
				case 2:
					obj = VEHICLE::CREATE_VEHICLE(ObjModel, VehBonePos.x, VehBonePos.y, VehBonePos.z, 0.0f, false, false);
					LOG_DEBUG("Create Vehicle");
					break;
				case 3:
					LOG_DEBUG("Create Object");
					obj = OBJECT::CREATE_OBJECT_NO_OFFSET(ObjModel, VehBonePos.x, VehBonePos.y, VehBonePos.z, 0, 0, 0);//通过载具骨骼创建
					break;
				default:
					LOG_DEBUG("Unknown Entities");
					break;
				}
				ENTITY::SET_ENTITY_VISIBLE(obj, StringtoBool(AttachObj->FirstChild("IsVisible")->ToElement()->GetText()), 0);
				ENTITY::SET_ENTITY_ALPHA(obj, atoi(AttachObj->FirstChild("OpacityLevel")->ToElement()->GetText()), 0);//设置隐形
				LOG_DEBUG("Alpha:%s", AttachObj->FirstChild("OpacityLevel")->ToElement()->GetText());
			}
			else {
				LOG_PRINT("Invaild Model %s", ObjModel);
			}
			ObjectPool.insert(std::pair<int, Object>(ObjHandle, obj));
			if (AttachObj->FirstChild("Attachment")->FirstChild("TaskSequence") != nullptr)
			{
				Vector3 RelativePosition, RelativeRotation;
				Color FxCol;
				std::string AssetName = AttachObj->FirstChild("Attachment")->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("AssetName")->ToElement()->GetText();
				std::string EffectName = AttachObj->FirstChild("Attachment")->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("EffectName")->ToElement()->GetText();
				float Scale = MStringToFloat(AttachObj->FirstChild("Attachment")->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("Scale")->ToElement()->GetText());
				FxCol.R = (float)atoi(AttachObj->FirstChild("Attachment")->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("Colour")->ToElement()->Attribute("R"));
				FxCol.G = (float)atoi(AttachObj->FirstChild("Attachment")->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("Colour")->ToElement()->Attribute("G"));
				FxCol.B = (float)atoi(AttachObj->FirstChild("Attachment")->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("Colour")->ToElement()->Attribute("B"));
				FxCol.A = (float)atoi(AttachObj->FirstChild("Attachment")->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("Colour")->ToElement()->Attribute("A"));

				RelativePosition.x = (float)atoi(AttachObj->FirstChild("Attachment")->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("RelativePosition")->ToElement()->Attribute("X"));
				RelativePosition.y = (float)atoi(AttachObj->FirstChild("Attachment")->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("RelativePosition")->ToElement()->Attribute("Y"));
				RelativePosition.z = (float)atoi(AttachObj->FirstChild("Attachment")->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("RelativePosition")->ToElement()->Attribute("Z"));

				RelativeRotation.x = (float)atoi(AttachObj->FirstChild("Attachment")->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("RelativeRotation")->ToElement()->Attribute("X"));
				RelativeRotation.y = (float)atoi(AttachObj->FirstChild("Attachment")->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("RelativeRotation")->ToElement()->Attribute("Y"));
				RelativeRotation.z = (float)atoi(AttachObj->FirstChild("Attachment")->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("RelativeRotation")->ToElement()->Attribute("Z"));


				STREAMING::REQUEST_NAMED_PTFX_ASSET((char*)AssetName.c_str());
				while (STREAMING::HAS_PTFX_ASSET_LOADED())
				{
					WAIT(0);
				}
				GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL((char*)EffectName.c_str());
				int ptfxHandle = GRAPHICS::_START_PARTICLE_FX_LOOPED_ON_ENTITY_BONE((char*)EffectName.c_str(), obj, 0, 0, 0, 0, 0, 0, BoneIndex, Scale, 0, 0, 0);
				GRAPHICS::SET_PARTICLE_FX_LOOPED_COLOUR(ptfxHandle, FxCol.R, FxCol.G, FxCol.B, true);
			}
			int AttachTo;
			if (rootElement->Attribute("menyoo_ver") != nullptr)
			{
				AttachTo = atoi(AttachObj->FirstChild("Attachment")->FirstChild("AttachedTo")->ToElement()->GetText());
			}
			else
			{
				AttachTo = veh;
			}


			std::map <int, Object>::iterator it_find;
			it_find = ObjectPool.find(AttachTo);
			if (it_find != ObjectPool.end())
			{
				veh = it_find->second;
			}
			else
			{
				LOG_DEBUG("Invaild Handle!");
			}
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(obj);
			ENTITY::FREEZE_ENTITY_POSITION(obj, 1);
			ENTITY::ATTACH_ENTITY_TO_ENTITY(obj, veh, BoneIndex, xoffset, yoffset, zoffset, pitch, roll, yaw, 1, 0, 0, 2, 0, 1);


			/*STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_agencyheist");
			GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL("scr_agencyheist");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_fbi_dd_breach_smoke", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 31086, 0.2f, 0, 0, 0);
			GRAPHICS::START_PARTICLE_FX_LOOPED_ON_ENTITY();*/

			//0 = no entity
			//	1 = ped
			//	2 = vehicle
			//	3 = object
			WAIT(0);
			AttachObj = AttachObj->NextSiblingElement();
		}
		//LOG_PRINT("Attach Complete!");
		showNotification((char*)("Load Complete!" + xmlPath).c_str());
	}
	else
	{
		showNotification("XML File is invaild!");
	}
}


std::vector<std::string> GetIniFile()
{
	std::string iniPath = ModVehfolder;
	std::vector<std::string> ret;
	DIR *dir;
	struct dirent *ent;
	LOG_DEBUG((char*)("Fucking Path" + iniPath).c_str());
	if ((dir = opendir((char*)iniPath.c_str())) != NULL) {
		/* print all the files and directories within directory */
		LOG_DEBUG("Done");
		while ((ent = readdir(dir)) != NULL) {
			ret.push_back(ent->d_name);
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		LOG_DEBUG("Error");
		ret.push_back("ERROR");
	}
	return ret;
}
