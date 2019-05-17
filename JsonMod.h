#pragma once

struct Rotation
{
	float Pitch;
	float Yaw;
	float Roll;
};

struct Tyre
{
	bool Burst;
	Color Smoke;
	int Type;

};

struct VehicleProperties
{
	int Pearlescent;
	int Primary;
	int Secondary;
	int WheelCol;

	int VehMod[49];

	int PlateType;
	std::string Platename;
	Tyre tyre;
	int WindowTint;
};

struct BasicProperties
{
	bool Collision;
	bool Driveable;
	bool Dynamic;
	bool EngineOn;
	bool Frozen;
	bool Invincible;
	int OpacityLevel;
	bool Visible;
};

struct AttachmentsProper
{
	std::string Bone;
	int ParentsId;
	Vector3 Position;
	Rotation Rot;
};

struct VehilceData
{
	bool HasParents;
	AttachmentsProper AttachTo;
	std::string name;
	Hash hash;
	std::string type;
	int initHandle;
	std::string Propname;
	BasicProperties BasicProper;
	Rotation Rot;

	VehicleProperties VehicleProper;
};

struct ObjectData
{
	bool HasParents;
	AttachmentsProper AttachTo;
	std::string name;
	Hash hash;
	std::string type;
	std::string Propname;
	int initHandle;
	BasicProperties BasicProper;
	Rotation Rot;
};


void LoadJsonVeh(std::string name);