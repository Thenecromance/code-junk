#include "stdafx.h"
TiXmlDocument Map;
TiXmlElement * MaprootElement;
//TiXmlElement * firstElement;
std::string MapFolder;
std::map <int, Object> MapObjectPool;
std::vector <Hash> MapObjectDataBase;

void SetMapPath(std::string Path)
{
	MapFolder = Path + "\\";
}


EntType GetEntType(std::string type)
{
	switch (atoi(type.c_str()))
	{
	case 1:
		return ped;
		break;
	case 2:
		return vehicle;
		break;
	case 3:
		return object;
		break;
	default:
		LOG_DEBUG("Wrong Type Entity");
	case 0:
		return no_entity;
		break;
	}
}
EntType getType(std::string type)
{
	if ("Vehicle" == type)
		return vehicle;
	if ("Ped" == type)
		return ped;
	if ("Prop" == type)
		return object;

	return no_entity;
}
//bool bMarker = 1;
//bool SetMarker(Entity entity, float x, float y, float z) 
//{ 
//	if (DOES_ENTITY_EXIST(entity)) 
//	{ 
//		DRAW_MARKER(2, x, y, z + 2, 0, 0, 0, 0, 180, 0, 2, 2, 2, 255, 128, 0, 50, 0, 1, 1, 0, 0, 0, 0);		
//		return 1;
//	}	
//	return 0;
//}
//void main() {
//	while (true) {		// LOOP		
//		if (bMarker)	
//		{			
//			if (IS_PED_IN_ANY_VEHICLE(PLAYER_PED_ID(), 1))	
//			{				
//				Vehicle car = GET_VEHICLE_PED_IS_USING(PLAYER_PED_ID());	
//				Vector3 v3 = GET_ENTITY_COORDS(car, 1);			
//				SetMarker(car, v3.x, v3.y, v3.z);	
//			}	
//		}	
//		WAIT(0);	
//	}
//}

