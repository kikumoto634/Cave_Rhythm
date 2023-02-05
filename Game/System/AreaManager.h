#pragma once
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
	void Initialize();

	void Update(Camera* camera, Vector3 PlayerPos);

	void BeatEndUpdate(GameManager* gameManager);

	void Draw();

	void Finalize();

	Vector3 GetExitPosition()	{return exitPosition;}

private:
	//�n��
	void PlaneInitialize();
	void PlaneUpdate();
	void PlaneBeatEndUpdate();
	void PlaneDraw();
	void PlaneFinalize();

	//��
	void WallsInitialize();
	void WallUpdate();
	void WallBeatEndUpdate();
	void WallDraw();
	void WallFinalize();

	void CreateMap();
	Room DevideRoom(Area area, bool hr = false);
	Room CreateRoom(Area area);
	void ConnectRoom(Room parent, Room childRoom, int divline, bool hr);

private:
	//�؂����
	Camera* camera = nullptr;
	GameManager* gameManager = nullptr;
	Vector3 PlayerPos={};

	static const int DIV_NUM = 25;
	static const float Block_Size;

	//Plane
	std::unique_ptr<Planes> plane[DIV_NUM][DIV_NUM] = {};
	bool IsComboColorChange = false;

	//Wall
	bool IsAlive = false;
	char WallMap[DIV_NUM][DIV_NUM] = {};
	Planes* Wall[DIV_NUM][DIV_NUM] = {nullptr};

	//�o��
	Vector3 exitPosition = {0,-5,0};
};

