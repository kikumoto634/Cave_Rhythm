#pragma once
#include <sstream>
#include <string>

#include "../3D/Plane/Planes.h"
#include "../3D/Wall/Walls.h"

#include "GameManager.h"

class AreaManager
{
private:
	//�̈�
struct Area{
	int X;
	int Y;

	int Width;
	int Height;
};

//����
struct Room{
	int X;
	int Y;

	int Width;
	int Height;
};

public:
	void RandamAreaInitialize();
	void CSVAreaInitialize(std::string name);

	void Update(Camera* camera, Vector3 PlayerPos);

	void BeatEndUpdate(GameManager* gameManager);

	void Draw();

	void ParticleDraw();

	void Finalize();

	//�����_���p
	Vector3 GetExitPosition()	{return exitPosition;}
	Vector3 GetPlayerPosition()	{return PlayerPopPosition;}
	Vector3 GetObjectPopPosition();
	
	//CSV�p
	Vector3 GetCSVObjectPopPosition(int index);
	bool GetCSVObjectPopActive(int index, bool IsFlag = true);


private:
	//�n��
	void RandamAreaPlaneInitialize();
	void CSVAreaPlaneInitialize();
	void PlaneUpdate();
	void PlaneBeatEndUpdate();
	void PlaneDraw();
	void PlaneFinalize();

	//��
	void RandamAreaWallsInitialize();
	void CVSAreaWallsInitialize();
	void WallUpdate();
	void WallBeatEndUpdate();
	void WallDraw();
	void WallParticleDraw();
	void WallFinalize();

	//�_���W��������
	void CreateMap();
	Room DevideRoom(Area area, bool hr = false);
	Room CreateRoom(Area area);
	void ConnectRoom(Room parent, Room childRoom, int divline, bool hr);

	//CSV�ǂݍ���
	void CSVMapDataLoad(std::string fullPath);

	//���W
	void ObjectRandomPop();
	

private:
	//�؂����
	Camera* camera = nullptr;
	GameManager* gameManager = nullptr;
	Vector3 PlayerPos={};

	static const int DIV_NUM = 31;
	//static const int DIV_NUM = 6;
	static const float Block_Size;

	//Plane
	Planes* plane[DIV_NUM][DIV_NUM] = {nullptr};
	bool IsComboColorChange = false;

	//Wall
	bool IsAlive = false;
	char WallMap[DIV_NUM][DIV_NUM] = {};
	Walls* Wall[DIV_NUM][DIV_NUM] = {nullptr};

	//����
	std::vector<Room> rooms;

	//�o��
	Vector3 exitPosition = {0,-5,0};

	//�v���C���[
	Vector3 PlayerPopPosition = {0,-3,0};

	//�����I�u�W�F�N�g
	Vector3 ObjectPopPosition = {0,0,0};

	//CSV�R�}���h
	std::stringstream csvCommands;
	int CSVMap[DIV_NUM][DIV_NUM] = {0};
	std::vector<Vector3> ObjectPos;
	std::vector<bool> ObjectPopActive;

};