//TODO:There is something wrong with Menyoo
std::vector<Hash> TempHash;
const int ARR_SIZE = 1024;
void LoadMenyooMap(TiXmlElement* root)
{
	Vector6 ModlePos;
	Vector3 Coord;
	Vector3 ObjBonePos;
	bool changebool = false;
	Object Attachto;
	int InitHandle;
	int BoneIndex = 0;
	Hash ModelHash, newHash;


	if (!MapObjectDataBase.empty())
	{
		MapObjectDataBase.clear();
	}

	MapObjectPool.clear();

	LOG_PRINT("MaprootElement %s", root->Value());
	Coord.x = MStringToFloat(root->FirstChild("ReferenceCoords")->FirstChild("X")->ToElement()->GetText());
	Coord.y = MStringToFloat(root->FirstChild("ReferenceCoords")->FirstChild("Y")->ToElement()->GetText());
	Coord.z = MStringToFloat(root->FirstChild("ReferenceCoords")->FirstChild("Z")->ToElement()->GetText());
	//Clean Area
	/*GAMEPLAY::_CLEAR_AREA_OF_EVERYTHING(Coord.x, Coord.y, Coord.z, 100, false, false, false, false);
	GAMEPLAY::CLEAR_AREA_OF_VEHICLES(Coord.x, Coord.y, Coord.z, 100, false, false, false, false, false);
	GAMEPLAY::CLEAR_AREA_OF_OBJECTS(Coord.x, Coord.y, Coord.z, 100, 0);
	GAMEPLAY::CLEAR_AREA_OF_PEDS(Coord.x, Coord.y, Coord.z, 100, 0);
	GAMEPLAY::CLEAR_AREA_OF_COPS(Coord.x, Coord.y, Coord.z, 100, 0);
	GAMEPLAY::CLEAR_AREA_OF_PROJECTILES(Coord.x, Coord.y, Coord.z, 100, 0);*/

	teleport(Coord.x, Coord.y, Coord.z);

	Entity tobjects[ARR_SIZE];
	int tcount;
	GAMEPLAY::CLEAR_AREA(Coord.x, Coord.y, Coord.z, 1000.0f, true, false, false, false);
	GAMEPLAY::_CLEAR_AREA_OF_EVERYTHING(Coord.x, Coord.y, Coord.z, 1000.0f, true, false, false, false);
	tcount = rage::GetAllWorld_count(PoolTypeObject, ARR_SIZE, tobjects);
	
	for (int i = 0; i < tcount; i++)
	{
		ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&tobjects[i]);
	}
	WAIT(1000);
	TiXmlElement *MapObj = MaprootElement->FirstChild("Placement")->ToElement();
	//CAM::DO_SCREEN_FADE_OUT(1000);
	int spawnobjnum = 0;
	std::string isinmem = "true";

	while (MapObj != NULL)//Start to load Object or smt else shit;
	{
		int count, pedcount, vehiclecount;
		Object objects[ARR_SIZE];
		count = rage::GetAllWorld_count(PoolTypeObject, ARR_SIZE, objects);
		pedcount= rage::GetAllWorld_count(PoolTypePed, ARR_SIZE, objects);
		vehiclecount = rage::GetAllWorld_count(PoolTypeVehicle, ARR_SIZE, objects);
		spawnobjnum++;
		if (!strcmp(MapObj->Value(), "Placement"))//Load Obejct Shit
		{
			//LOG_PRINT("Load Object Name %s", MapObj->FirstChild("HashName")->ToElement()->GetText());
			InitHandle = atoi(MapObj->FirstChild("InitialHandle")->ToElement()->GetText());
			ModelHash = StringtoDec(MapObj->FirstChild("ModelHash")->ToElement()->GetText());
			ModlePos.x = MStringToFloat(MapObj->FirstChild("PositionRotation")->FirstChild("X")->ToElement()->GetText());
			ModlePos.y = MStringToFloat(MapObj->FirstChild("PositionRotation")->FirstChild("Y")->ToElement()->GetText());
			ModlePos.z = MStringToFloat(MapObj->FirstChild("PositionRotation")->FirstChild("Z")->ToElement()->GetText());
			ModlePos.Rotx = MStringToFloat(MapObj->FirstChild("PositionRotation")->FirstChild("Pitch")->ToElement()->GetText());
			ModlePos.Roty = MStringToFloat(MapObj->FirstChild("PositionRotation")->FirstChild("Roll")->ToElement()->GetText());
			ModlePos.Rotz = MStringToFloat(MapObj->FirstChild("PositionRotation")->FirstChild("Yaw")->ToElement()->GetText());
			Object obj;

			if (STREAMING::IS_MODEL_IN_CDIMAGE(ModelHash))
			{
				if (!STREAMING::HAS_MODEL_LOADED(ModelHash))
				{
					STREAMING::REQUEST_MODEL(ModelHash);
					TempHash.push_back(ModelHash);
				}
				while (!STREAMING::HAS_MODEL_LOADED(ModelHash)) WAIT(10);

				switch (GetEntType(MapObj->FirstChild("Type")->ToElement()->GetText()))
				{
				case no_entity:
					break;
				case ped:
					obj = PED::CREATE_PED(26, ModelHash, ModlePos.x, ModlePos.y, ModlePos.z, 0.0f, false, false);
					break;
				case vehicle:
					obj = VEHICLE::CREATE_VEHICLE(ModelHash, ModlePos.x, ModlePos.y, ModlePos.z, 0.0f, false, false);
					//VEHICLE::SET_VEHICLE_DOOR_OPEN(obj);
					//VEHICLE::SET_VEHICLE_DOOR_BROKEN
					break;
				case object:
					//OBJECT::_HIGHLIGHT_PLACEMENT_COORDS(ModlePos.x, ModlePos.y, ModlePos.z,2);
					obj = OBJECT::CREATE_OBJECT_NO_OFFSET(ModelHash, ModlePos.x, ModlePos.y, ModlePos.z, 1, 1, 1);//通过载具骨骼创建						
					break;
				default:
					break;
				}
				
				ENTITY::FREEZE_ENTITY_POSITION(obj, StringtoBool(MapObj->FirstChild("FrozenPos")->ToElement()->GetText()));
				ENTITY::SET_ENTITY_ROTATION(obj, ModlePos.Rotx, ModlePos.Roty, ModlePos.Rotz, 2, true);
				ENTITY::SET_ENTITY_VISIBLE(obj, StringtoBool(MapObj->FirstChild("IsVisible")->ToElement()->GetText()), 0);
				ENTITY::SET_ENTITY_ALPHA(obj, atoi(MapObj->FirstChild("OpacityLevel")->ToElement()->GetText()), 0);
				ENTITY::SET_ENTITY_LOD_DIST(obj, atoi(MapObj->FirstChild("LodDistance")->ToElement()->GetText()));
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(obj, 1, 0);
				ENTITY::SET_ENTITY_MAX_HEALTH(obj, atoi(MapObj->FirstChild("MaxHealth")->ToElement()->GetText()));
				ENTITY::SET_ENTITY_HAS_GRAVITY(obj, StringtoBool(MapObj->FirstChild("HasGravity")->ToElement()->GetText()));
				ENTITY::SET_ENTITY_INVINCIBLE(obj, StringtoBool(MapObj->FirstChild("IsInvincible")->ToElement()->GetText()));		
				LOG_PRINT("[Spawned:%d][Type:%s][HashName:%s][isInMem:%s][world object count:%d][ped count %d][veh:%d]",
					spawnobjnum,
					MapObj->FirstChild("Type")->ToElement()->GetText(),
					MapObj->FirstChild("HashName")->ToElement()->GetText(),
					isinmem,
					count,
					pedcount,
					vehiclecount
				);
				isinmem = "true";
				int AttachTo;
				if (StringtoBool(MapObj->FirstChild("Attachment")->ToElement()->Attribute("isAttached")))
				{
					if (
						(AttachTo = atoi(MapObj->FirstChild("Attachment")->FirstChild("AttachedTo")->ToElement()->GetText()))//if(0)
						)
					{
					}
					else
					{
						BoneIndex = atoi(MapObj->FirstChild("Attachment")->FirstChild("BoneIndex")->ToElement()->GetText());
						//ObjBonePos = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(obj, BoneIndex);

						float xoffset = MStringToFloat(MapObj->FirstChild("Attachment")->FirstChild("X")->ToElement()->GetText());
						float yoffset = MStringToFloat(MapObj->FirstChild("Attachment")->FirstChild("Y")->ToElement()->GetText());
						float zoffset = MStringToFloat(MapObj->FirstChild("Attachment")->FirstChild("Z")->ToElement()->GetText());
						float pitch = MStringToFloat(MapObj->FirstChild("Attachment")->FirstChild("Pitch")->ToElement()->GetText());
						float roll = MStringToFloat(MapObj->FirstChild("Attachment")->FirstChild("Roll")->ToElement()->GetText());
						float yaw = MStringToFloat(MapObj->FirstChild("Attachment")->FirstChild("Yaw")->ToElement()->GetText());
						std::map <int, Object>::iterator it_find;
						it_find = MapObjectPool.find(AttachTo);
						if (it_find != MapObjectPool.end())
						{
							Attachto = it_find->second;
						}
						else
						{
							LOG_DEBUG("Invaild Handle!");
						}
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(obj);
						ENTITY::FREEZE_ENTITY_POSITION(obj, 1);
						ENTITY::ATTACH_ENTITY_TO_ENTITY(obj, Attachto, BoneIndex, xoffset, yoffset, zoffset, pitch, roll, yaw, 1, 0, 0, 2, 0, 1);
					}

				}
			}
			//To load Task
			if (MapObj->FirstChild("TaskSequence") != nullptr)
			{
				Vector3 RelativePosition, RelativeRotation;
				Color FxCol;
				std::string AssetName = MapObj->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("AssetName")->ToElement()->GetText();
				std::string EffectName = MapObj->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("EffectName")->ToElement()->GetText();
				float Scale = MStringToFloat(MapObj->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("Scale")->ToElement()->GetText());
				FxCol.R = (float)atoi(MapObj->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("Colour")->ToElement()->Attribute("R"));
				FxCol.G = (float)atoi(MapObj->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("Colour")->ToElement()->Attribute("G"));
				FxCol.B = (float)atoi(MapObj->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("Colour")->ToElement()->Attribute("B"));
				FxCol.A = (float)atoi(MapObj->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("Colour")->ToElement()->Attribute("A"));

				RelativePosition.x = (float)atoi(MapObj->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("RelativePosition")->ToElement()->Attribute("X"));
				RelativePosition.y = (float)atoi(MapObj->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("RelativePosition")->ToElement()->Attribute("Y"));
				RelativePosition.z = (float)atoi(MapObj->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("RelativePosition")->ToElement()->Attribute("Z"));

				RelativeRotation.x = (float)atoi(MapObj->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("RelativeRotation")->ToElement()->Attribute("X"));
				RelativeRotation.y = (float)atoi(MapObj->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("RelativeRotation")->ToElement()->Attribute("Y"));
				RelativeRotation.z = (float)atoi(MapObj->FirstChild("TaskSequence")->FirstChild("Task")->FirstChild("RelativeRotation")->ToElement()->Attribute("Z"));


				STREAMING::REQUEST_NAMED_PTFX_ASSET((char*)AssetName.c_str());
				while (!STREAMING::HAS_NAMED_PTFX_ASSET_LOADED((char*)AssetName.c_str()))WAIT(0);
				GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL((char*)EffectName.c_str());
				int ptfxHandle = GRAPHICS::_START_PARTICLE_FX_LOOPED_ON_ENTITY_BONE((char*)EffectName.c_str(), obj, 0, 0, 0, 0, 0, 0, BoneIndex, Scale, 0, 0, 0);
				GRAPHICS::SET_PARTICLE_FX_LOOPED_COLOUR(ptfxHandle, (float)FxCol.R, (float)FxCol.G, (float)FxCol.B, true);
			}
			MapObjectPool.insert(std::pair<int, Object>(InitHandle, obj));
			MapObjectDataBase.insert(MapObjectDataBase.begin(), ModelHash);

			//ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED((Entity*)obj);
		}
		//BUG
		else if (!strcmp(MapObj->Value(), "Marker"))//Draw Marker
		{
			LOG_PRINT("Marker Name %s", MapObj->FirstChild("Name")->ToElement()->GetText());
			Color Marker;
			Vector6 MarkerPos;//Marker Position
			Vector3 MarkerLoc;//Marker Location
			int Type = atoi(MapObj->FirstChild("Type")->ToElement()->GetText());
			int Scale = atoi(MapObj->FirstChild("Scale")->ToElement()->GetText());
			Marker.R = atoi(MapObj->FirstChild("Colour")->ToElement()->Attribute("R"));//Get Marker Color
			Marker.G = atoi(MapObj->FirstChild("Colour")->ToElement()->Attribute("G"));//Get Marker Color
			Marker.B = atoi(MapObj->FirstChild("Colour")->ToElement()->Attribute("B"));//Get Marker Color
			Marker.A = atoi(MapObj->FirstChild("Colour")->ToElement()->Attribute("A"));//Get Marker Color

			MarkerPos.x = MStringToFloat(MapObj->FirstChild("Position")->ToElement()->Attribute("X"));//Marker Position
			MarkerPos.y = MStringToFloat(MapObj->FirstChild("Position")->ToElement()->Attribute("Y"));//Marker Position
			MarkerPos.z = MStringToFloat(MapObj->FirstChild("Position")->ToElement()->Attribute("Z"));//Marker Position
			MarkerPos.Rotx = MStringToFloat(MapObj->FirstChild("Rotation")->ToElement()->Attribute("X"));
			MarkerPos.Roty = MStringToFloat(MapObj->FirstChild("Rotation")->ToElement()->Attribute("Y"));
			MarkerPos.Rotz = MStringToFloat(MapObj->FirstChild("Rotation")->ToElement()->Attribute("Z"));

			//Marker Location
			MarkerLoc.x = MStringToFloat(MapObj->FirstChild("Destination")->ToElement()->Attribute("X"));
			MarkerLoc.y = MStringToFloat(MapObj->FirstChild("Destination")->ToElement()->Attribute("Y"));
			MarkerLoc.z = MStringToFloat(MapObj->FirstChild("Destination")->ToElement()->Attribute("Z"));

			int Heading = atoi(MapObj->FirstChild("DestinationHeading")->ToElement()->GetText());//DestinationHeading
			bool rotate = StringtoBool(MapObj->FirstChild("RotateContinuously")->ToElement()->GetText());
			GRAPHICS::DRAW_MARKER(Type, MarkerPos.x, MarkerPos.y, MarkerPos.z, 0, 0, 0, MarkerPos.Rotx, MarkerPos.Roty, MarkerPos.Rotz, Scale, Scale, Scale, Marker.R, Marker.G, Marker.B, Marker.A, 0, 0, 2, rotate, NULL, NULL, false);
			//GRAPHICS::DRAW_MARKER(2, v.x, v.y, v.z + 2, 0, 0, 0, 0, 180, 0, 2, 2, 2, 255, 128, 0, 50, 0, 1, 1, 0, 0, 0, 0);
		}
		else
		{
			LOG_PRINT("Find new Value %s", MapObj->Value());
		}
		MapObj = MapObj->NextSiblingElement();
		showNotification("Loading Map");
		WAIT(10);
	}

	for (int i = 0; i < TempHash.size(); i++)
	{
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(TempHash[i]);
	}

	
	//CAM::DO_SCREEN_FADE_IN(1000);
	showNotification("Load Complete!");
}



