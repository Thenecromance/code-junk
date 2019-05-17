#pragma once


struct ObjectInfo
{
	float X;
	float Y;
	float Z;
	float RotX;
	float RotY;
	float RotZ;
	Hash modelhash;
	std::string modelname;
};

void LoadXmlVeh(std::string name);
std::vector<std::string> GetIniFile();
void SetModVehFolder();
void CreateFolder(std::string path);
std::string FloatToString(float flt);
Object SpawnObject(char* name, int selectplayer = 0);

float MStringToFloat(std::string str);
std::string FloatToString(float flt);
void LoadIniVeh(std::string name);
std::string GetJsonFile(std::string name);
Object SpawnPed(char* name, int selectplayer);