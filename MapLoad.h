#pragma once
void LoadMap(std::string name);
bool LoadMapEditor(std::string name);
void SetMapPath(std::string Path);

enum EntType
{
	no_entity = 0,
	ped,
	vehicle,
	object
};