///<summary>
///   <para>
///       Function name:  To Load Map Editor
///       Function is part of Class: LoadMapEditor
///   </para>
///</summary>
///<param name="name">
///   <para>
///       Type: std::string File Name
///   </para>
///</param>
bool LoadMapEditor(std::string name)
{
	Hash ModelHash;
	Vector6 ModlePos;
	Vector4 Quaternion;
	Object obj;
	int PrimCol, SecCol;
	bool Dynamic;
	std::string Creater, Name;

	if (!Map.LoadFile((char*)name.c_str()))
	{
		return false;
	}
	MaprootElement = Map.RootElement();
	TiXmlElement *MapObj = MaprootElement->FirstChild("Objects")->FirstChild("MapObject")->ToElement();

	//LOG_PRINT("MaprootElement %s", root->Value());
	Creater = MaprootElement->FirstChild("Metadata")->FirstChild("Creator")->ToElement()->GetText();
	Name = MaprootElement->FirstChild("Metadata")->FirstChild("Name")->ToElement()->GetText();
	showNotification(Creater.c_str());
	showNotification(Name.c_str());
	WAIT(10);
	CAM::DO_SCREEN_FADE_OUT(1000);
	int spawnobj = 0;
	while (MapObj != NULL)
	{
		spawnobj++;

		//To get Entities Pos;
		ModlePos.x = MStringToFloat(MapObj->FirstChild("Position")->FirstChild("X")->ToElement()->GetText());
		ModlePos.y = MStringToFloat(MapObj->FirstChild("Position")->FirstChild("Y")->ToElement()->GetText());
		ModlePos.z = MStringToFloat(MapObj->FirstChild("Position")->FirstChild("Z")->ToElement()->GetText());
		ModlePos.Rotx = MStringToFloat(MapObj->FirstChild("Rotation")->FirstChild("X")->ToElement()->GetText());
		ModlePos.Roty = MStringToFloat(MapObj->FirstChild("Rotation")->FirstChild("Y")->ToElement()->GetText());
		ModlePos.Rotz = MStringToFloat(MapObj->FirstChild("Rotation")->FirstChild("Z")->ToElement()->GetText());

		Quaternion.x = MStringToFloat(MapObj->FirstChild("Quaternion")->FirstChild("X")->ToElement()->GetText());
		Quaternion.y = MStringToFloat(MapObj->FirstChild("Quaternion")->FirstChild("Y")->ToElement()->GetText());
		Quaternion.z = MStringToFloat(MapObj->FirstChild("Quaternion")->FirstChild("Z")->ToElement()->GetText());
		Quaternion.w = MStringToFloat(MapObj->FirstChild("Quaternion")->FirstChild("W")->ToElement()->GetText());
		ModelHash = atoi(MapObj->FirstChild("Hash")->ToElement()->GetText());
		Dynamic = StringtoBool(MapObj->FirstChild("Dynamic")->ToElement()->GetText());
		if (STREAMING::IS_MODEL_VALID(ModelHash) || STREAMING::IS_MODEL_IN_CDIMAGE(ModelHash))
		{
			STREAMING::REQUEST_MODEL(ModelHash);
			while (!STREAMING::HAS_MODEL_LOADED(ModelHash)) WAIT(0);

			switch (getType(MapObj->FirstChild("Type")->ToElement()->GetText()))
			{
			case no_entity:
				break;
			case ped:
				obj = PED::CREATE_PED(26, ModelHash, ModlePos.x, ModlePos.y, ModlePos.z, 0.0f, false, false);
				break;
			case vehicle:
				obj = VEHICLE::CREATE_VEHICLE(ModelHash, ModlePos.x, ModlePos.y, ModlePos.z, 0.0f, false, false);
				PrimCol = atoi(MapObj->FirstChild("PrimaryColor")->ToElement()->GetText());
				SecCol = atoi(MapObj->FirstChild("SecondaryColor")->ToElement()->GetText());
				VEHICLE::SET_VEHICLE_COLOURS(obj, PrimCol, SecCol);
				//VEHICLE::SET_VEHICLE_DOOR_OPEN(obj);
				//VEHICLE::SET_VEHICLE_DOOR_BROKEN
				break;
			case object:

				obj = OBJECT::CREATE_OBJECT(ModelHash, ModlePos.x, ModlePos.y, ModlePos.z, 1, 1, 1);//通过载具骨骼创建						
				break;
			default:
				break;
			}
			ENTITY::GET_ENTITY_QUATERNION(obj, &Quaternion.x, &Quaternion.y, &Quaternion.z, &Quaternion.w);
			ENTITY::SET_ENTITY_DYNAMIC(obj, Dynamic);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(obj);
			ENTITY::FREEZE_ENTITY_POSITION(obj, 1);
		}
		else
		{
			LOG_PRINT("Model is invaild");
		}



		LOG_PRINT("[%d] [%s] %s",
			spawnobj,
			MapObj->FirstChild("Type")->ToElement()->GetText(),
			MapObj->FirstChild("Hash")->ToElement()->GetText()
		);

		MapObj = MapObj->NextSiblingElement();
		WAIT(0);
	}
	teleport(
		ModlePos.x,
		ModlePos.y,
		ModlePos.z
	);
	CAM::DO_SCREEN_FADE_IN(1000);
	showNotification("Load Complete!");
	return true;
}

void LoadMap(std::string name)
{
	std::string MapPath = MapFolder + name + ".xml";
	if (Map.LoadFile((char*)MapPath.c_str()))
	{
		MaprootElement = Map.RootElement();
		LOG_PRINT("%s", MaprootElement->Value());

		if (!strcmp("Map", MaprootElement->Value()))
		{
			LOG_PRINT("Map Editor Version Found !");
			LoadMapEditor(MapPath);
		}
		else if (!strcmp("SpoonerPlacements", MaprootElement->Value()))
		{
			LOG_PRINT("Menyoo Map File");
			LoadMenyooMap(MaprootElement);
		}
		else
		{
			LOG_PRINT("Unknow type");
			showNotification("Sorry For this files, I could not Load this :P");
		}
	}
	else
	{
		showNotification("Ohh!this file Load failed");
	}
}