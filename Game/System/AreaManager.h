#pragma once
#include "../3D/Plane/Planes.h"
#include "../3D/Wall/Walls.h"

#include "GameManager.h"

class AreaManager
{
private:
	//領域
struct Area{
	int X;
	int Y;

	int Width;
	int Height;
};

//部屋
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

	void ParticleDraw();

	void Finalize();

	Vector3 GetExitPosition()	{return exitPosition;}
	Vector3 GetPlayerPosition()	{return PlayerPopPosition;}

	Vector3 GetObjectPopPosition();

private:
	//地面
	void PlaneInitialize();
	void PlaneUpdate();
	void PlaneBeatEndUpdate();
	void PlaneDraw();
	void PlaneFinalize();

	//壁
	void WallsInitialize();
	void WallUpdate();
	void WallBeatEndUpdate();
	void WallDraw();
	void WallParticleDraw();
	void WallFinalize();

	//ダンジョン生成
	void CreateMap();
	Room DevideRoom(Area area, bool hr = false);
	Room CreateRoom(Area area);
	void ConnectRoom(Room parent, Room childRoom, int divline, bool hr);

	//座標
	void ObjectRandomPop();
	

private:
	//借りもの
	Camera* camera = nullptr;
	GameManager* gameManager = nullptr;
	Vector3 PlayerPos={};

	static const int DIV_NUM = 31;
	//static const int DIV_NUM = 11;
	static const float Block_Size;

	//Plane
	std::unique_ptr<Planes> plane[DIV_NUM][DIV_NUM] = {};
	bool IsComboColorChange = false;

	//Wall
	bool IsAlive = false;
	char WallMap[DIV_NUM][DIV_NUM] = {};
	Walls* Wall[DIV_NUM][DIV_NUM] = {nullptr};

	//部屋
	std::vector<Room> rooms;

	//出口
	Vector3 exitPosition = {0,-5,0};

	//プレイヤー
	Vector3 PlayerPopPosition = {0,-3,0};

	//生成オブジェクト
	Vector3 ObjectPopPosition = {0,0,0};
};

