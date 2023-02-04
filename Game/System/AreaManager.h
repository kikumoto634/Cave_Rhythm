#pragma once
#include "../3D/Plane/Planes.h"

#include "GameManager.h"

class AreaManager
{
private:
	//—Ìˆæ
struct Area{
	int X;
	int Y;

	int Width;
	int Height;
};

//•”‰®
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

private:
	//’n–Ê
	void PlaneInitialize();
	void PlaneUpdate();
	void PlaneBeatEndUpdate();
	void PlaneDraw();
	void PlaneFinalize();

	//•Ç
	void WallInitialize();
	void WallUpdate();
	void WallBeatEndUpdate();
	void WallDraw();
	void WallFinalize();

	void CreateMap();
	Room DevideRoom(Area area, bool hr = false);
	Room CreateRoom(Area area);
	void ConnectRoom(Room parent, Room childRoom, int divline, bool hr);

private:
	//ŽØ‚è‚à‚Ì
	Camera* camera = nullptr;
	GameManager* gameManager = nullptr;
	Vector3 PlayerPos={};

	static const int DIV_NUM = 25;
	static const float Block_Size;

	//Plane
	std::unique_ptr<Planes> plane[DIV_NUM][DIV_NUM];
	bool IsComboColorChange = false;

	//Wall
	bool IsAlive = false;
	char WallMap[DIV_NUM][DIV_NUM];
	std::unique_ptr<BaseObjObject> Wall[DIV_NUM][DIV_NUM];
};

